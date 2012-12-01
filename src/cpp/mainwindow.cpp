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
 * \brief       The main launcher window for everything
 *
 * \file    	mainwindow.cpp
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
#include "../h/mainwindow.h"
#include "ui_mainwindow.h"

/******************************************************************************/
/*                                                                            */
/*    Constructor/Deconstructor                                               */
/*                                                                            */
/******************************************************************************/

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    torrent = NULL;

    // Create the tray icon
    setupTray();

    // Translator
    translator = new QTranslator();

    // Set the current launcher version
    settings.setSetting("Relics of Annorath/launcherVersion", QCoreApplication::applicationVersion());

    // Check if launcher changed its install directory
    if(QApplication::applicationDirPath() != settings.getSetting("Relics of Annorath/installLocation"))
    {
        settings.setSetting("Relics of Annorath/installLocation", QApplication::applicationDirPath());
        configurePath();
    }
    else
    {
        configurePath();
    }

    // Remove window borders
    setWindowFlags(Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground, true);

    // Set window state as not moving
    moving = false;
    setMouseTracking(false);

    //@FIXME: What does this?
    this->ui->webView->setDisabled(true);

    // Set not muted and torrent not paused
    muted = false;
    pause = false;

    // Torrent is not done yet
    torrentDone = 0;

    // Hide the game start button
    this->ui->buttonPlay->hide();

    // Style progressbar
    this->ui->progressBar->setStyleSheet("QProgressBar {"
                                         "  color: rgb(0, 0, 0);"
                                         "  border: 0px solid;"
                                         "  background-color: rgba(255, 255, 127, 40);"
                                         "}"
                                         "QProgressBar::chunk {"
                                         "  color: rgb(0, 0, 0);"
                                         "background-color: rgba(42, 85, 0, 90);"
                                         "  width: 20px;"
                                         "}");

    // Add values to the boxes for language seleciton
    ui->boxLanguage->addItem( "English", "english" );
    ui->boxLanguage->addItem( "German", "german" );
    ui->boxLanguage->addItem( "French", "french" );
    ui->boxLanguage->addItem( "Italian", "italian" );
    ui->boxLanguage->addItem( "Spain", "spain" );

    // Add rendering options
    ui->boxRendering->addItem("OpenGL", "opengl");

#ifdef Q_WS_WIN
    // Set rendering options
    ui->boxRendering->addItem("DirectX 9", "direct3d9");
    ui->boxRendering->addItem("DirectX 10", "direct3d10");
    ui->boxRendering->addItem("DirectX 11", "direct3d11");
#endif

    // Get configured values for the language
    ui->boxLanguage->setCurrentIndex(ui->boxLanguage->findData(settings.getSetting(QString("Relics of Annorath/language"))));

    // Get renderer
    ui->boxRendering->setCurrentIndex(ui->boxRendering->findData(settings.getSetting(QString("Relics of Annorath/rendering"))));

    // Get torrent values
    ui->lineTCP->setText(settings.getSetting(QString("Relics of Annorath/torPort")));
    ui->lineMaxDL->setText(settings.getSetting(QString("Relics of Annorath/torDLMax")));
    ui->lineMaxUL->setText(settings.getSetting(QString("Relics of Annorath/torULMax")));

    // Retranslate to the new language
    retranslate();
}

