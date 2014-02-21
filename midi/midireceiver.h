
#ifndef __MIDIRECEIVER_H_
#define __MIDIRECEIVER_H_

#include "RtMidi.h"
#include <set>
#include <list>

class Controller;

class MidiReceiver
{
public:
    typedef std::list<std::pair<unsigned, std::string> > PortInfo;
    void GetMIDIPortInfo(PortInfo &info);
public:
    struct CallbackInfo
    {
        Controller &m_Controller;
        CallbackInfo(Controller &controller) :
            m_Controller(controller) {}
    };
    bool Initialize(unsigned portNumber);
    MidiReceiver(Controller &controller);
private:
    CallbackInfo m_Info;
    RtMidiIn m_MidiIn;
};

#endif // __MIDIRECEIVER_H_

