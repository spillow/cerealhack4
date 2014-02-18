
#ifndef __CONTROLLER_H_
#define __CONTROLLER_H_

#include "shared.h"
#include "hardware.h"
#include "midireceiver.h"

#include <map>

class Controller
{
public:
    struct MidiEvent {
        enum Type {
            NOTE_ON,
            NOTE_OFF
        };
        Type     eventType;
        unsigned noteNumber;
        unsigned velocity;
        MidiEvent(Type eventType, unsigned noteNumber, unsigned velocity) :
            eventType(eventType), noteNumber(noteNumber), velocity(velocity) {}
    };
    struct GuiEvent {
        // TODO
    };
public:
    void NoteOn(unsigned noteNumber, unsigned velocity);
    void NoteOff(unsigned noteNumber, unsigned velocity);
    void RunIteration();
    void Initialize();
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
    std::vector<float> m_NoteVolumes;
    std::map<unsigned, NoteId> m_InFlightNotes;
    ConcurrentQueue<MidiEvent> m_MidiQueue;
    ConcurrentQueue<GuiEvent> m_GuiQueue;
    Hardware m_Hardware;
    MidiReceiver m_MidiReceiver;
};

#endif // __CONTROLLER_H_

