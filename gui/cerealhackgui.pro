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

INCLUDEPATH += ../stk-4.4.4/include
INCLUDEPATH += ../stk-4.4.4/src/include

DEFINES += HAVE_GETTIMEOFDAY
DEFINES += LITTLE_ENDIAN
DEFINES += RAWWAVE_PATH="\"../stk-4.4.4/rawwaves/\""

win {
    DEFINES += __WINDOWS_DS__
    DEFINES += __WINDOWS_MM__

    LIBS += -lole32 -lwinmm -lWsock32 -ldsound -lwinmm
}

macx {
    DEFINES  += __MACOSX_CORE__

    LIBS += -lpthread -lm -framework CoreAudio -framework CoreFoundation -framework CoreMidi
}


SOURCES += main.cpp\
           mainwindow.cpp\
        ../stk-4.4.4/src/ADSR.cpp\
        ../stk-4.4.4/src/Asymp.cpp\
        ../stk-4.4.4/src/BandedWG.cpp\
        ../stk-4.4.4/src/BeeThree.cpp\
        ../stk-4.4.4/src/BiQuad.cpp\
        ../stk-4.4.4/src/Blit.cpp\
        ../stk-4.4.4/src/BlitSaw.cpp\
        ../stk-4.4.4/src/BlitSquare.cpp\
        ../stk-4.4.4/src/BlowBotl.cpp\
        ../stk-4.4.4/src/BlowHole.cpp\
        ../stk-4.4.4/src/Bowed.cpp\
        ../stk-4.4.4/src/Brass.cpp\
        ../stk-4.4.4/src/Chorus.cpp\
        ../stk-4.4.4/src/Clarinet.cpp\
        ../stk-4.4.4/src/Delay.cpp\
        ../stk-4.4.4/src/DelayA.cpp\
        ../stk-4.4.4/src/DelayL.cpp\
        ../stk-4.4.4/src/Drummer.cpp\
        ../stk-4.4.4/src/Echo.cpp\
        ../stk-4.4.4/src/Envelope.cpp\
        ../stk-4.4.4/src/FileLoop.cpp\
        ../stk-4.4.4/src/FileRead.cpp\
        ../stk-4.4.4/src/FileWrite.cpp\
        ../stk-4.4.4/src/FileWvIn.cpp\
        ../stk-4.4.4/src/FileWvOut.cpp\
        ../stk-4.4.4/src/Fir.cpp\
        ../stk-4.4.4/src/Flute.cpp\
        ../stk-4.4.4/src/FM.cpp\
        ../stk-4.4.4/src/FMVoices.cpp\
        ../stk-4.4.4/src/FormSwep.cpp\
        ../stk-4.4.4/src/FreeVerb.cpp\
        ../stk-4.4.4/src/Granulate.cpp\
        ../stk-4.4.4/src/Guitar.cpp\
        ../stk-4.4.4/src/HevyMetl.cpp\
        ../stk-4.4.4/src/Iir.cpp\
        ../stk-4.4.4/src/InetWvIn.cpp\
        ../stk-4.4.4/src/InetWvOut.cpp\
        ../stk-4.4.4/src/JCRev.cpp\
        ../stk-4.4.4/src/LentPitShift.cpp\
        ../stk-4.4.4/src/Mandolin.cpp\
        ../stk-4.4.4/src/Mesh2D.cpp\
        ../stk-4.4.4/src/Messager.cpp\
        ../stk-4.4.4/src/MidiFileIn.cpp\
        ../stk-4.4.4/src/Modal.cpp\
        ../stk-4.4.4/src/ModalBar.cpp\
        ../stk-4.4.4/src/Modulate.cpp\
        ../stk-4.4.4/src/Moog.cpp\
        ../stk-4.4.4/src/Mutex.cpp\
        ../stk-4.4.4/src/Noise.cpp\
        ../stk-4.4.4/src/NRev.cpp\
        ../stk-4.4.4/src/OnePole.cpp\
        ../stk-4.4.4/src/OneZero.cpp\
        ../stk-4.4.4/src/PercFlut.cpp\
        ../stk-4.4.4/src/Phonemes.cpp\
        ../stk-4.4.4/src/PitShift.cpp\
        ../stk-4.4.4/src/Plucked.cpp\
        ../stk-4.4.4/src/PoleZero.cpp\
        ../stk-4.4.4/src/PRCRev.cpp\
        ../stk-4.4.4/src/Resonate.cpp\
        ../stk-4.4.4/src/Rhodey.cpp\
        ../stk-4.4.4/src/RtAudio.cpp\
        ../stk-4.4.4/src/RtMidi.cpp\
        ../stk-4.4.4/src/RtWvIn.cpp\
        ../stk-4.4.4/src/RtWvOut.cpp\
        ../stk-4.4.4/src/Sampler.cpp\
        ../stk-4.4.4/src/Saxofony.cpp\
        ../stk-4.4.4/src/Shakers.cpp\
        ../stk-4.4.4/src/Simple.cpp\
        ../stk-4.4.4/src/SineWave.cpp\
        ../stk-4.4.4/src/SingWave.cpp\
        ../stk-4.4.4/src/Sitar.cpp\
        ../stk-4.4.4/src/Skini.cpp\
        ../stk-4.4.4/src/Socket.cpp\
        ../stk-4.4.4/src/Sphere.cpp\
        ../stk-4.4.4/src/StifKarp.cpp\
        ../stk-4.4.4/src/Stk.cpp\
        ../stk-4.4.4/src/TapDelay.cpp\
        ../stk-4.4.4/src/TcpClient.cpp\
        ../stk-4.4.4/src/TcpServer.cpp\
        ../stk-4.4.4/src/Thread.cpp\
        ../stk-4.4.4/src/TubeBell.cpp\
        ../stk-4.4.4/src/Twang.cpp\
        ../stk-4.4.4/src/TwoPole.cpp\
        ../stk-4.4.4/src/TwoZero.cpp\
        ../stk-4.4.4/src/UdpSocket.cpp\
        ../stk-4.4.4/src/Voicer.cpp\
        ../stk-4.4.4/src/VoicForm.cpp\
        ../stk-4.4.4/src/Whistle.cpp\
        ../stk-4.4.4/src/Wurley.cpp\

