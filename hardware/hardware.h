
#ifndef __HARDWARE_H_
#define __HARDWARE_H_

#include "RtAudio.h"

class NoteId_;

typedef NoteId_* NoteId;

class State;

class Hardware
{
    class HardwareImpl;
    HardwareImpl *m_HardwareImpl;
public:
    enum Instrument {
        Clarinet,
        Flute
    };
public:
    Hardware();
    ~Hardware();
    bool Initialize();
    NoteId NoteOn(Instrument instrument, float freq, float amplitude);
    void NoteOff(NoteId id);
};

#endif // __HARDWARE_H_