MainWindow::~MainWindow()
{
    if(torrent != NULL)
    {
        delete torrent;
    }

    if(mTray != NULL)
    {
        mTray->hide();
        delete mTray;
    }

    if(ui != NULL)
    {
        delete ui;
    }
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

void MainWindow::configurePath()
{
    // Prepare http updater
    httpUpdater = new HttpUpdate(&settings);

    // Show the progress window
    httpUpdater->show();

    connect(httpUpdater, SIGNAL(updateChecked(bool, bool)), this, SLOT(slot_updateDone(bool, bool)));
}

void MainWindow::setupTray()
{

    // Creat the tray object
    mTray = new QSystemTrayIcon();

    // Set the icon
    mTray->setIcon( QIcon(":/images/icon.png"));

    // Connect the actions
    minimizeAction = new QAction(tr("Restore"), this);
    connect(minimizeAction, SIGNAL(triggered()), this, SLOT(on_buttonMinimize_clicked()));

    quitAction = new QAction(tr("Quit"), this);
    connect(quitAction, SIGNAL(triggered()), this, SLOT(on_buttonClose_clicked()));

    // Add the menu
    trayMenu = new QMenu();
    trayMenu->addAction(minimizeAction);
    trayMenu->addAction(quitAction);

    // Set the menu
    mTray->setContextMenu(trayMenu);

    connect(mTray, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this, SLOT(slot_slotTrayActivated(QSystemTrayIcon::ActivationReason)));

    mTray->show();
}

void MainWindow::toggleVisibility()
{
    // Check if main window is hidden
    if(isHidden())
    {
        // Show it
        show();

        // Check window state
        if(isMinimized())
        {
            if(isMaximized())
                showMaximized();
            else
                showNormal();
        }

        // Raise and activate it
        raise();
        activateWindow();

        // Unmute it
        if(!muted)
            sound->play();
    }
    else
    {
        // Hide everything and stop sound
        hide();

        // Mute it
        if(!muted)
            sound->stop();
    }
}

void MainWindow::retranslate()
{
    // Retranslate the whole ui while language changed
    QApplication::removeTranslator(translator);

    if(settings.getSetting("Relics of Annorath/language") == "english")
    {
        translator->load("roa_eng");
        this->ui->webView->setUrl(QUrl("https://launcher.annorath-game.com/launcher/news_en.html"));
    }
    else
    {
        translator->load("roa_ger");
        this->ui->webView->setUrl(QUrl("https://launcher.annorath-game.com/launcher/news_ger.html"));
    }

    QApplication::installTranslator(translator);

    ui->retranslateUi(this);

    // Hack while we can not rentranslate stuff outside this ui
    if(torrent != NULL)
    {
        if(pause)
        {
            if ( torrent->getProgress() == 100 )
                ui->buttonPause->setText(tr("Resume Seeding"));
            else
                ui->buttonPause->setText(tr("Resume Update"));
        }
        else
        {
            if ( torrent->getProgress() == 100 )
                ui->buttonPause->setText(tr("Pause Seeding"));
            else
                ui->buttonPause->setText(tr("Pause Update"));
        }
    }

    /// \todo Check this
    this->setEnabled(true);
}

/******************************************************************************/
/*                                                                            */
/*    Events                                                                  */
/*                                                                            */
/******************************************************************************/

void MainWindow::mousePressEvent(QMouseEvent *event)
{
    // We move the window with the left button
    QMainWindow::mousePressEvent(event);
    if((event->button() == Qt::LeftButton)) {
        moving = true;
        offset = event->pos();
    }
}

void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
    // The move event while mouse is pressed
    QMainWindow::mouseMoveEvent(event);
    if(moving)
        this->move(event->globalPos() - offset);
}

void MainWindow::mouseReleaseEvent(QMouseEvent *event)
{
    // The release event
    QMainWindow::mouseReleaseEvent(event);
    if(event->button() == Qt::LeftButton) {
        moving = false;
    }
}

/******************************************************************************/
/*                                                                            */
/*    Slots                                                                   */
/*                                                                            */
/******************************************************************************/

void MainWindow::slot_updateDone(bool updateStatus, bool updateState)
{

    // Check if update was successfully
    if(updateStatus)
    {
        // Show launcher window and hide update ui
        this->show();
        httpUpdater->hide();

        delete httpUpdater;

        // Start the torrent
        torrent = new TorrentClient(&settings);

        // Time for torrent status updating
        connect(&timer, SIGNAL(timeout()), this , SLOT(slot_updateTorrentStatus()));
        timer.start(100);

        // Create background sound
        sound = new QSound( "sounds/background.wav" );
        sound->setLoops(-1);
        sound->play();

        // When launcher was muted befor, mute it again
        if(settings.getSetting("Relics of Annorath/launcherMuted") == "true")
        {
            on_buttonMute_clicked();
        }
    }
    else
    {
        if(updateState)
        {
            // Close the appliaction for launcher update
            QApplication::quit();
        }
        else
        {
            //Display an error
            //QApplication::quit();
        }
    }
}

