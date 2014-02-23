#-------------------------------------------------
#
# Project created by QtCreator 2014-02-08T16:12:40
#
#-------------------------------------------------

win32: CONFIG += win
win64: CONFIG += win

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = cerealhackgui
TEMPLATE = app

STK_PATH        = ../stk-4.4.4/
HW_PATH         = ../hardware/
SHARED_PATH     = ../shared/
CONTROLLER_PATH = ../controller/
MIDI_PATH       = ../midi/

INCLUDEPATH += $${STK_PATH}/include
INCLUDEPATH += $${STK_PATH}/src/include
INCLUDEPATH += $${SHARED_PATH}
INCLUDEPATH += $${CONTROLLER_PATH}
INCLUDEPATH += $${MIDI_PATH}

INCLUDEPATH += $${HW_PATH}

DEFINES += LITTLE_ENDIAN
DEFINES += RAWWAVE_PATH="\\\"$${STK_PATH}/rawwaves/\\\""

#USE_ASIO = 1

win {
    # CH_FIXME: look into gettimeofday() equivalent for windows
    DEFINES += __WINDOWS_DS__
    DEFINES += __WINDOWS_MM__
    !isEmpty(USE_ASIO) {
        DEFINES += __WINDOWS_ASIO__
        DEFINES -= UNICODE
    }

    LIBS += -lole32 -lwinmm -lWsock32 -ldsound -lwinmm
    !isEmpty(USE_ASIO) {
        LIBS += -ladvapi32
    }
}

macx {
    DEFINES += HAVE_GETTIMEOFDAY
    DEFINES  += __MACOSX_CORE__

    LIBS += -lpthread -lm -framework CoreAudio -framework CoreFoundation -framework CoreMidi
}


SOURCES += main.cpp\
           mainwindow.cpp\
           $${HW_PATH}/hardware.cpp\
           $${SHARED_PATH}/shared.cpp\
           $${CONTROLLER_PATH}/controller.cpp\
           $${MIDI_PATH}/midireceiver.cpp\
        $${STK_PATH}/src/ADSR.cpp\
        $${STK_PATH}/src/Asymp.cpp\
        $${STK_PATH}/src/BandedWG.cpp\
        $${STK_PATH}/src/BeeThree.cpp\
        $${STK_PATH}/src/BiQuad.cpp\
        $${STK_PATH}/src/Blit.cpp\
        $${STK_PATH}/src/BlitSaw.cpp\
        $${STK_PATH}/src/BlitSquare.cpp\
        $${STK_PATH}/src/BlowBotl.cpp\
        $${STK_PATH}/src/BlowHole.cpp\
        $${STK_PATH}/src/Bowed.cpp\
        $${STK_PATH}/src/Brass.cpp\
        $${STK_PATH}/src/Chorus.cpp\
        $${STK_PATH}/src/Clarinet.cpp\
        $${STK_PATH}/src/Delay.cpp\
        $${STK_PATH}/src/DelayA.cpp\
        $${STK_PATH}/src/DelayL.cpp\
        $${STK_PATH}/src/Drummer.cpp\
        $${STK_PATH}/src/Echo.cpp\
        $${STK_PATH}/src/Envelope.cpp\
        $${STK_PATH}/src/FileLoop.cpp\
        $${STK_PATH}/src/FileRead.cpp\
        $${STK_PATH}/src/FileWrite.cpp\
        $${STK_PATH}/src/FileWvIn.cpp\
        $${STK_PATH}/src/FileWvOut.cpp\
        $${STK_PATH}/src/Fir.cpp\
        $${STK_PATH}/src/Flute.cpp\
        $${STK_PATH}/src/FM.cpp\
        $${STK_PATH}/src/FMVoices.cpp\
        $${STK_PATH}/src/FormSwep.cpp\
        $${STK_PATH}/src/FreeVerb.cpp\
        $${STK_PATH}/src/Granulate.cpp\
        $${STK_PATH}/src/Guitar.cpp\
        $${STK_PATH}/src/HevyMetl.cpp\
        $${STK_PATH}/src/Iir.cpp\
        $${STK_PATH}/src/InetWvIn.cpp\
        $${STK_PATH}/src/InetWvOut.cpp\
        $${STK_PATH}/src/JCRev.cpp\
        $${STK_PATH}/src/LentPitShift.cpp\
        $${STK_PATH}/src/Mandolin.cpp\
        $${STK_PATH}/src/Mesh2D.cpp\
        $${STK_PATH}/src/Messager.cpp\
        $${STK_PATH}/src/MidiFileIn.cpp\
        $${STK_PATH}/src/Modal.cpp\
        $${STK_PATH}/src/ModalBar.cpp\
        $${STK_PATH}/src/Modulate.cpp\
        $${STK_PATH}/src/Moog.cpp\
        $${STK_PATH}/src/Mutex.cpp\
        $${STK_PATH}/src/Noise.cpp\
        $${STK_PATH}/src/NRev.cpp\
        $${STK_PATH}/src/OnePole.cpp\
        $${STK_PATH}/src/OneZero.cpp\
        $${STK_PATH}/src/PercFlut.cpp\
        $${STK_PATH}/src/Phonemes.cpp\
        $${STK_PATH}/src/PitShift.cpp\
        $${STK_PATH}/src/Plucked.cpp\
        $${STK_PATH}/src/PoleZero.cpp\
        $${STK_PATH}/src/PRCRev.cpp\
        $${STK_PATH}/src/Resonate.cpp\
        $${STK_PATH}/src/Rhodey.cpp\
        $${STK_PATH}/src/RtAudio.cpp\
        $${STK_PATH}/src/RtMidi.cpp\
        $${STK_PATH}/src/RtWvIn.cpp\
        $${STK_PATH}/src/RtWvOut.cpp\
        $${STK_PATH}/src/Sampler.cpp\
        $${STK_PATH}/src/Saxofony.cpp\
        $${STK_PATH}/src/Shakers.cpp\
        $${STK_PATH}/src/Simple.cpp\
        $${STK_PATH}/src/SineWave.cpp\
        $${STK_PATH}/src/SingWave.cpp\
        $${STK_PATH}/src/Sitar.cpp\
        $${STK_PATH}/src/Skini.cpp\
        $${STK_PATH}/src/Socket.cpp\
        $${STK_PATH}/src/Sphere.cpp\
        $${STK_PATH}/src/StifKarp.cpp\
        $${STK_PATH}/src/Stk.cpp\
        $${STK_PATH}/src/TapDelay.cpp\
        $${STK_PATH}/src/TcpClient.cpp\
        $${STK_PATH}/src/TcpServer.cpp\
        $${STK_PATH}/src/Thread.cpp\
        $${STK_PATH}/src/TubeBell.cpp\
        $${STK_PATH}/src/Twang.cpp\
        $${STK_PATH}/src/TwoPole.cpp\
        $${STK_PATH}/src/TwoZero.cpp\
        $${STK_PATH}/src/UdpSocket.cpp\
        $${STK_PATH}/src/Voicer.cpp\
        $${STK_PATH}/src/VoicForm.cpp\
        $${STK_PATH}/src/Whistle.cpp\
        $${STK_PATH}/src/Wurley.cpp

