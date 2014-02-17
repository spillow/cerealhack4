
#ifndef __MIDIRECEIVER_H_
#define __MIDIRECEIVER_H_

#include "controller.h"
#include "RtMidi.h"

#include <set>

class MidiReceiver
{
public:
    struct CallbackInfo
    {
        std::set<unsigned> m_InFlightNotes;
        Controller &m_Controller;
        CallbackInfo(Controller &controller) :
            m_Controller(controller) {}
    };
    bool Initialize();
    MidiReceiver(Controller &controller);
private:
    CallbackInfo m_Info;
    RtMidiIn m_MidiIn;
};

#endif // __MIDIRECEIVER_H_