void MainWindow::slot_updateTorrentStatus()
{
    // Set torrent values in the gui
    ui->progressBar->setValue(torrent->getProgress());
    ui->labelDownload->setText(QString::number(torrent->getDownloadRate()) + " kB/s");
    ui->labelUpload->setText(QString::number(torrent->getUploadRate()) + " kB/s");
    ui->labelSize->setText( QString::number(torrent->getSizeDownloaded()) + " MB" + "/" + QString::number(torrent->getSize()) + " MB");

    // Check torrent status and decide what to display
    if(torrent->getStatus() == "Prepare" || torrent->getStatus() == "Checking files" || torrent->getStatus() == "Connecting" || torrent->getStatus() == "Allocating")
    {
        this->ui->progressBar->setFormat(QString(tr("Progress: %p% done - Preparing")));
    }
    else
    {
        // Calculate the hours and minutes
        int hours = (torrent->getSize() - torrent->getSizeDownloaded() ) * 1024 / (torrent->getDownloadRate() + 1) / 60 / 60;
        int minutes = ((torrent->getSize() - torrent->getSizeDownloaded() ) * 1024 / (torrent->getDownloadRate() + 1)) / 60 - (hours * 60);

        // Decide what to display
        if(hours >= 10)
        {
            this->ui->progressBar->setFormat(QString(tr("Progress: %p% done - Calculating")));
        }
        else if(hours >= 1)
        {
            this->ui->progressBar->setFormat(QString(tr("Progress: %p% done - ETA: ") + QString::number(hours) +  tr(" h and ") + QString::number(minutes) + tr(" m")));
        }
        else if ( minutes >= 1)
        {
            this->ui->progressBar->setFormat(QString(tr("Progress: %p% done - ETA: ") + QString::number(minutes) + tr(" minutes")));
        }
        else if ( torrent->getProgress() == 100 )
        {
            this->ui->progressBar->setFormat(QString(tr("Progress: %p% done")));
            this->ui->buttonPlay->show();

            // While we are seeding the text is a bit different
            if(!torrentDone)
            {
                torrentDone = 1;
                ui->buttonPause->setText(tr("Pause Seeding"));
            }
        }
        else
        {
            this->ui->progressBar->setFormat(QString(tr("Progress: %p% done - ETA: ") + tr("Some seconds left...")));
        }
    }

    // If torrent is paused we display it
    if(pause)
    {
        ui->labelStatus->setText(tr("Paused") );
    }
    else
    {
        ui->labelStatus->setText( torrent->getStatus() );
    }
}


void MainWindow::on_buttonPause_clicked()
{
    // Toggle for pause the torrent
    if(!pause)
    {
        ui->buttonPause->setStyleSheet("background-image: url(:/images/play.png);");
        pause = true;
    }
    else
    {
        ui->buttonPause->setStyleSheet("background-image: url(:/images/pause.png);");
        pause = false;
    }

    // Toggle the torrent status
    torrent->toggleStatus();
}

void MainWindow::on_buttonAccount_clicked()
{
    QDesktopServices::openUrl(QUrl("https://portal.annorath-game.com/"));
}

void MainWindow::on_buttonBugTracker_clicked()
{
    QDesktopServices::openUrl(QUrl("https://mantis.annorath-game.com/"));
}

void MainWindow::on_buttonForum_clicked()
{
    QDesktopServices::openUrl(QUrl("https://forum.annorath-game.com/"));
}

void MainWindow::on_buttonNews_clicked()
{
    /// \todo Load webview
}

void MainWindow::slot_slotTrayActivated(QSystemTrayIcon::ActivationReason reason)
{
    // Toggle the tray icon
    if (reason == QSystemTrayIcon::Trigger)
        toggleVisibility();
}

