#-------------------------------------------------
#
# Project created by QtCreator 2011-04-02T20:27:08
#
#-------------------------------------------------

# Needed qt modules
QT += core gui network webkitwidgets xml

# Target and projekt template
TARGET = ROALauncher
TEMPLATE = app

# Additional module for one-app-at-one-time
include(src/external/qtsingleapplication.pri)

# Configure libboost
QMAKE_CFLAGS += -DBOOST_FILESYSTEM_VERSION=2
QMAKE_CXXFLAGS += -DBOOST_FILESYSTEM_VERSION=2

win32 {
    message( "Building for windows")

    # Add includes
    INCLUDEPATH += "D:/03_dist/03_boost/include"
    INCLUDEPATH += "D:/03_dist/04_libtorrent_rasterbar/include"
    INCLUDEPATH += "D:/03_dist/05_sfml/include"
    INCLUDEPATH += "D:/02_sources/07_libarchive/libarchive"

    win32-msvc*:contains(QMAKE_TARGET.arch, x86_64) {
        message( "Building for 64 bit")

        # Add libs
        LIBS += "C:/buildenv/w8sdk/Lib/win8/um/x64/shell32.lib"
        LIBS += "D:/03_dist/07_libarchive/libarchive/Release/archive.lib"
        LIBS += "D:/03_dist/03_boost/64/date_time/libboost_date_time-vc110-mt-1_49.lib"
        LIBS += "D:/03_dist/03_boost/64/filesystem/boost_filesystem-vc-mt-1_49.lib"
        LIBS += "D:/03_dist/03_boost/64/system/libboost_system-vc110-mt-1_49.lib"
        LIBS += "D:/03_dist/03_boost/64/thread/libboost_thread-vc110-mt-1_49.lib"
        LIBS += "D:/03_dist/04_libtorrent_rasterbar/64/torrent.lib"
        LIBS += "D:/03_dist/05_sfml/64/sfml-audio.lib"
        LIBS += "D:/03_dist/05_sfml/64/sfml-system.lib"
    } else {
        message( "Building for 32 bit")

        # Add libs
        LIBS += "C:/buildenv/w8sdk/Lib/win8/um/x86/shell32.lib"
        LIBS += "D:/03_dist/07_libarchive/libarchive/Release/archive.lib"
        LIBS += "D:/03_dist/03_boost/32/date_time/libboost_date_time-vc110-mt-1_49.lib"
        LIBS += "D:/03_dist/03_boost/32/filesystem/boost_filesystem-vc-mt-1_49.lib"
        LIBS += "D:/03_dist/03_boost/32/system/libboost_system-vc110-mt-1_49.lib"
        LIBS += "D:/03_dist/03_boost/32/thread/libboost_thread-vc110-mt-1_49.lib"
        LIBS += "D:/03_dist/04_libtorrent_rasterbar/32/torrent.lib"
        LIBS += "D:/03_dist/05_sfml/32/sfml-audio.lib"
        LIBS += "D:/03_dist/05_sfml/32/sfml-system.lib"
    }
}

# Linux stuff
unix {
    INCLUDEPATH += "/home/mgysin/Downloads/SFML-2.0-rc/include"
    INCLUDEPATH += "/usr/local/include/"
    LIBS += "/home/mgysin/Downloads/SFML-2.0-rc/lib/libsfml-audio.so"
    LIBS += "/home/mgysin/Downloads/SFML-2.0-rc/lib/libsfml-system.so"
    #LIBS += "/usr/local/lib/libtorrent-rasterbar.so"
    #LIBS += "/usr/local/lib/"
    LIBS += -lboost_filesystem -lboost_system -larchive
    CONFIG += link_pkgconfig
    PKGCONFIG += libtorrent-rasterbar
}

# Source files
SOURCES += src/cpp/torrentclient.cpp \
           src/cpp/settings.cpp \
           src/cpp/mainwindow.cpp \
           src/cpp/main.cpp \
           src/cpp/httpupdate.cpp \
           src/cpp/httpdownload.cpp \
           src/cpp/filevalidationthread.cpp \
           src/cpp/firstrun.cpp \
           src/cpp/customsound.cpp \
           src/cpp/aboutlauncher.cpp \
    src/cpp/filedecompression.cpp

# Header files
HEADERS  += src/h/torrentclient.h \
            src/h/settings.h \
            src/h/mainwindow.h \
            src/h/httpupdate.h \
            src/h/httpdownload.h \
            src/h/filevalidationthread.h \
            src/h/firstrun.h \
            src/h/customsound.h \
            src/h/aboutlauncher.h \
    src/h/constants.h \
    src/h/filedecompression.h

# Forms
FORMS    += src/ui/mainwindow.ui \
            src/ui/httpupdate.ui \
            src/ui/firstrun.ui \
            src/ui/aboutlauncher.ui

# Ressources
RESOURCES += resources/res.qrc

# RC
RC_FILE =       src/ROALauncher.rc

# Translations
TRANSLATIONS = resources/translations/roal_french.ts \
               resources/translations/roal_greek.ts \
               resources/translations/roal_italian.ts \
               resources/translations/roal_polish.ts \
               resources/translations/roal_portuguese.ts \
               resources/translations/roal_spanish.ts \
               resources/translations/roal_swedish.ts \
               resources/translations/roal_english.ts \
               resources/translations/roal_german.ts \
