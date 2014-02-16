
#include "hardware.h"

#include "Stk.h"

#include "RtWvOut.h"
#include "RtAudio.h"

#include "Instrmnt.h"
#include "Clarinet.h"
#include "Flute.h"

#include <assert.h>
#include <set>
#include <list>

class MessageQueue;

using namespace stk;

struct State
{
    std::set<Instrmnt*> m_Instruments;
    MessageQueue *m_Queue;
};

class Hardware::HardwareImpl
{
    RtAudio m_DAC;
    State m_State;
public:
    bool Initialize();
    NoteId NoteOn(Instrument i, float freq, float amplitude);
    void NoteOff(NoteId id);
};

static StkFloat MixSamples(const std::list<StkFloat> &samples)
{
    StkFloat sum = 0.0f;
    for (auto sample : samples) {
        sum += sample;
    }

    return sum;
}

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
    // FIXME: write to queue.
    m_State.m_Instruments.insert(i);

    return reinterpret_cast<NoteId>(i);
}

void Hardware::HardwareImpl::NoteOff(NoteId id)
{
}

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
