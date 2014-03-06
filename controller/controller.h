
#ifndef __CONTROLLER_H_
#define __CONTROLLER_H_

#include "shared.h"
#include "hardware.h"
#include "midireceiver.h"

#include <functional>
#include <map>
#include <stack>

class Controller final
{
public:
    void NoteOn(unsigned noteNumber, unsigned velocity);
    void NoteOff(unsigned noteNumber, unsigned velocity);
    void RunIteration();
    void GetMIDIPortInfo(MidiReceiver::PortInfo &info);
    void Initialize(unsigned portNumber);
    Controller();

    ////////////////////////////////////////////////
    //
    // top level controls
    //
    // TODO: design question: do these take effect on currently
    // ringing notes or on future notes?
    //
    void SetVoice(Hardware::Instrument instrument);
    void SetSustain(bool sustain);
    void SetIntonation(float centDeltas[12]);
    // valid value range: [0.0f, 1.0f]
    void SetVolumes(float volumes[12]);
    void SetStandardPitch(float freq);
    void SetTransposeAmount(int numSemitones);
private:
    float m_StandardPitchFreq;
    bool m_IsSustained;
    int m_TransposeAmount;
    Hardware::Instrument m_CurrVoice;
    // The convention here is:
    // m_CentDeltasFromEqual[0] => C
    // m_CentDeltasFromEqual[1] => C#
    // ...
    // m_CentDeltasFromEqual[11] => B
    std::vector<float> m_CentDeltasFromEqual;
    // same convention as m_CentDeltasFromEqual
    std::vector<float> m_NoteVolumeScaler;
    typedef std::stack<NoteId> NoteCollection;
    // Notes currently making sound.
    std::map<unsigned, NoteCollection> m_RingingNotes;
    // Notes that are being held (no note off event yet)
    std::map<unsigned, unsigned> m_PushedDownNotes;
    ConcurrentQueue<std::function<void()> > m_MsgQueue;
    Hardware m_Hardware;
    MidiReceiver m_MidiReceiver;
private:
    Controller(const Controller &);
    Controller& operator=(const Controller&);
};

#endif // __CONTROLLER_H_

