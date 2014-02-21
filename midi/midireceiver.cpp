
#include "midireceiver.h"
#include "RtMidi.h"
#include "controller.h"

#include <assert.h>

MidiReceiver::MidiReceiver(Controller &controller)
    : m_Info(controller) {}

// from the perspective of channel 0
static const unsigned c_NoteOffMidi  = 0x80;
static const unsigned c_NoteOnMidi   = 0x90;


static bool RecognizedEvent(const std::vector<unsigned char> &message)
{
    if (message.size() != 3) {
        return false;
    }

    const unsigned char status = message[0];

    const unsigned upperNibble = status & 0xF0;
    if (upperNibble != c_NoteOffMidi &&
        upperNibble != c_NoteOnMidi) {
        return false;
    }

    return true;
}

static void ParseEvent(
        const std::vector<unsigned  char> &message,
        bool &noteOn, unsigned &noteNumber, unsigned &velocity)
{
    const unsigned char status = message[0];
    const unsigned upperNibble = status & 0xF0;

    noteOn     = (upperNibble == c_NoteOnMidi);
    noteNumber = message[1] & 0x7F;
    velocity   = message[2] & 0x7F;

    // some midi controllers send a note off as a note on
    // with a velocity of zero.
    if (noteOn && velocity == 0) {
        noteOn = false;
    }
}

void ReadMidiCallback(double deltatime,
                      std::vector<unsigned char> *message,
                      void *userData)
{
    assert(userData != NULL);
    assert(message != NULL);

    if (!RecognizedEvent(*message)) {
        return;
    }

    MidiReceiver::CallbackInfo &info = *static_cast<MidiReceiver::CallbackInfo*>(userData);

    bool noteOn         = false;
    unsigned noteNumber = 0;
    unsigned velocity   = 0;

    ParseEvent(*message, noteOn, noteNumber, velocity);

    if (noteOn) {
        info.m_Controller.NoteOn(noteNumber, velocity);
    }
    else {
        info.m_Controller.NoteOff(noteNumber, velocity);
    }
}

void MidiReceiver::GetMIDIPortInfo(PortInfo &info)
{
    // Check inputs.
    const unsigned int numPorts = m_MidiIn.getPortCount();
    for (unsigned int i=0; i < numPorts; i++) {
        try {
            const std::string portName = m_MidiIn.getPortName(i);
            info.push_back(std::make_pair(i, portName));
        }
        catch (RtError &) {
            // TODO: just move past this?
        }
    }
}

bool MidiReceiver::Initialize(unsigned portNumber)
{
    // Check available ports.
    const unsigned int nPorts = m_MidiIn.getPortCount();
    if ( nPorts == 0 ) {
        return false;
    }

    try {
        // TODO: make this more general?
        m_MidiIn.openPort(portNumber);
    }
    catch (RtError&) {
        return false;
    }

    // Set our callback function.  This should be done immediately after
    // opening the port to avoid having incoming messages written to the
    // queue.
    m_MidiIn.setCallback(ReadMidiCallback, (void*)&m_Info);

    // Don't ignore sysex, timing, or active sensing messages.
    m_MidiIn.ignoreTypes(true, true, true);

	return true;
}

