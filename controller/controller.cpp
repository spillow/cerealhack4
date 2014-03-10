#include <math.h>

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
    m_NoteVolumeScaler(std::vector<float>(12, 1.0f)),
    m_CurrVoice(Hardware::Clarinet)
{
}

void Controller::GetMIDIPortInfo(MidiReceiver::PortInfo &info)
{
    m_MidiReceiver.GetMIDIPortInfo(info);
}

void Controller::Initialize(unsigned portNumber)
{
    m_Hardware.Initialize();
    m_MidiReceiver.Initialize(portNumber);
}

float Controller::CalculateFrequency(unsigned MIDINoteNumber, unsigned &noteIndex) const
{
    const float A4_Midi = 69.0f;
    const int transposedNote = (int)MIDINoteNumber + m_TransposeAmount;
    noteIndex = GetNoteIndex(transposedNote);
    float frequency = m_StandardPitchFreq * pow(2.0, ((float)transposedNote - A4_Midi) / 12.0f);
    frequency *= pow(10.0, c_CentConstant * m_CentDeltasFromEqual[noteIndex]);

    return frequency;
}

void Controller::NoteOn(unsigned noteNumber, unsigned velocity)
{
    auto thunk = [=]() {
        unsigned noteIndex = 0;
        const float frequency = CalculateFrequency(noteNumber, noteIndex);
        NoteId noteId = m_Hardware.NoteOn(m_CurrVoice, frequency, m_NoteVolumeScaler[noteIndex]);
        m_RingingNotes[noteNumber].push_back(noteId);
        m_PushedDownNotes[noteNumber]++;
    };

    m_MsgQueue.push(thunk);
}

void Controller::NoteOff(unsigned noteNumber, unsigned velocity)
{
    auto thunk = [=]() {
        auto &pushedNotes = m_PushedDownNotes[noteNumber];
        if (pushedNotes > 0) {
            pushedNotes--;
        }

        if (!m_IsSustained) {
            auto &ringingNotes = m_RingingNotes[noteNumber];
            if (!ringingNotes.empty()) {
                auto &noteId = ringingNotes.back();
                m_Hardware.NoteOff(noteId);
                ringingNotes.pop_back();
            }
        }
    };

    m_MsgQueue.push(thunk);
}

void Controller::SetVoice(Hardware::Instrument instrument)
{
    auto thunk = [=]() {
        m_CurrVoice = instrument;
    };

    m_MsgQueue.push(thunk);
}

void Controller::SetSustain(bool sustain)
{
    auto thunk = [=]() {
        m_IsSustained = sustain;

        if (sustain == false) {
            for (auto &kv : m_RingingNotes) {
                NoteCollection &coll = kv.second;
                const unsigned noteNumber  = kv.first;
                const unsigned numSustained = coll.size() - m_PushedDownNotes[noteNumber];
                for (unsigned i=0; i < numSustained; i++) {
                    NoteOff(noteNumber, 0);
                }
            }
        }
    };

    m_MsgQueue.push(thunk);
}

void Controller::UpdateFrequencies()
{
    for (auto &ringingNotes : m_RingingNotes) {
        unsigned idx;
        const float frequency = CalculateFrequency(ringingNotes.first, idx);
        for (auto &note : ringingNotes.second) {
            m_Hardware.UpdateFrequency(note, frequency);
        }
    }
}

void Controller::UpdateVolumes()
{
    for (auto &ringingNotes : m_RingingNotes) {
        for (auto &note : ringingNotes.second) {
            unsigned idx = 0;
            CalculateFrequency(ringingNotes.first, idx);
            float scaler = m_NoteVolumeScaler[idx];
            m_Hardware.UpdateVolume(note, scaler);
        }
    }
}

void Controller::SetIntonation(float centDeltas[12])
{
    auto thunk = [=]() {
        for (unsigned i=0; i < 12; i++) {
            m_CentDeltasFromEqual[i] = centDeltas[i];
        }

        UpdateFrequencies();
    };

    m_MsgQueue.push(thunk);
}

void Controller::SetVolumes(float volumes[12])
{
    auto thunk = [=]() {
        for (unsigned i=0; i < 12; i++) {
            m_NoteVolumeScaler[i] = volumes[i];
        }

        UpdateVolumes();
    };

    m_MsgQueue.push(thunk);
}

void Controller::SetStandardPitch(float freq)
{
    auto thunk = [=]() {
        m_StandardPitchFreq = freq;

        UpdateFrequencies();
    };

    m_MsgQueue.push(thunk);
}

void Controller::SetTransposeAmount(int numSemitones)
{
    auto thunk = [=]() {
        m_TransposeAmount = numSemitones;

        UpdateFrequencies();
    };

    m_MsgQueue.push(thunk);
}

void Controller::RunIteration()
{
    while (!m_MsgQueue.empty()) {
        auto event = m_MsgQueue.pop();
        event();
    }

    // FIXME: remove when we have interrupt based trigger
    stk::Stk::sleep(1);
}
