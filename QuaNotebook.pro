#-------------------------------------------------
#
# Project created by QtCreator 2015-07-01T08:03:28
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets webkitwidgets

TARGET = IBFNotebook
TEMPLATE = app

# You'll need to define this one manually if using a build system other
# than qmake or using QuaZIP sources directly in your project.
CONFIG(staticlib): DEFINES += QUAZIP_STATIC
unix:LIBS += -L/lib/x86_64-linux-gnu/ -lz

SOURCES += main.cpp\
    IBFWebView.cpp \
    IBFTreeView.cpp \
    QuaMainWindow.cpp \
    QuaTextEdit.cpp \
    QuaPreferenceWindow.cpp \
    QuaZIP/JlCompress.cpp \
    QuaZIP/qioapi.cpp \
    QuaZIP/quaadler32.cpp \
    QuaZIP/quacrc32.cpp \
    QuaZIP/quagzipfile.cpp \
    QuaZIP/quaziodevice.cpp \
    QuaZIP/quazip.cpp \
    QuaZIP/quazipdir.cpp \
    QuaZIP/quazipfile.cpp \
    QuaZIP/quazipfileinfo.cpp \
    QuaZIP/quazipnewinfo.cpp \
    QuaZIP/unzip.c \
    QuaZIP/zip.c

HEADERS  += \
    IBFWebView.h \
    IBFTreeView.h \
    QuaMainWindow.h \
    QuaTextEdit.h \
    QuaPreferenceWindow.h \
    QuaZIP/crypt.h \
    QuaZIP/ioapi.h \
    QuaZIP/JlCompress.h \
    QuaZIP/quaadler32.h \
    QuaZIP/quachecksum32.h \
    QuaZIP/quacrc32.h \
    QuaZIP/quagzipfile.h \
    QuaZIP/quaziodevice.h \
    QuaZIP/quazip.h \
    QuaZIP/quazip_global.h \
    QuaZIP/quazipdir.h \
    QuaZIP/quazipfile.h \
    QuaZIP/quazipfileinfo.h \
    QuaZIP/quazipnewinfo.h \
    QuaZIP/unzip.h \
    QuaZIP/zip.h

FORMS +=

RESOURCES += \
    IBFResource.qrc

OTHER_FILES +=