void MainWindow::on_buttonMinimize_clicked()
{
    // We send the window to tray when minimized
    toggleVisibility();
}

void MainWindow::on_buttonClose_clicked()
{
    // Quit the application
    QApplication::exit();
}

void MainWindow::on_buttonMute_clicked()
{
    switch(muted)
    {
        case true:
            sound->play();
            muted = false;
            this->ui->buttonMute->setStyleSheet("background-image: url(:/images/mute0.png);");
            settings.setSetting("Relics of Annorath/launcherMuted", "false");
            break;
        case false:
            sound->stop();
            muted = true;
            this->ui->buttonMute->setStyleSheet("background-image: url(:/images/mute1.png);");
            settings.setSetting("Relics of Annorath/launcherMuted", "true");
            break;
    }
}

void MainWindow::on_buttonPlay_clicked()
{
    // Get the current env
    QProcessEnvironment env = QProcessEnvironment::systemEnvironment();

    // The arguments to start the engine
    QStringList arguments;

    // Get fullscreen settings
    QString fscreen = settings.getSetting(QString("Relics of Annorath/fullscreen"));

    // Translate true/false to 0/1
    if(fscreen == "true")
    {
        fscreen = "1";
    }
    else
    {
        fscreen = "0";
    }

#ifdef Q_WS_WIN

    /// \todo Replace with UAC conform ShellExecute

    // Add roa to path
    env.insert("PATH", env.value("Path") + ";" + QString(settings.getSetting(QString("Relics of Annorath/installLocation")) + "/files/bin"));
    env.insert("PATH", env.value("Path") + ";" + QString(settings.getSetting(QString("Relics of Annorath/installLocation")) + "/files/lib"));

    // Get the render setting
    QString rendering = settings.getSetting(QString("Relics of Annorath/rendering"));

    // Create the arguments
    arguments << rendering << QString(QStringList(settings.getSetting(QString("Relics of Annorath/resolution")).split("x")).first()) << QString(QStringList(settings.getSetting(QString("Relics of Annorath/resolution")).split("x")).last()) << fscreen;

    // Set the new env
    game.setProcessEnvironment(env);

    // Start the process
    game.start(settings.getSetting("Relics of Annorath/installLocation") + "/files/bin/roa.exe", arguments );
#endif

#ifdef Q_OS_LINUX

    // Add roa to the ld path
    env.insert("LD_LIBRARY_PATH",QString(settings.getSetting(QString("Relics of Annorath/installLocation")) + "/files/lib"));

    // Create the arguments
    arguments << "opengl" << QString(QStringList(settings.getSetting(QString("Relics of Annorath/resolution")).split("x")).first()) << QString(QStringList(settings.getSetting(QString("Relics of Annorath/resolution")).split("x")).last()) << fscreen;

    // Set the new nev
    game.setProcessEnvironment(env);

    // Star the process
    game.start(settings.getSetting("Relics of Annorath/installLocation") + "/files/bin/roa", arguments );
#endif

    // Hide and mute launcher
    toggleVisibility();

}

void MainWindow::on_boxLanguage_currentIndexChanged(int index)
{
    settings.setSetting("Relics of Annorath/language", ui->boxLanguage->itemData(index).toString());
    retranslate();
}

void MainWindow::on_boxRendering_currentIndexChanged(int index)
{
    settings.setSetting("Relics of Annorath/rendering", ui->boxRendering->itemData(index).toString());
}

void MainWindow::on_lineMaxDL_editingFinished()
{
    settings.setSetting("Relics of Annorath/torDLMax", ui->lineMaxDL->text());
    torrent->applySettings();
}

void MainWindow::on_lineMaxUL_editingFinished()
{
    settings.setSetting("Relics of Annorath/torULMax", ui->lineMaxUL->text());
    torrent->applySettings();
}

void MainWindow::on_lineTCP_editingFinished()
{
    settings.setSetting("Relics of Annorath/torPort", ui->lineTCP->text());
    torrent->applySettings();
}

void MainWindow::on_checkHTTP_clicked()
{
    /// \todo Implement me
}
