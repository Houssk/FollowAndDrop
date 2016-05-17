#-------------------------------------------------
#
# Project created by QtCreator 2015-05-24T20:04:07
#
#-------------------------------------------------

QT       += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = FollowAndDrop
TEMPLATE = app


SOURCES += \
    chronometre.cpp \
    main.cpp \
    myglwidget.cpp \
    score.cpp \
    view.cpp \
    webcamwindow.cpp

HEADERS  += \
    chronometre.h \
    myglwidget.h \
    score.h \
    view.h \
    webcamwindow.h

INCLUDEPATH +=$$(OPENCV_DIR)\include

LIBS += -L$$(OPENCV_DIR)\lib \
    -lopencv_core2410 \
    -lopencv_highgui2410 \
    -lopencv_imgproc2410 \
    -lopencv_video2410 \
    -lopencv_features2d2410 \
    -lopencv_calib3d2410 \
    -lopencv_objdetect2410

RESOURCES += \
    src.qrc
