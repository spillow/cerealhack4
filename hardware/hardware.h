
#ifndef __HARDWARE_H_
#define __HARDWARE_H_

class NoteId_;

typedef NoteId_* NoteId;

class Hardware final
{
    class HardwareImpl;
    HardwareImpl *m_HardwareImpl;
public:
    enum Instrument {
        Clarinet,
        Flute,
        Brass,
        Saxofony,
        Bowed
    };
public:
    Hardware();
    ~Hardware();
    bool Initialize();
    NoteId NoteOn(Instrument instrument, float freq, float amplitude);
    void NoteOff(NoteId id);
    void UpdateFrequency(NoteId id, float frequency);
    void UpdateVolume(NoteId id, float scaler);
private:
    Hardware(const Hardware&);
    Hardware& operator=(const Hardware&);
};

#endif // __HARDWARE_H_
