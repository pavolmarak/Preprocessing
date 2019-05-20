QT       += gui
Debug:TARGET = Preprocessingd
Release:TARGET = Preprocessing
TEMPLATE = lib

DEFINES += PREPROCESSING_LIBRARY
DEFINES += QT_DEPRECATED_WARNINGS
DEFINES += PRO_PWD=\\\"$$_PRO_FILE_PWD_\\\"

QMAKE_CFLAGS_ISYSTEM=

SOURCES += preprocessing.cpp \
    orientationmap.cpp \
    thinning.cpp \
    binarization.cpp \
    gaborfiltermultithread.cpp \
    gaborthread.cpp \
    contrastenhancement.cpp \
    frequencymap.cpp \
    mask.cpp \
    qualitymap.cpp \
    gaborfiltergpu.cpp \
    preprocessing_caffenetwork.cpp \
    contrastbatch.cpp \
    maskbatch.cpp \
    binarizationbatch.cpp \
    thinningmultithread.cpp

HEADERS += preprocessing.h\
        preprocessing_global.h \
    helper.h \
    orientationmap.h \
    thinning.h \
    binarization.h \
    gaborfiltermultithread.h \
    gaborthread.h \
    contrastenhancement.h \
    frequencymap.h \
    mask.h \
    imagecontour.h \
    qualitymap.h \
    gaborfiltergpu.h \
    preprocessing_config.h \
    preprocessing_caffenetwork.h \
    contrastbatch.h \
    maskbatch.h \
    binarizationbatch.h \
    thinningmultithread.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

#CUDA
unix:!macx: LIBS += -L/opt/cuda/lib/ -lcudart
INCLUDEPATH += /opt/cuda/include
DEPENDPATH += /opt/cuda/include

#ArrayFire
unix:!macx: LIBS += -L/usr/lib/ -lafcuda
INCLUDEPATH += /usr/include
DEPENDPATH += /usr/include

#OpenCV
unix:!macx: LIBS += -L/usr/lib/ -lopencv_core -lopencv_highgui -lopencv_imgcodecs -lopencv_imgproc
INCLUDEPATH += /usr/include/opencv4

#caffe
unix:!macx: LIBS += -L$$PWD/../../../../usr/lib/ -lcaffe
INCLUDEPATH += $$PWD/../../../../usr/include/caffe
DEPENDPATH += $$PWD/../../../../usr/include/caffe
