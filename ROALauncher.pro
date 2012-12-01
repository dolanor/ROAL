#-------------------------------------------------
#
# Project created by QtCreator 2011-04-02T20:27:08
#
#-------------------------------------------------

# Needed qt modules
QT += core gui network multimedia webkitwidgets

# Target and projekt template
TARGET = ROALauncher
TEMPLATE = app

# Additional module for one-app-at-one-time
include(src/external/qtsingleapplication.pri)

# Configure libboost
QMAKE_CFLAGS += -DBOOST_FILESYSTEM_VERSION=2
QMAKE_CXXFLAGS += -DBOOST_FILESYSTEM_VERSION=2

# Windows specified stuff
win32 {
    INCLUDEPATH += "C:/Users/developer/Desktop/02_sources/05_libtorrent-rasterbar/zlib"
    INCLUDEPATH += "C:/Users/developer/Desktop/02_sources/05_libtorrent-rasterbar/include"
    INCLUDEPATH += "C:/Users/developer/Desktop/02_sources/04_boost"
    INCLUDEPATH += "C:/Users/developer/Desktop/02_sources/03_qt5/qtwebkit/include"
    CONFIG += embed_manifest_exe
}

# Special libs for windows build
win32 {
#    win32-msvc*:contains(QMAKE_TARGET.arch, x86_64):{
        LIBS += "C:/Program Files/Microsoft SDKs/Windows/v7.1/Lib/x64/shell32.lib"
        LIBS += "C:/Users/developer/Desktop/03_dist/04_boost/lib64/system/boost_system-vc100-mt-1_49.lib"
        LIBS += "C:/Users/developer/Desktop/03_dist/04_boost/lib64/filesystem/boost_filesystem-vc100-mt-1_49.lib"
        LIBS += "C:/Users/developer/Desktop/03_dist/04_boost/lib64/thread/boost_thread-vc100-mt-1_49.lib"
        LIBS += "C:/Users/developer/Desktop/03_dist/04_boost/lib64/datetime/boost_date_time-vc100-mt-1_49.lib"
        LIBS += "C:/Users/developer/Desktop/03_dist/05_libtorrent_rasterbar/torrent.lib"
        LIBS += "C:/Users/developer/Desktop/02_sources/03_qt5/qtbase/plugins/imageformats/qgif.lib"
        LIBS += "C:/Users/developer/Desktop/02_sources/03_qt5/qtwebkit/lib/Qt5WebKitWidgets.lib"
#    }
#    else {
#    }
}

# Linux stuff
unix {
    LIBS += -lboost_filesystem -lboost_system
    CONFIG += link_pkgconfig
    PKGCONFIG += libtorrent-rasterbar
}

# Source files
SOURCES += src/cpp/torrentclient.cpp \
    src/cpp/settings.cpp \
    src/cpp/mainwindow.cpp \
    src/cpp/main.cpp \
    src/cpp/httpupdate.cpp

# Header files
HEADERS  += src/h/torrentclient.h \
    src/h/settings.h \
    src/h/mainwindow.h \
    src/h/httpupdate.h

# Forms
FORMS    += src/ui/mainwindow.ui \
    src/ui/httpupdate.ui

# Ressources
RESOURCES += ressources/res.qrc

# Translations
TRANSLATIONS = roa_en.ts \
               roa_ger.ts
