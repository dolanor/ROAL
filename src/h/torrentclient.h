/**
 * \copyright
 *              Copyright © 2011 Manuel Gysin
 *              Copyright © 2012 QuantumBytes inc.
 *
 *              For more information, see https://www.quantum-bytes.com/
 *
 * \section LICENSE
 *
 *              This file is part of Relics of Annorath Launcher.
 *
 *              Relics of Annorath Launcher is free software: you can redistribute it and/or modify
 *              it under the terms of the GNU General Public License as published by
 *              the Free Software Foundation, either version 3 of the License, or
 *              any later version.
 *
 *              Relics of Annorath Launcher is distributed in the hope that it will be useful,
 *              but WITHOUT ANY WARRANTY; without even the implied warranty of
 *              MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *              GNU General Public License for more details.
 *
 *              You should have received a copy of the GNU General Public License
 *              along with Relics of Annorath Launcher.  If not, see <http://www.gnu.org/licenses/>.
 *
 * \brief       The bit torrent protocol implemenation for downloading the needed application data.
 *
 * \file    	torrentclient.h
 *
 * \note
 *
 * \version 	2.0
 *
 * \author  	Manuel Gysin <manuel.gysin@quantum-bytes.com>
 *
 * \date        2011/04/10 19:00:00 GMT+1
 *              2012/11/30 23:10:00 GMT+1
 *
 */

#ifndef TORRENTCLIENT_H
#define TORRENTCLIENT_H

/******************************************************************************/
/*                                                                            */
/*    C/C++ includes                                                          */
/*                                                                            */
/******************************************************************************/
#include <iostream>
#include <fstream>
#include <iterator>
#include <exception>

/******************************************************************************/
/*                                                                            */
/*    Qt includes                                                             */
/*                                                                            */
/******************************************************************************/
#include <QString>
#include <QMessageBox>

/******************************************************************************/
/*                                                                            */
/*    Libtorrent-rasterbar includes                                           */
/*                                                                            */
/******************************************************************************/
//#define BOOST_ALL_DYN_LINK
#define BOOST_ASIO_DYN_LINK

#ifndef Q_MOC_RUN
#include "libtorrent/entry.hpp"
#include "libtorrent/bencode.hpp"
#include "libtorrent/session.hpp"
#endif // Q_MOC_RUN

/******************************************************************************/
/*                                                                            */
/*    Others includes                                                         */
/*                                                                            */
/******************************************************************************/
#include "settings.h"


// Set namespace to libtorrent
using namespace libtorrent;


/**
 * \brief Class for the torrent stuff
 */
class TorrentClient
{

    public:

        /******************************************************************************/
        /*                                                                            */
        /*    Members                                                                 */
        /*                                                                            */
        /******************************************************************************/

        /******************************************************************************/
        /*                                                                            */
        /*    Methods                                                                 */
        /*                                                                            */
        /******************************************************************************/

        /**
         * \brief Constuctor
         *
         * \param[in] *_settings Pointer to the settings
         *
         */
        TorrentClient(Settings *_settings);

        /**
         * \brief Deconstuctor
        */
        ~TorrentClient();

        /**
         * \brief Get the current download rate
         *
         * \todo Remove overhead download rate
         *
         * \return The current download rate
         */
        int getDownloadRate();

        /**
         * \brief Get the current upload rate
         *
         * \todo Remove overhead upload rate
         *
         * \return The current upload rate
         */
        int getUploadRate();

        /**
         * \brief Get the current progress in percent
         *
         * \return The current progress in percent
         */
        int getProgress();

        /**
         * \brief Get the download size in MB
         *
         * \return The download size in MB
         */
        int getSize();

        /**
         * \brief Get the downloaded size in MB
         *
         * \return The downloaded size in MB
         */
        int getSizeDownloaded();

        /**
         * \brief Get current torrent status
         *
         * \return The torrent status
         */
        QString getStatus();

        /**
         * \brief Toggle torrent status (active/pause)
         */
        void toggleStatus();

        /**
         * \brief Apply torrent settings (needs no restart)
         */
        void applySettings();

    private:

        /******************************************************************************/
        /*                                                                            */
        /*    Members                                                                 */
        /*                                                                            */
        /******************************************************************************/

        /**
         * \brief Provides all handles about a torrent
         */
        torrent_handle torrentHandler;

        /**
         * \brief Provides all information about a torrent
         */
        torrent_status torrentStatus;

        /**
         * \brief Provides all information about the session
         */
        session_status status;

        /**
         * \brief Handles everything releated to the torrent session
         */
        session *ses;

        /**
         * \brief The Qt settings object for torrent settings
         */
        Settings *settings;

        /**
         * \brief Sessions settings
         */
        session_settings sesSettings;

        /******************************************************************************/
        /*                                                                            */
        /*    Methods                                                                 */
        /*                                                                            */
        /******************************************************************************/

        /**
         * \brief Update torrent status
         */
        void updateStats();
};

#endif // TORRENTCLIENT_H
