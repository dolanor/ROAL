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
/*    Others includes                                                         */
/*                                                                            */
/******************************************************************************/
#include "../h/httpdownload.h"

/******************************************************************************/
/*                                                                            */
/*    Constructor/Deconstructor                                               */
/*                                                                            */
/******************************************************************************/

HttpDownload::HttpDownload(QString _path)
{
    installationPath = _path;
    downloadPhase = 0;
    totalSizeDownload = 0;
    totalSizeDownloadedCurrent = 0;
    totalSizeDownloaded = 0;
    speed = 0;
    status = "checking";

    //Create all dirs, maybe one is missing
    QStringList dirs;
    dirs << installationPath + "game"
         << installationPath + "game/bin"
         << installationPath + "game/data"
         << installationPath + "game/lib"
         << installationPath + "game/data/logs"
         << installationPath + "game/data/relics_of_annorath"
         << installationPath + "game/data/relics_of_annorath/configuration"
         << installationPath + "game/data/relics_of_annorath/configuration/gui"
         << installationPath + "game/data/relics_of_annorath/terrains"
         << installationPath + "game/data/relics_of_annorath/terrains/merasurien"
         << installationPath + "game/data/relics_of_annorath/terrains/merasurien/1_14_westzones"
         << installationPath + "game/data/relics_of_annorath/terrains/merasurien/1_14_westzones/1_14_westzones"
         << installationPath + "game/data/relics_of_annorath/terrains/merasurien/1_14_westzones/Layers"
         << installationPath + "game/data/relics_of_annorath/terrains/merasurien/3_14_Nordzone1"
         << installationPath + "game/data/relics_of_annorath/terrains/merasurien/3_14_Nordzone1/3_14_nordzone1"
         << installationPath + "game/data/relics_of_annorath/terrains/merasurien/3_14_Nordzone1/Terrain_Masken"
         << installationPath + "game/data/relics_of_annorath/terrains/merasurien/4_14_Nordzone2"
         << installationPath + "game/data/relics_of_annorath/terrains/merasurien/4_14_Nordzone2/4_14_nordzone2";


    for(int i = 0; i < dirs.size(); i++)
    {
        QDir dir(dirs.at(i));
        if(!dir.exists())
            QDir().mkpath(dirs.at(i));
    }

    getRemoteFileList();
}

HttpDownload::~HttpDownload()
{

}

int HttpDownload::progress()
{
    int value;

    if(status == "downloading")
        value = float(totalSizeDownloaded + totalSizeDownloadedCurrent) / float(totalSizeDownload + 0.1) * 100;
    else if(status == "done")
        value = 100;
    else
        value = 0;

    return value;
}

double HttpDownload::networkSpeed()
{
    return speed;
}

qint64 HttpDownload::getFullDownloadSize()
{
    return totalSizeDownload / 1024 / 1024;
}

qint64 HttpDownload::getCurrentDownloadSize()
{
    return (totalSizeDownloaded + totalSizeDownloadedCurrent) / 1024 / 1024;
}

QString HttpDownload::getStatus()
{
    return status;
}

int HttpDownload::getFilesLeft()
{
    return filesLeft;
}

/******************************************************************************/
/*                                                                            */
/*    Private methods                                                         */
/*                                                                            */
/******************************************************************************/

void HttpDownload::getRemoteFileList()
{
    // Prepare downloading over ssl
    certificates.append(QSslCertificate::fromPath(":/certs/class2.pem"));
    certificates.append(QSslCertificate::fromPath(":/certs/ca.pem"));

    sslConfig.defaultConfiguration();
    sslConfig.setCaCertificates(certificates);

    request.setSslConfiguration(sslConfig);

    connect(&manager, SIGNAL(finished(QNetworkReply*)),this, SLOT(slot_downloadFinished(QNetworkReply*)));
    connect(&manager, SIGNAL(sslErrors(QNetworkReply*, const QList<QSslError>&)),this, SLOT(slot_getSSLError(QNetworkReply*, const QList<QSslError>&)));

    request.setUrl(QUrl("https://launcher.annorath-game.com/data/game/game_content.txt"));

    // Start download
    manager.get(request);
}

