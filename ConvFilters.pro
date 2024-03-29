#-------------------------------------------------
#
# Project created by QtCreator 2019-06-29T11:52:43
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ConvFilters
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
        FilterImage.cpp \
        FloatImage.cpp \
        main.cpp \
        convfilter.cpp

HEADERS += \
        FilterImage.h \
        FloatImage.h \
        convfilter.h

FORMS += \
        convfilter.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    ExampleFilters/avgfilter.txt \
    ExampleFilters/sobel_H.txt \
    ExampleFilters/sobel_V.txt \
    ExampleFilters/verticalsobel.txt \
    ExamplePhotos/addams-family.pgm \
    ExamplePhotos/liver1.pgm \
    ExamplePhotos/noise_blur_10.pgm \
    ExamplePhotos/vessel.pgm
