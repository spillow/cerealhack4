
#include "controller.h"
#include "shared.h"

#include "Stk.h"

//////////////////////////////////////////////////////
///
/// GetNoteIndex()
///
/// Takes a MIDI note number and returns its value
/// inside the range [0-11] which corresponds to
/// the convention from m_CentDeltasFromEqual.
///
static unsigned GetNoteIndex(int noteNumber)
{
    const int C4_Midi = 60;
    const int diff = noteNumber - C4_Midi;

    if (diff >= 0) {
        return diff % 12;
    }
    else {
        const unsigned wrap = abs(diff) % 12;
        if (wrap == 0) {
            return 0;
        }
        else {
            return 12 - wrap;
        }
    }
}

// log10(2) / 1200
const float c_CentConstant = 2.50858329719984e-004f;

Controller::Controller() :
    m_MidiReceiver(*this), m_TransposeAmount(0), m_IsSustained(false),
    m_StandardPitchFreq(440.0f),
    m_CentDeltasFromEqual(std::vector<float>(12, 0.0f)),
    m_NoteVolumes(std::vector<float>(12, 0.1f)),
    m_CurrVoice(Hardware::Clarinet)
{
}

void Controller::Initialize()
{
    m_Hardware.Initialize();
    m_MidiReceiver.Initialize();
}

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
    const float A4_Midi = 69.0f;

    while (!m_MidiQueue.empty()) {
        Controller::MidiEvent event = m_MidiQueue.pop();

        // for debug
        if (event.noteNumber == 0x60) {
            throw 10;
        }

        if (event.eventType == Controller::MidiEvent::NOTE_ON) {
            // TODO: what happens if we're in sustain and a note that is already
            // sustained is hit again?
            const int transposedNote = (int)event.noteNumber + m_TransposeAmount;
            const unsigned noteIndex = GetNoteIndex(transposedNote);
            float frequency = m_StandardPitchFreq * pow(2.0, ((float)transposedNote - A4_Midi) / 12.0f);
            frequency *= pow(10.0, c_CentConstant * m_CentDeltasFromEqual[noteIndex]);
            m_InFlightNotes[event.noteNumber] = m_Hardware.NoteOn(m_CurrVoice, frequency, m_NoteVolumes[noteIndex]);
        }
        else {
            if (!m_IsSustained) {
                auto iter = m_InFlightNotes.find(event.noteNumber);
                if (iter != m_InFlightNotes.end()) {
                    m_Hardware.NoteOff(iter->second);
                    m_InFlightNotes.erase(iter);
                }
            }
        }
    }

    while (!m_GuiQueue.empty()) {
        // TODO
    }

    // FIXME: remove when we have interrupt based trigger
    stk::Stk::sleep(1);
}
