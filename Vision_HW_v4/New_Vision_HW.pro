#-------------------------------------------------
#
# Project created by QtCreator 2019-01-22T16:16:45
#
#-------------------------------------------------

QT       += core gui serialport network sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Vision
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


CONFIG += c++11

SOURCES += \
        main.cpp \
        mainwindow.cpp \
    led_control.cpp \
    play.cpp \
    save_thread.cpp \
    stage_change_thread.cpp \
    setting_form.cpp \
    vision_check.cpp \
    clientStuff.cpp

HEADERS += \
        mainwindow.h \
    led_control.h \
    play.h \
    save_thread.h \
    stage_change_thread.h \
    setting_form.h \
    vision_check.h \
    clientStuff.h

FORMS += \
        mainwindow.ui \
    led_control.ui \
    play.ui \
    setting_form.ui \
    setting_form.ui \
    vision_check.ui

INCLUDEPATH += C:\opencv\debug4\install\include

LIBS += C:\opencv\debug4\bin\libopencv_core345.dll
LIBS += C:\opencv\debug4\bin\libopencv_highgui345.dll
LIBS += C:\opencv\debug4\bin\libopencv_imgcodecs345.dll
LIBS += C:\opencv\debug4\bin\libopencv_imgproc345.dll
LIBS += C:\opencv\debug4\bin\libopencv_features2d345.dll
LIBS += C:\opencv\debug4\bin\libopencv_calib3d345.dll
LIBS += C:\opencv\debug4\bin\libopencv_video345.dll
LIBS += C:\opencv\debug4\bin\libopencv_videoio345.dll
LIBS += C:\opencv\debug4\bin\libopencv_videostab345.dll
LIBS += C:\opencv\debug4\bin\libopencv_photo345.dll
LIBS += C:\opencv\debug4\bin\libopencv_objdetect345.dll
LIBS += C:\opencv\debug4\bin\libopencv_calib3d345.dll
LIBS += C:\opencv\debug4\bin\libopencv_dnn345.dll
LIBS += C:\opencv\debug4\bin\libopencv_objdetect345.dll
LIBS += C:\opencv\debug4\bin\libopencv_flann345.dll
LIBS += C:\opencv\debug4\bin\libopencv_ml345.dll
LIBS += C:\opencv\debug4\bin\libopencv_shape345.dll


# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
