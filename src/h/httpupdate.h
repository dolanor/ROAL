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
 * \brief       Handels the update process for the launcher and related files
 *
 * \file    	httpupdate.h
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

#ifndef HTTPUPDATE_H
#define HTTPUPDATE_H

/******************************************************************************/
/*                                                                            */
/*    C/C++ includes                                                          */
/*                                                                            */
/******************************************************************************/
#include <iostream>



/******************************************************************************/
/*                                                                            */
/*    Qt includes                                                             */
/*                                                                            */
/******************************************************************************/
#include <QWidget>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QFile>
#include <QTimer>
#include <QProcess>
#include <QDir>
#include <QSslError>
#include <QTranslator>
#include <QObject>
#include <QSslConfiguration>

/******************************************************************************/
/*                                                                            */
/*    Others includes                                                         */
/*                                                                            */
/******************************************************************************/
#include "settings.h"

#ifdef Q_OS_WIN
#include <Windows.h>
#endif

/**
 * \addtogroup Ui
 */
namespace Ui {
    class HttpUpdate;
}

/**
 * \brief Check for new version and get needed update files.
 */
class HttpUpdate : public QWidget
{
        Q_OBJECT

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
         * \param _settings Settings object
         * \param parent Parent
         */
        explicit HttpUpdate(Settings *_settings, QWidget *parent = 0);

        /**
         * \brief Deconstuctor
         */
        ~HttpUpdate();

    private:

        /******************************************************************************/
        /*                                                                            */
        /*    Members                                                                 */
        /*                                                                            */
        /******************************************************************************/

        /**
         * \brief The UI-File
         */
        Ui::HttpUpdate *ui;

        /**
         * \brief Manages the session for network communication.
         */
        QNetworkAccessManager manager;

        /**
         * \brief Settings object
         */
        Settings *settings;

        /**
         * \brief String with launcher version on server
         */
        QString remoteVersion;

        /**
         * \brief A simply timer used for delay the checking
         */
        QTimer timer;

        /**
         * \brief Process for updating the launcher
         */
        QProcess installer;

        /**
         * \brief SSL configuration to add custom CAs
         */
        QSslConfiguration sslConfig;

        /**
         * \brief SSL certificates for custom CAs
         */
        QList<QSslCertificate> certificates;

        /**
         * \brief Network request manager for downloading
         */
        QNetworkRequest request;

        /******************************************************************************/
        /*                                                                            */
        /*    Methods                                                                 */
        /*                                                                            */
        /******************************************************************************/

        /**
         * \brief Prepares the network request for downloading the installer binary
         */
        void downloadInstaller();

        /**
         * \brief Prepares the network request for downloading the torrent file
         */
        void downloadTorrent();

    private slots:

        /**
         * \brief Check the version remotly
         * \param reply Reply with data
         */
        void slot_checkVersion(QNetworkReply* reply);

        /**
         * \brief Saves and starts the installer
         * \param reply Reply with data
         */
        void slot_startInstaller(QNetworkReply* reply);

        /**
         * \brief Saves the torrent to disk
         * \param reply Reply with data
         */
        void slot_saveTorrent(QNetworkReply* reply);

        /**
         * \brief Checks for SSL errors when requesting data from remote server
         * \param reply Reply with data
         * \param erros List with SSL errors
         */
        void slot_getSSLError(QNetworkReply* reply, const QList<QSslError> &errors);

        /**
         * \brief Starts the check process called by timer
         */
        void slot_startCheck();

        void slot_installerFinished();

    signals:

        /**
         * \brief Signal for the outside about the state of the update
         * \param updateStatus Was the update successfully or not
         * \param updateState The application state if restart is needed or not
         */
        void updateChecked(bool updateStatus, bool updateState);

};

#endif // HTTPUPDATE_H
