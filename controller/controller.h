
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
private:
    std::map<unsigned, NoteId> m_InFlightNotes;
    ConcurrentQueue<MidiEvent> m_MidiQueue;
    ConcurrentQueue<GuiEvent> m_GuiQueue;
    Hardware m_Hardware;
    MidiReceiver m_MidiReceiver;
};

#endif // __CONTROLLER_H_

