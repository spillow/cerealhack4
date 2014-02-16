
#include "hardware.h"
#include "shared.h"

#include "Stk.h"

#include "RtWvOut.h"
#include "RtAudio.h"

#include "Instrmnt.h"
#include "Clarinet.h"
#include "Flute.h"

#include <assert.h>
#include <set>
#include <list>

using namespace stk;

/////////////////////////////////////////////////////////////////////
/// \brief The NoteMessage struct
///
/// turnOn - if true then note is added, otherwise it is removed.
///
struct NoteMessage
{
    bool turnOn;
    Instrmnt *instrument;
    NoteMessage(bool turnOn, Instrmnt *instrument) :
        turnOn(turnOn), instrument(instrument) {}
};

/////////////////////////////////////////////////////////////////////
/// \brief The State struct
///
/// state of the hardware maintained between callback invocations.
/// Tracks messages sent from the controller thread and instruments
/// that are currently playing.
///
struct State
{
    std::set<Instrmnt*> m_Instruments;
    ConcurrentQueue<NoteMessage> m_Queue;
};

/////////////////////////////////////////////////////////////////////
/// \brief The Hardware::HardwareImpl class
///
/// Actual implementation of Hardware
///
class Hardware::HardwareImpl
{
    RtAudio m_DAC;
    State m_State;
public:
	~HardwareImpl();
    bool Initialize();
    NoteId NoteOn(Instrument i, float freq, float amplitude);
    void NoteOff(NoteId id);
};

/////////////////////////////////////////////////////////////////////
/// \brief MixSamples
/// \param samples
/// \return
///
/// Needs further thought here.  This takes all samples currently
/// being emitted by all instruments.  Currently mixed via simple
/// addition.  May need scaling to avoid clipping.
///
static StkFloat MixSamples(const std::list<StkFloat> &samples)
{
    StkFloat sum = 0.0f;
    for (auto sample : samples) {
        sum += sample;
    }

    return sum;
}

/////////////////////////////////////////////////////////////////////
/// \brief tick
/// \param outputBuffer
/// \param nBufferFrames
/// \param userState
/// \return
///
/// The main update method.  The audio thread will periodically
/// call tick() to fill up the audio buffer for playback.
///
static int tick(void *outputBuffer,
                void * /*inputBuffer*/,
                unsigned int nBufferFrames,
                double /*streamTime*/,
                RtAudioStreamStatus /*status*/,
                void *userState)
{
    assert(userState != NULL);
    assert(outputBuffer != NULL);

    State *currState = static_cast<State*>(userState);
    StkFloat *out = static_cast<StkFloat*>(outputBuffer);

    // read from the message queue to determine if the
    // controller has requested a change in the state
    // of the hardware.
    while (!currState->m_Queue.empty()) {
        NoteMessage msg = currState->m_Queue.pop();

        if (msg.turnOn) {
            currState->m_Instruments.insert(msg.instrument);
        }
        else {
            // TODO: what do we do to finish the off case?
            /*
            currState->m_Instruments.erase(msg.instrument);
            delete msg.instrument;
            */
        }
    }

    // nBufferFrames is the number of samples that we need to
    // fill up.  We're currently sampling at 44100 samples/sec.
    for (unsigned i=0; i < nBufferFrames; i++) {
        std::list<StkFloat> samples;
        for (auto instrument : currState->m_Instruments) {
            samples.push_back(instrument->tick());
        }

        *out = MixSamples(samples);
        out++;
    }

    return 0;
}

Hardware::Hardware()
{
    m_HardwareImpl = new HardwareImpl;
}

Hardware::~Hardware()
{
    delete m_HardwareImpl;
}

bool Hardware::Initialize()
{
    return m_HardwareImpl->Initialize();
}

NoteId Hardware::NoteOn(Instrument i, float freq, float amplitude)
{
    return m_HardwareImpl->NoteOn(i, freq, amplitude);
}

void Hardware::NoteOff(NoteId id)
{
    return m_HardwareImpl->NoteOff(id);
}

/////////////////////////////////////////////////////////////////////
/// \brief Hardware::HardwareImpl::NoteOn
/// \param instrument
/// \param freq
/// \param amplitude
/// \return
///
/// Called from the controller thread.
/// Creates a new note and fires off a message to the audio thread
/// to add the note into the mix.
///
NoteId Hardware::HardwareImpl::NoteOn(Instrument instrument, float freq, float amplitude)
{
    Instrmnt *i = NULL;
    switch (instrument)
    {
    case Hardware::Clarinet:
        i = new stk::Clarinet(10.0f);
        break;
    case Hardware::Flute:
        i = new stk::Flute(10.0f);
        break;
    default:
        return 0;
    }

    // start the physical process.
    i->noteOn(freq, amplitude);

    // notify audio thread to pick new note up.
    m_State.m_Queue.push(NoteMessage(true, i));

    // Send a pointer back to the caller of the generated object.
    // This is used as a unique tag to reference it for deletion
    // later.
    return reinterpret_cast<NoteId>(i);
}

/////////////////////////////////////////////////////////////////////
/// \brief Hardware::HardwareImpl::NoteOff
/// \param id
///
void Hardware::HardwareImpl::NoteOff(NoteId id)
{
    assert(id != NULL);
    Instrmnt *instrument = reinterpret_cast<Instrmnt*>(id);
    instrument->noteOff(0.003f); // TODO: how sharp?
    m_State.m_Queue.push(NoteMessage(false, instrument));
}

/////////////////////////////////////////////////////////////////////
/// \brief Hardware::HardwareImpl::~HardwareImpl
///
/// Kills the audio thread.
///
Hardware::HardwareImpl::~HardwareImpl()
{
	m_DAC.closeStream();
}

/////////////////////////////////////////////////////////////////////
/// \brief Hardware::HardwareImpl::Initialize
/// \return
///
/// Must be called before firing notes off.
///
bool Hardware::HardwareImpl::Initialize()
{
    Stk::setSampleRate(44100.0);
    // CH_TODO: what does this do?
    Stk::setRawwavePath(Stk::rawwavePath());

    // Setup the parameters
    RtAudio::StreamParameters parameters;
    // CH_TODO: what are the options here?
    parameters.deviceId = m_DAC.getDefaultOutputDevice();
    parameters.nChannels = 1;
    const RtAudioFormat format = ( sizeof(StkFloat) == 8 ) ? RTAUDIO_FLOAT64 : RTAUDIO_FLOAT32;
    unsigned int bufferFrames = RT_BUFFER_SIZE;

    try {
      m_DAC.openStream(&parameters, NULL, format,
                       (unsigned int)Stk::sampleRate(), &bufferFrames,
                       tick, (void *)&m_State);
    }
    catch (RtError&) {
        return false;
    }

    try {
      m_DAC.startStream();
    }
    catch (RtError&) {
      return false;
    }

    return true;
}
