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
 * \file    	torrentclient.cpp
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

/******************************************************************************/
/*                                                                            */
/*    Others includes                                                         */
/*                                                                            */
/******************************************************************************/
#include "../h/torrentclient.h"


// Set namespace to libtorrent
using namespace libtorrent;

/******************************************************************************/
/*                                                                            */
/*    Constructor/Deconstructor                                               */
/*                                                                            */
/******************************************************************************/

TorrentClient::TorrentClient(Settings *_settings)
{  
    // Add settings
    settings = _settings;

    // Error code
    error_code ec;

    sesSettings.download_rate_limit = settings->getSetting("torDLMax").toInt() * 1024;
    sesSettings.upload_rate_limit = settings->getSetting("torULMax").toInt() * 1024;
    sesSettings.connections_limit = 250; // We let this hardcoded

    // Create session and set settings
    ses = new session();
    ses->set_settings(sesSettings);

    ses->listen_on(std::make_pair(settings->getSetting("torPort").toInt(), settings->getSetting("torPort").toInt() + 8 ), ec);

    // Check for error
    if (ec)
    {
        QMessageBox::critical(0, QObject::tr("Error on setting the port"), QString::fromStdString(ec.message()));
    }

    /// \todo Add upnp and natpmp
    add_torrent_params p;

    // Get the path
    QString path = settings->getSetting("installLocation");

    // Set path
    p.save_path = QString(path + "game/").toUtf8().constData();
    path = path + "launcher/downloads/roa.torrent";

    // Start and check for error code
    p.ti = new torrent_info(path.toUtf8().constData());

    // Check for error
    if (ec)
    {
        QMessageBox::critical(0, QObject::tr("Error on preparing torrent"), QString::fromStdString(ec.message()));
    }

    torrentHandler = ses->add_torrent(p, ec);

    // Check for errors
    if (ec)
    {
        QMessageBox::critical(0, QObject::tr("Error on adding torrent"), QString::fromStdString(ec.message()));
    }

    torrentStatus = torrentHandler.status();
}

TorrentClient::~TorrentClient()
{
    torrentHandler.save_resume_data();
    //ses.save_state();
    delete ses;
}

/******************************************************************************/
/*                                                                            */
/*    Public methods                                                          */
/*                                                                            */
/******************************************************************************/

int TorrentClient::getDownloadRate()
{
    updateStats();
    return torrentStatus.download_rate/1024;
}

int TorrentClient::getUploadRate()
{
    updateStats();
    return torrentStatus.upload_rate/1024;
}

int TorrentClient::getProgress()
{
    updateStats();

    int value = float(torrentStatus.total_wanted_done + 1) / float( torrentStatus.total_wanted ) * 100;

    return value;
}

void TorrentClient::toggleStatus()
{
    if(ses->is_paused())
    {
        ses->resume();
    }
    else
    {
        ses->pause();
    }
}

void TorrentClient::applySettings()
{
    // Error code
    error_code ec;

    session_settings sesSettingsTemp = ses->settings();

    sesSettingsTemp.download_rate_limit = settings->getSetting("torDLMax").toInt() * 1024;
    sesSettingsTemp.upload_rate_limit = settings->getSetting("torULMax").toInt() * 1024;
    sesSettingsTemp.connections_limit = settings->getSetting("torConMax").toInt();

    ses->listen_on(std::make_pair(settings->getSetting("torPort").toInt(), settings->getSetting("torPort").toInt() + 8 ), ec);

    // Check for error
    if (ec)
    {
        QMessageBox::critical(0, QObject::tr("Error on setting the port"), QString::fromStdString(ec.message()));
    }

    ses->set_settings(sesSettingsTemp);
}

int TorrentClient::getSize()
{
    return torrentStatus.total_wanted / 1024 / 1024;
}

int TorrentClient::getSizeDownloaded()
{
    return torrentStatus.total_wanted_done / 1024 / 1024;
}

QString TorrentClient::getStatus()
{
    switch( torrentStatus.state )
    {
        case 0:
            return QObject::tr("Prepare");
        case 1:
            return QObject::tr("Checking files");
        case 2:
            return QObject::tr("Connecting");
        case 3:
            return QObject::tr("Downloading");
        case 4:
            return QObject::tr("Finished");
        case 5:
            return QObject::tr("Seeding");
        case 6:
            return QObject::tr("Allocating");
        case 7:
            return QObject::tr("Checking files");
        default:
            return QObject::tr("Bug found, please report!");
    }
}

/******************************************************************************/
/*                                                                            */
/*    Private methods                                                         */
/*                                                                            */
/******************************************************************************/

void TorrentClient::updateStats()
{
    status = ses->status();
    torrentStatus = torrentHandler.status();
}
