
#include "hardware.h"
#include "shared.h"

#include "Stk.h"

#include "RtWvOut.h"
#include "RtAudio.h"

#include "Instrmnt.h"
#include "Clarinet.h"
#include "Flute.h"
#include "Brass.h"
#include "Saxofony.h"
#include "Bowed.h"

#include <assert.h>
#include <map>
#include <list>

using namespace stk;

static const float c_SampleRate = 44100.0f;
static const float c_ONE_OVER_128 = 1.0f / 128.0f;

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
/// \brief The InstrumentState struct
///
/// Keeps track of the decay of a note after a note off event.
///
struct InstrumentState
{
    // has the note been released?
    bool m_Decaying;
    // number of samples that have passed since note release.
    unsigned m_DecayTime;
    InstrumentState() :
        m_Decaying(false), m_DecayTime(0) {}
};

typedef std::map<Instrmnt*, InstrumentState> StatesType;

/////////////////////////////////////////////////////////////////////
/// \brief The State struct
///
/// state of the hardware maintained between callback invocations.
/// Tracks messages sent from the controller thread and instruments
/// that are currently playing.
///
struct State
{
    static const unsigned m_DecayTimeInSamples;
    StatesType m_InstrumentStates;
    ConcurrentQueue<NoteMessage> m_Queue;
};

// We allow for a note to hold on for 0.2 sec after release at which
// point it will be deleted.
const unsigned State::m_DecayTimeInSamples = unsigned(0.2f * c_SampleRate);

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

    StatesType &instrumentMap = currState->m_InstrumentStates;

    // read from the message queue to determine if the
    // controller has requested a change in the state
    // of the hardware.
    while (!currState->m_Queue.empty()) {
        NoteMessage msg = currState->m_Queue.pop();

        if (msg.turnOn) {
            assert(instrumentMap.find(msg.instrument) == instrumentMap.end());
            instrumentMap.insert(std::make_pair(msg.instrument, InstrumentState()));
        }
        else {
            auto iter = instrumentMap.find(msg.instrument);
            assert(iter != instrumentMap.end());
            iter->second.m_Decaying = true;
        }
    }

    // nBufferFrames is the number of samples that we need to
    // fill up.  We're currently sampling at 44100 samples/sec.
    for (unsigned i=0; i < nBufferFrames; i++) {
        std::list<StkFloat> samples;
        for (auto &instrumentState : instrumentMap) {
            samples.push_back(instrumentState.first->tick());
        }

        *out = MixSamples(samples);
        out++;
    }

    {
        std::list<Instrmnt*> toRemove;
        // bump decay counters for instruments
        for (auto &instrumentState : instrumentMap) {
            Instrmnt *instrument   = instrumentState.first;
            InstrumentState &state = instrumentState.second;
            if (state.m_Decaying) {
                state.m_DecayTime += nBufferFrames;
            }

            if (state.m_DecayTime >= State::m_DecayTimeInSamples) {
                toRemove.push_back(instrument);
            }
        }

        for (auto item : toRemove) {
            instrumentMap.erase(item);
            delete item;
        }
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
    // TODO: where to turn the knobs on these?
    switch (instrument)
    {
    case Hardware::Clarinet:
        i = new stk::Clarinet(10.0f);
        break;
    case Hardware::Flute:
        i = new stk::Flute(10.0f);
        break;
    case Hardware::Brass:
        i = new stk::Brass();
        break;
    case Hardware::Saxofony:
        i = new stk::Saxofony(10.0f);
        break;
    case Hardware::Bowed:
        i = new stk::Bowed();
        break;
    default:
        assert(0 && "unknown instrument");
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
    //instrument->noteOff(0.003f); // TODO: how sharp?
    instrument->noteOff(0.4f * c_ONE_OVER_128); // TODO: how sharp?
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
    Stk::setSampleRate(c_SampleRate);
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