!isEmpty(USE_ASIO) {
SOURCES += \
        $${STK_PATH}/src/include/asio.cpp\
        $${STK_PATH}/src/include/asiodrivers.cpp\
        $${STK_PATH}/src/include/asiolist.cpp\
        $${STK_PATH}/src/include/iasiothiscallresolver.cpp
}

HEADERS  += mainwindow.h\
            $${HW_PATH}/hardware.h\
            $${SHARED_PATH}/shared.h\
            $${CONTROLLER_PATH}/controller.h\
            $${MIDI_PATH}/midireceiver.h\
            $${STK_PATH}/src/include/asio.h\
            $${STK_PATH}/src/include/asiodrivers.h\
            $${STK_PATH}/src/include/asiodrvr.h\
            $${STK_PATH}/src/include/asiolist.h\
            $${STK_PATH}/src/include/asiosys.h\
            $${STK_PATH}/src/include/dsound.h\
            $${STK_PATH}/src/include/ginclude.h\
            $${STK_PATH}/src/include/iasiodrv.h\
            $${STK_PATH}/src/include/iasiothiscallresolver.h\
            $${STK_PATH}/src/include/soundcard.h\
            $${STK_PATH}/include/ADSR.h\
            $${STK_PATH}/include/Asymp.h\
            $${STK_PATH}/include/BandedWG.h\
            $${STK_PATH}/include/BeeThree.h\
            $${STK_PATH}/include/BiQuad.h\
            $${STK_PATH}/include/Blit.h\
            $${STK_PATH}/include/BlitSaw.h\
            $${STK_PATH}/include/BlitSquare.h\
            $${STK_PATH}/include/BlowBotl.h\
            $${STK_PATH}/include/BlowHole.h\
            $${STK_PATH}/include/Bowed.h\
            $${STK_PATH}/include/BowTable.h\
            $${STK_PATH}/include/Brass.h\
            $${STK_PATH}/include/Chorus.h\
            $${STK_PATH}/include/Clarinet.h\
            $${STK_PATH}/include/Cubic.h\
            $${STK_PATH}/include/Delay.h\
            $${STK_PATH}/include/DelayA.h\
            $${STK_PATH}/include/DelayL.h\
            $${STK_PATH}/include/Drummer.h\
            $${STK_PATH}/include/Echo.h\
            $${STK_PATH}/include/Effect.h\
            $${STK_PATH}/include/Envelope.h\
            $${STK_PATH}/include/FileLoop.h\
            $${STK_PATH}/include/FileRead.h\
            $${STK_PATH}/include/FileWrite.h\
            $${STK_PATH}/include/FileWvIn.h\
            $${STK_PATH}/include/FileWvOut.h\
            $${STK_PATH}/include/Filter.h\
            $${STK_PATH}/include/Fir.h\
            $${STK_PATH}/include/Flute.h\
            $${STK_PATH}/include/FM.h\
            $${STK_PATH}/include/FMVoices.h\
            $${STK_PATH}/include/FormSwep.h\
            $${STK_PATH}/include/FreeVerb.h\
            $${STK_PATH}/include/Function.h\
            $${STK_PATH}/include/Generator.h\
            $${STK_PATH}/include/Granulate.h\
            $${STK_PATH}/include/Guitar.h\
            $${STK_PATH}/include/HevyMetl.h\
            $${STK_PATH}/include/Iir.h\
            $${STK_PATH}/include/InetWvIn.h\
            $${STK_PATH}/include/InetWvOut.h\
            $${STK_PATH}/include/Instrmnt.h\
            $${STK_PATH}/include/JCRev.h\
            $${STK_PATH}/include/JetTable.h\
            $${STK_PATH}/include/LentPitShift.h\
            $${STK_PATH}/include/Mandolin.h\
            $${STK_PATH}/include/Mesh2D.h\
            $${STK_PATH}/include/Messager.h\
            $${STK_PATH}/include/MidiFileIn.h\
            $${STK_PATH}/include/Modal.h\
            $${STK_PATH}/include/ModalBar.h\
            $${STK_PATH}/include/Modulate.h\
            $${STK_PATH}/include/Moog.h\
            $${STK_PATH}/include/Mutex.h\
            $${STK_PATH}/include/Noise.h\
            $${STK_PATH}/include/NRev.h\
            $${STK_PATH}/include/OnePole.h\
            $${STK_PATH}/include/OneZero.h\
            $${STK_PATH}/include/PercFlut.h\
            $${STK_PATH}/include/Phonemes.h\
            $${STK_PATH}/include/PitShift.h\
            $${STK_PATH}/include/Plucked.h\
            $${STK_PATH}/include/PoleZero.h\
            $${STK_PATH}/include/PRCRev.h\
            $${STK_PATH}/include/ReedTable.h\
            $${STK_PATH}/include/Resonate.h\
            $${STK_PATH}/include/Rhodey.h\
            $${STK_PATH}/include/RtAudio.h\
            $${STK_PATH}/include/RtError.h\
            $${STK_PATH}/include/RtMidi.h\
            $${STK_PATH}/include/RtWvIn.h\
            $${STK_PATH}/include/RtWvOut.h\
            $${STK_PATH}/include/Sampler.h\
            $${STK_PATH}/include/Saxofony.h\
            $${STK_PATH}/include/Shakers.h\
            $${STK_PATH}/include/Simple.h\
            $${STK_PATH}/include/SineWave.h\
            $${STK_PATH}/include/SingWave.h\
            $${STK_PATH}/include/Sitar.h\
            $${STK_PATH}/include/Skini.h\
            $${STK_PATH}/include/SKINI.msg\
            $${STK_PATH}/include/SKINI.tbl\
            $${STK_PATH}/include/Socket.h\
            $${STK_PATH}/include/Sphere.h\
            $${STK_PATH}/include/StifKarp.h\
            $${STK_PATH}/include/Stk.h\
            $${STK_PATH}/include/TapDelay.h\
            $${STK_PATH}/include/TcpClient.h\
            $${STK_PATH}/include/TcpServer.h\
            $${STK_PATH}/include/Thread.h\
            $${STK_PATH}/include/TubeBell.h\
            $${STK_PATH}/include/Twang.h\
            $${STK_PATH}/include/TwoPole.h\
            $${STK_PATH}/include/TwoZero.h\
            $${STK_PATH}/include/UdpSocket.h\
            $${STK_PATH}/include/Vector3D.h\
            $${STK_PATH}/include/Voicer.h\
            $${STK_PATH}/include/VoicForm.h\
            $${STK_PATH}/include/Whistle.h\
            $${STK_PATH}/include/Wurley.h\
            $${STK_PATH}/include/WvIn.h\
            $${STK_PATH}/include/WvOut.h

FORMS    += mainwindow.ui
