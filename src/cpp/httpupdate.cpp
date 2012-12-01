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

/******************************************************************************/
/*                                                                            */
/*    Others includes                                                         */
/*                                                                            */
/******************************************************************************/
#include "../h/httpupdate.h"
#include "ui_httpupdate.h"

#ifdef Q_WS_WIN
#include <Windows.h>
#endif

/******************************************************************************/
/*                                                                            */
/*    Constructor/Deconstructor                                               */
/*                                                                            */
/******************************************************************************/

HttpUpdate::HttpUpdate(Settings *_settings, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::HttpUpdate)
{
    // Create the GUI
    ui->setupUi(this);

    setWindowFlags(Qt::FramelessWindowHint);

    // Create the settings object
    settings = _settings;

    // Set the path of the torrent
    QString torrentPath = settings->getSetting("Relics of Annorath/installLocation") + "/roa.torrent";

    // Check if the torrent exists
    if(!QFile::exists(torrentPath))
    {
        // If does not exists we set version to 0, so we force a check of the game content.
        settings->setSetting(QString("Relics of Annorath/version"), "0");
    }

    // Add certificates
    certificates.append(QSslCertificate::fromPath(":/certs/class2.pem"));
    certificates.append(QSslCertificate::fromPath(":/certs/ca.pem"));

    // Set ssl config
    sslConfig.defaultConfiguration();
    sslConfig.setCaCertificates(certificates);

    request.setSslConfiguration(sslConfig);

    // Retranslate
    ui->retranslateUi(this);

    // Start and Connect the timer
    connect(&timer, SIGNAL(timeout()), this , SLOT(slot_startCheck()));
    timer.start(2000);
}

HttpUpdate::~HttpUpdate()
{
    delete ui;
}

/******************************************************************************/
/*                                                                            */
/*    Public methods                                                          */
/*                                                                            */
/******************************************************************************/

/******************************************************************************/
/*                                                                            */
/*    Private methods                                                         */
/*                                                                            */
/******************************************************************************/

void HttpUpdate::downloadInstaller()
{
    // Download the installer file from the remote server
#ifdef Q_OS_WIN32
#ifdef Q_OS_WIN64
    manager.get(QNetworkRequest(QUrl("https://launcher.annorath-game.com/launcher/installer_win_x86_64.exe")));
#else
    manager.get(QNetworkRequest(QUrl("https://launcher.annorath-game.com/launcher/installer_win_x86.exe")));
#endif
#endif

#ifdef Q_OS_LINUX
#ifdef __x86_64__
    request.setUrl(QUrl("https://launcher.annorath-game.com/launcher/installer_lin_x86_64.bin"));
#else
    request.setUrl(QUrl("https://launcher.annorath-game.com/launcher/installer_lin_x86.bin"));
#endif
#endif

    connect(&manager, SIGNAL(finished(QNetworkReply*)),this, SLOT(slot_startInstaller(QNetworkReply*)));
    manager.get(request);
}

void HttpUpdate::downloadTorrent()
{
    // Download the new torrent
    request.setUrl(QUrl("https://launcher.annorath-game.com/launcher/roa.torrent"));

    connect(&manager, SIGNAL(finished(QNetworkReply*)),this, SLOT(slot_saveTorrent(QNetworkReply*)));

    manager.get(request);
}

/******************************************************************************/
/*                                                                            */
/*    Slots                                                                   */
/*                                                                            */
/******************************************************************************/

void HttpUpdate::slot_startCheck()
{
    // Stop the timer we started in the constructor
    timer.stop();

    // Connect the manager to the finished signal, when the signal is stated we have everything
    connect(&manager, SIGNAL(finished(QNetworkReply*)),this, SLOT(slot_checkVersion(QNetworkReply*)));
    connect(&manager, SIGNAL(sslErrors(QNetworkReply*, const QList<QSslError>&)),this, SLOT(getSSLError(QNetworkReply*, const QList<QSslError>&)));

    // Download the version file from the remote server
    request.setUrl(QUrl("https://launcher.annorath-game.com/launcher/launcher.txt"));

    manager.get(request);
}

void HttpUpdate::slot_checkVersion(QNetworkReply *reply)
{
    // Disconnect signal
    disconnect(&manager, SIGNAL(finished(QNetworkReply*)),this, SLOT(slot_checkVersion(QNetworkReply*))); /// \warning This is essential, else wrong data is read later!

    if(reply->error() == QNetworkReply::NoError)
    {
        // Set the remote version
        remoteVersion = reply->read(11);

        // Check with our version
        if(QCoreApplication::applicationVersion() == remoteVersion)
        {
            ui->label->setText(tr("No launcher updates found"));
            downloadTorrent(); // We do this everytime, best way to keep content up2date
        }
        else
        {
            ui->label->setText(tr("Downloading new launcher"));
            downloadInstaller();
        }
    }
    // If error, we stop everything
    else
    {
        // Print error in the widget
        ui->label->setText(reply->errorString()+ " !");

        // Send the singla that we are done here, but something goes horrible wrong
        emit updateChecked(false, false);
    }
}

