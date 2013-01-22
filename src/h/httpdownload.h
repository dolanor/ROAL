#ifndef HTTPDOWNLOAD_H
#define HTTPDOWNLOAD_H

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
 * \brief       The http download part, used for downloading game files over http.
 *
 * \file    	httpdownload.h
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
/*    C/C++ includes                                                          */
/*                                                                            */
/******************************************************************************/

/******************************************************************************/
/*                                                                            */
/*    Qt includes                                                             */
/*                                                                            */
/******************************************************************************/
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QSslConfiguration>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
#include <QtNetwork/QSslError>
#include <QFile>
#include <QStringList>
#include <QDir>

/******************************************************************************/
/*                                                                            */
/*    Others includes                                                         */
/*                                                                            */
/******************************************************************************/
#include "filevalidationthread.h"

/**
 * \brief Class for the torrent stuff
 */
class HttpDownload : public QObject
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
         *
         * \param[in] *_settings Pointer to the settings
         *
         */
        HttpDownload(QString _path);

        /**
         * \brief Deconstuctor
        */
        ~HttpDownload();

        int progress();

        double networkSpeed();

        qint64 getFullDownloadSize();
        qint64 getCurrentDownloadSize();

        QString getStatus();

        int getFilesLeft();

    private:

        /******************************************************************************/
        /*                                                                            */
        /*    Members                                                                 */
        /*                                                                            */
        /******************************************************************************/

        /**
         * \brief Network manager for downloading files
         */
        QNetworkAccessManager manager;

        /**
         * \brief SSL config for custom CAs
         */
        QSslConfiguration sslConfig;

        /**
         * \brief Custom CAs
         */
        QList<QSslCertificate> certificates;

        /**
         * \brief Request for downloading
         */
        QNetworkRequest request;

        /**
         * \brief Installation path
         */
        QString installationPath;

        /**
         * \brief File list to download
         */
        QStringList fileList;

        /**
         * \brief MD5 of files for verifying
         */
        QStringList fileListMD5;

        /**
         * \brief Sizes of files for verifying
         */
        QStringList fileListSize;

        /**
         * \brief MD5 of files for verifying
         */
        //QStringList fileListSize;

        /**
         * \brief Current download object used for speed measuring
         */
        QNetworkReply *currentDownload;

        QTime downloadTime;

        QString status;

        FileValidationThread *thread;

        /**
         * \brief Current download phase (0 init, 1 file download)
         */
        int downloadPhase;

        /**
         * \brief Files left to download
         */
        int filesLeft;

        /**
         * \brief File size already downloaded
         */
        qint64 totalSizeDownload;

        /**
         * \brief File size already downloaded
         */
        qint64 totalSizeDownloaded;

        /**
         * \brief File size downloaded for the current one
         */
        qint64 totalSizeDownloadedCurrent;

        double speed;

        /******************************************************************************/
        /*                                                                            */
        /*    Methods                                                                 */
        /*                                                                            */
        /******************************************************************************/

        /**
         * \brief Get file list from remote server
         */
        void getRemoteFileList();

        /**
         * \brief Prepare the download
         */
        void prepareDownload();

        /**
         * \brief Download the next file in queue
         */
        void getNextFile();

        /**
         * \brief Check file with md5
         */
        void checkFilesWithHash();

    private slots:

        /**
         * \brief Saves the file when download finished
         * \param reply The data of the downloaded file
         */
        void slot_downloadFinished(QNetworkReply *reply);

        void downloadProgress(qint64 bytesReceived, qint64 bytesTotal);

        void slot_verificationDone();

        /**
         * \brief Checks for SSL errors
         * \param reply The reply
         * \param errors Error list
         */
        void slot_getSSLError(QNetworkReply* reply, const QList<QSslError> &errors);
};


#endif // HTTPDOWNLOAD_H
