QT       += gui
TEMPLATE = lib

TARGET = ofpreproc
DEFINES += PREPROCESSING_LIBRARY
DEFINES += QT_DEPRECATED_WARNINGS
DEFINES += PRO_PWD=\\\"$$_PRO_FILE_PWD_\\\"

CONFIG += c++17
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
    preprocessing_caffenetwork.cpp

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
    preprocessing_caffenetwork.h

# ********************************************************************
#                           DEPENDENCIES
# ********************************************************************

CONFIG += manjaro
#CONFIG += debian

manjaro {
    # CUDA - Manjaro (as a package)
    unix:!macx: LIBS += -L/usr/local/cuda-10.2/lib64/ -lcudart
    INCLUDEPATH += /usr/local/cuda-10.2/include
    DEPENDPATH += /usr/local/cuda-10.2/include

    #ArrayFire - Manjaro (as a package)
    unix:!macx: LIBS += -L/usr/lib/ -lafcuda
    INCLUDEPATH += /usr/include
    DEPENDPATH += /usr/include

    #OpenCV - Manjaro (as a package)
    unix:!macx: LIBS += -L/usr/lib/ -lopencv_core
    unix:!macx: LIBS += -L/usr/lib/ -lopencv_imgproc
    unix:!macx: LIBS += -L/usr/lib/ -lopencv_imgcodecs
    unix:!macx: LIBS += -L/usr/lib/ -lopencv_highgui
    INCLUDEPATH += /usr/include/opencv4
    DEPENDPATH += /usr/include/opencv4

    #Caffe - Manjaro (built from source)
    unix:!macx: LIBS += -L/usr/local/lib/ -lcaffe
    INCLUDEPATH += /usr/local/include
    DEPENDPATH += /usr/local/include

    #glog, protobuf, boost - Manjaro (as a package)
    unix:!macx: LIBS += -L/usr/lib/ -lglog
    unix:!macx: LIBS += -L/usr/lib/ -lprotobuf
    unix:!macx: LIBS += -L/usr/lib/ -lboost_system
    INCLUDEPATH += /usr/include
    DEPENDPATH += /usr/include
}

debian {
    #CUDA - Debian
    unix:!macx: LIBS += -L/usr/local/cuda/lib64/ -lcudart
    INCLUDEPATH += /usr/local/cuda/include
    DEPENDPATH += /usr/local/cuda/include

    #ArrayFire - Debian (built from source)
    unix:!macx: LIBS += -L/usr/local/lib/ -lafcuda
    INCLUDEPATH += /usr/local/include
    DEPENDPATH += /usr/local/include

    #OpenCV - Debian (as a package)
    unix:!macx: LIBS += -L/usr/lib/x86_64-linux-gnu/ -lopencv_core
    unix:!macx: LIBS += -L/usr/lib/x86_64-linux-gnu/ -lopencv_imgproc
    unix:!macx: LIBS += -L/usr/lib/x86_64-linux-gnu/ -lopencv_imgcodecs
    unix:!macx: LIBS += -L/usr/lib/x86_64-linux-gnu/ -lopencv_highgui
    INCLUDEPATH += /usr/include/opencv4
    DEPENDPATH += /usr/include/opencv4

    #Caffe - Debian (built from source)
    unix:!macx: LIBS += -L/usr/local/lib/ -lcaffe
    INCLUDEPATH += /usr/local/include
    DEPENDPATH += /usr/local/include

    #glog, protobuf, boost - Debian (as a package)
    unix:!macx: LIBS += -L/usr/lib/x86_64-linux-gnu/ -lglog
    unix:!macx: LIBS += -L/usr/lib/x86_64-linux-gnu/ -lprotobuf
    unix:!macx: LIBS += -L/usr/lib/x86_64-linux-gnu/ -lboost_system
    INCLUDEPATH += /usr/include
    DEPENDPATH += /usr/include
}