void HttpUpdate::slot_startInstaller(QNetworkReply *reply)
{
    // Disconnect signal
    disconnect(&manager, SIGNAL(finished(QNetworkReply*)),this, SLOT(slot_startInstaller(QNetworkReply*))); /// \warning This is essential, else wrong data is read later!

    if(reply->error() == QNetworkReply::NoError)
    {
        ui->label->setText(tr("Saving new launcher"));

        // Check if the folder exists
        if(!QDir(settings->getSetting("Relics of Annorath/installLocation") + "/downloads").exists())
        {
            QDir().mkdir(settings->getSetting("Relics of Annorath/installLocation") + "/downloads");
        }

        // Open the file to write to
#ifdef Q_OS_WIN32
        QFile file(settings->getSetting("Relics of Annorath/installLocation") + "/downloads/launcher.exe");
#endif

#ifdef Q_OS_LINUX
        QFile file(settings->getSetting("Relics of Annorath/installLocation") + "/downloads/launcher.tar.gz");
#endif

        file.open(QIODevice::WriteOnly);

        // Open a stream to write into the file
        QDataStream stream(&file);

        // Get the size of the torrent
        int size = reply->size();

        // Get the data of the torrent
        QByteArray temp = reply->readAll();

        // Write the file
        stream.writeRawData(temp, size);

        // Close the file
        file.close();

#ifdef Q_WS_WIN
        // Needed cause of UAC, my lord... Setup is running with /silent
        ShellExecute(0, 0,  reinterpret_cast<const WCHAR*>(QString(settings->getSetting("Relics of Annorath/installLocation") + "/downloads/launcher.exe").utf16()), reinterpret_cast<const WCHAR*>(QString(" /silent").utf16()), NULL, SW_SHOWNORMAL);
        emit updateChecked(false, true);
#endif

#ifdef Q_OS_LINUX
        // While the client is a linux os, we can simply overwrite stuff and restart the launcher
        //installer.start("tar xfvz " + settings->getSetting("Relics of Annorath/installLocation") + "/downloads/launcher.tar.gz" + " -C " + settings->getSetting("Relics of Annorath/installLocation"));

        // When process is done, send signal for QApplicationExit and new start of the launcher
        //connect(&installer, SIGNAL(finished(int, QProcess::ExitStatus)),this, SLOT(installerFinished(int, QProcess::ExitStatus)));
#endif
    }
    // If error, we stop everything
    else
    {
        // Print error in the widget
        ui->label->setText(reply->errorString()+ " !");

        // Send the singla that we are done here, but something goes horrible wrong
        emit updateChecked(false, false);
    }
}

void HttpUpdate::slot_saveTorrent(QNetworkReply *reply)
{
    // Disconnect signal
    disconnect(&manager, SIGNAL(finished(QNetworkReply*)),this, SLOT(slot_saveTorrent(QNetworkReply*))); /// \warning This is essential, else wrong data is read later!

    if(reply->error() == QNetworkReply::NoError)
    {
        // Check if the folder exists
        if(!QDir(settings->getSetting("Relics of Annorath/installLocation") + "/downloads").exists())
        {
            QDir().mkdir(settings->getSetting("Relics of Annorath/installLocation") + "/downloads");
        }

        // Open the file to write to
        QFile file(settings->getSetting("Relics of Annorath/installLocation") + "/downloads/roa.torrent");
        file.open(QIODevice::WriteOnly);

        // Open a stream to write into the file
        QDataStream stream(&file);

        // Get the size of the torrent
        int size = reply->size();

        // Get the data of the torrent
        QByteArray temp = reply->readAll();

        // Write the file
        stream.writeRawData(temp, size);

        // Close the file
        file.close();

        // Set the new version to the settings
        settings->setSetting(QString("Relics of Annorath/version"), remoteVersion);

        // Send the signal that we are done here
        emit updateChecked(true, false);
    }
    // If error, we stop everything
    else
    {
        // Print error in the widget
        ui->label->setText(reply->errorString()+ " !");

        // Send the singla that we are done here, but something goes horrible wrong
        emit updateChecked(false, false);
    }
}

void HttpUpdate::slot_getSSLError(QNetworkReply* reply, const QList<QSslError> &errors)
{
    // We only take the first error
    QSslError sqlError = errors.first();

    if(sqlError.error() == 11 )
    {
        //reply->ignoreSslErrors();
        ui->label->setText(reply->errorString());
    }
}
