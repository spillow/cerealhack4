
#ifndef __CONTROLLER_H_
#define __CONTROLLER_H_

#include "shared.h"
#include "hardware.h"

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
    Controller(Hardware &hardware);
    void RunIteration();
private:
    std::map<unsigned, NoteId> m_InFlightNotes;
    Hardware &m_Hardware;
    ConcurrentQueue<MidiEvent> m_MidiQueue;
    ConcurrentQueue<GuiEvent> m_GuiQueue;
};

#endif // __CONTROLLER_H_

