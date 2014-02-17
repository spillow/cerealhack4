
#include "controller.h"
#include "shared.h"

void Controller::NoteOn(unsigned noteNumber, unsigned velocity)
{
    m_MidiQueue.push(MidiEvent(Controller::MidiEvent::NOTE_ON, noteNumber, velocity));
}

void Controller::NoteOff(unsigned noteNumber, unsigned velocity)
{
    m_MidiQueue.push(MidiEvent(Controller::MidiEvent::NOTE_OFF, noteNumber, velocity));
}

void Controller::RunIteration()
{
    while (!m_MidiQueue.empty()) {
        Controller::MidiEvent event = m_MidiQueue.pop();

        // for debug
        if (event.noteNumber == 0x60) {
            throw 10;
        }

        if (event.eventType == Controller::MidiEvent::NOTE_ON) {
            float frequency = 220.0f * pow(2.0, ((float)event.noteNumber - 57.0f) / 12.0f);
            m_InFlightNotes[event.noteNumber] = m_Hardware.NoteOn(Hardware::Clarinet, frequency, 0.1f);
        }
        else {
            auto iter = m_InFlightNotes.find(event.noteNumber);
            if (iter != m_InFlightNotes.end()) {
                m_Hardware.NoteOff(iter->second);
                m_InFlightNotes.erase(iter);
            }
        }
    }

    while (!m_GuiQueue.empty()) {
        // TODO
    }
}

Controller::Controller(Hardware &hardware) :
    m_Hardware(hardware) {}