HEADERS  += mainwindow.h\
            ../stk-4.4.4/src/include/asio.h\
            ../stk-4.4.4/src/include/asiodrivers.h\
            ../stk-4.4.4/src/include/asiodrvr.h\
            ../stk-4.4.4/src/include/asiolist.h\
            ../stk-4.4.4/src/include/asiosys.h\
            ../stk-4.4.4/src/include/dsound.h\
            ../stk-4.4.4/src/include/ginclude.h\
            ../stk-4.4.4/src/include/iasiodrv.h\
            ../stk-4.4.4/src/include/iasiothiscallresolver.h\
            ../stk-4.4.4/src/include/soundcard.h\
            ../stk-4.4.4/include/ADSR.h\
            ../stk-4.4.4/include/Asymp.h\
            ../stk-4.4.4/include/BandedWG.h\
            ../stk-4.4.4/include/BeeThree.h\
            ../stk-4.4.4/include/BiQuad.h\
            ../stk-4.4.4/include/Blit.h\
            ../stk-4.4.4/include/BlitSaw.h\
            ../stk-4.4.4/include/BlitSquare.h\
            ../stk-4.4.4/include/BlowBotl.h\
            ../stk-4.4.4/include/BlowHole.h\
            ../stk-4.4.4/include/Bowed.h\
            ../stk-4.4.4/include/BowTable.h\
            ../stk-4.4.4/include/Brass.h\
            ../stk-4.4.4/include/Chorus.h\
            ../stk-4.4.4/include/Clarinet.h\
            ../stk-4.4.4/include/Cubic.h\
            ../stk-4.4.4/include/Delay.h\
            ../stk-4.4.4/include/DelayA.h\
            ../stk-4.4.4/include/DelayL.h\
            ../stk-4.4.4/include/Drummer.h\
            ../stk-4.4.4/include/Echo.h\
            ../stk-4.4.4/include/Effect.h\
            ../stk-4.4.4/include/Envelope.h\
            ../stk-4.4.4/include/FileLoop.h\
            ../stk-4.4.4/include/FileRead.h\
            ../stk-4.4.4/include/FileWrite.h\
            ../stk-4.4.4/include/FileWvIn.h\
            ../stk-4.4.4/include/FileWvOut.h\
            ../stk-4.4.4/include/Filter.h\
            ../stk-4.4.4/include/Fir.h\
            ../stk-4.4.4/include/Flute.h\
            ../stk-4.4.4/include/FM.h\
            ../stk-4.4.4/include/FMVoices.h\
            ../stk-4.4.4/include/FormSwep.h\
            ../stk-4.4.4/include/FreeVerb.h\
            ../stk-4.4.4/include/Function.h\
            ../stk-4.4.4/include/Generator.h\
            ../stk-4.4.4/include/Granulate.h\
            ../stk-4.4.4/include/Guitar.h\
            ../stk-4.4.4/include/HevyMetl.h\
            ../stk-4.4.4/include/Iir.h\
            ../stk-4.4.4/include/InetWvIn.h\
            ../stk-4.4.4/include/InetWvOut.h\
            ../stk-4.4.4/include/Instrmnt.h\
            ../stk-4.4.4/include/JCRev.h\
            ../stk-4.4.4/include/JetTable.h\
            ../stk-4.4.4/include/LentPitShift.h\
            ../stk-4.4.4/include/Mandolin.h\
            ../stk-4.4.4/include/Mesh2D.h\
            ../stk-4.4.4/include/Messager.h\
            ../stk-4.4.4/include/MidiFileIn.h\
            ../stk-4.4.4/include/Modal.h\
            ../stk-4.4.4/include/ModalBar.h\
            ../stk-4.4.4/include/Modulate.h\
            ../stk-4.4.4/include/Moog.h\
            ../stk-4.4.4/include/Mutex.h\
            ../stk-4.4.4/include/Noise.h\
            ../stk-4.4.4/include/NRev.h\
            ../stk-4.4.4/include/OnePole.h\
            ../stk-4.4.4/include/OneZero.h\
            ../stk-4.4.4/include/PercFlut.h\
            ../stk-4.4.4/include/Phonemes.h\
            ../stk-4.4.4/include/PitShift.h\
            ../stk-4.4.4/include/Plucked.h\
            ../stk-4.4.4/include/PoleZero.h\
            ../stk-4.4.4/include/PRCRev.h\
            ../stk-4.4.4/include/ReedTable.h\
            ../stk-4.4.4/include/Resonate.h\
            ../stk-4.4.4/include/Rhodey.h\
            ../stk-4.4.4/include/RtAudio.h\
            ../stk-4.4.4/include/RtError.h\
            ../stk-4.4.4/include/RtMidi.h\
            ../stk-4.4.4/include/RtWvIn.h\
            ../stk-4.4.4/include/RtWvOut.h\
            ../stk-4.4.4/include/Sampler.h\
            ../stk-4.4.4/include/Saxofony.h\
            ../stk-4.4.4/include/Shakers.h\
            ../stk-4.4.4/include/Simple.h\
            ../stk-4.4.4/include/SineWave.h\
            ../stk-4.4.4/include/SingWave.h\
            ../stk-4.4.4/include/Sitar.h\
            ../stk-4.4.4/include/Skini.h\
            ../stk-4.4.4/include/SKINI.msg\
            ../stk-4.4.4/include/SKINI.tbl\
            ../stk-4.4.4/include/Socket.h\
            ../stk-4.4.4/include/Sphere.h\
            ../stk-4.4.4/include/StifKarp.h\
            ../stk-4.4.4/include/Stk.h\
            ../stk-4.4.4/include/TapDelay.h\
            ../stk-4.4.4/include/TcpClient.h\
            ../stk-4.4.4/include/TcpServer.h\
            ../stk-4.4.4/include/Thread.h\
            ../stk-4.4.4/include/TubeBell.h\
            ../stk-4.4.4/include/Twang.h\
            ../stk-4.4.4/include/TwoPole.h\
            ../stk-4.4.4/include/TwoZero.h\
            ../stk-4.4.4/include/UdpSocket.h\
            ../stk-4.4.4/include/Vector3D.h\
            ../stk-4.4.4/include/Voicer.h\
            ../stk-4.4.4/include/VoicForm.h\
            ../stk-4.4.4/include/Whistle.h\
            ../stk-4.4.4/include/Wurley.h\
            ../stk-4.4.4/include/WvIn.h\
            ../stk-4.4.4/include/WvOut.h

FORMS    += mainwindow.ui