void HttpDownload::prepareDownload()
{
    // Open file and read it
    QFile file(installationPath + "launcher/downloads/files_game.txt");

    if (file.open(QIODevice::ReadOnly))
    {
        QTextStream in(&file);

        while ( !in.atEnd() )
        {
            QStringList tmp = QString(in.readLine()).split(";");

            // Check if we got valid input
            if(tmp.size() == 3)
            {
                    // Save values
                    fileList.append(tmp.at(0));
                    fileListMD5.append(tmp.at(1));
                    fileListSize.append(tmp.at(2));
                    //totalSizeDownload += tmp.at(2).toInt();
            }
        }
    }

    checkFilesWithHash();

    // Close file
    file.close();
}

void HttpDownload::checkFilesWithHash()
{
    thread = new FileValidationThread();
    thread->setData(installationPath, &fileList, &fileListMD5, &fileListSize);

    connect(thread, SIGNAL(finished()),SLOT(slot_verificationDone()));

    thread->start();
}

void HttpDownload::getNextFile()
{
    if(filesLeft > 0)
    {
        // Set URL and start download
        request.setUrl(QUrl("https://launcher.annorath-game.com/data/" + fileList.at(filesLeft-1)));

        // Start request
        currentDownload = manager.get(request);

        // Connect reply for measuring the speed
        connect(currentDownload, SIGNAL(downloadProgress(qint64,qint64)),SLOT(downloadProgress(qint64,qint64)));

        // Star the timeer
        downloadTime.start();

        // Set next file
        filesLeft -= 1;
    }
    else
    {
        status = "done";
    }
}

/******************************************************************************/
/*                                                                            */
/*    Slots                                                                   */
/*                                                                            */
/******************************************************************************/

void HttpDownload::slot_downloadFinished(QNetworkReply *reply)
{
    QString fileName;

    switch(downloadPhase)
    {
        case 0:
            fileName = "launcher/downloads/files_game.txt";
            break;
        case 1:
            fileName =  fileList.at(filesLeft);
            break;
    }

    // Open the file to write to
    QFile file(installationPath + fileName);

    if(file.open(QIODevice::WriteOnly))
    {
    // Open a stream to write into the file
    QDataStream stream(&file);

    // Get the size of the file
    int size = reply->size();

    // Add size to status int for displaying
    totalSizeDownloadedCurrent = qint64(0);
    totalSizeDownloaded += qint64(size);

    // Get the data of the file
    QByteArray temp = reply->readAll();

    // Write the file
    stream.writeRawData(temp, size);

    // Set exe permissions
    file.setPermissions(QFile::ExeUser | QFile::ExeGroup | QFile::ExeOwner | QFile::WriteUser | QFile::WriteGroup | QFile::WriteOwner | QFile::ReadGroup | QFile::ReadOwner | QFile::ReadUser);

    // Close the file
    file.close();
    }
    else
    {
        int a = 0;
    }

    // If phase 0 take future steps
    switch(downloadPhase)
    {
        case 0:
            prepareDownload();
            downloadPhase = 1;
            break;
        case 1:
            getNextFile();
            break;
    }
}

void HttpDownload::downloadProgress(qint64 bytesReceived, qint64 bytesTotal)
{
    // calculate the download speed
    speed = bytesReceived * 1000.0 / downloadTime.elapsed() / 1024;
    totalSizeDownloadedCurrent = bytesReceived;
}


void HttpDownload::slot_getSSLError(QNetworkReply* reply, const QList<QSslError> &errors)
{
    QSslError sslError = errors.first();

    if(sslError.error() == 11 )
    {
        // Error on main window
    }
}

void HttpDownload::slot_verificationDone()
{
    // Calculate remaing files
    filesLeft = fileList.size();

    for(int i = 0; i < fileListSize.size(); i++)
    {
        totalSizeDownload += fileListSize.at(i).toInt();
    }

    // Set status
    status = "downloading";

    // Get the next file
    getNextFile();
}
