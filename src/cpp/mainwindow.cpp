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
    settings.setSetting("launcherVersion", QCoreApplication::applicationVersion());

    // Get path and split it to remove /launcher
    QString tmpPath = QApplication::applicationDirPath().replace('\\','/');

    QStringList realPathList = tmpPath.split("/");
    tmpPath = "";

#ifdef Q_OS_LINUX
    for(int i = 0; i < realPathList.size() - 2; i++)
    {
        tmpPath += realPathList.at(i) + "/";
    }

    if(!tmpPath.endsWith("/"))
    {
        tmpPath += "/";
    }
#else
    for(int i = 0; i < realPathList.size() - 1; i++)
    {
        tmpPath += realPathList.at(i) + "/";
    }

    if(!tmpPath.endsWith("/"))
    {
        tmpPath += "/";
    }
#endif

    settings.setSetting("installLocation", tmpPath);

    // Remove window borders
    setWindowFlags(Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground, true);

    // Set window state as not moving
    moving = false;
    setMouseTracking(false);

    //@FIXME: What does this?
    //this->ui->webView->setDisabled(true);

    // Set not muted and torrent not paused
    muted = false;
    pause = false;

    // Torrent is not done yet
    torrentDone = 0;

    // Hide the game start button
    //this->ui->buttonPlay->hide();
    this->ui->buttonPlay->setText(tr("Wait..."));
    this->ui->buttonPlay->setEnabled(0);

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
    ui->boxLanguage->addItem( "Deutsch", "german" );
    ui->boxLanguage->addItem( "Français", "french" );
    ui->boxLanguage->addItem( "Italiano", "italian" );
    ui->boxLanguage->addItem( "Español", "spain" );
    ui->boxLanguage->addItem( "Pусский", "russian" );
    ui->boxLanguage->addItem( "Português", "portuguese" );
    ui->boxLanguage->addItem( "ελληνικά", "greek" );

    // Add rendering options
    ui->boxRendering->addItem("OpenGL", "opengl");

#ifdef Q_OS_WIN
    // Set rendering options
    ui->boxRendering->addItem("DirectX 9", "direct3d9");
    ui->boxRendering->addItem("DirectX 10", "direct3d10");
    ui->boxRendering->addItem("DirectX 11", "direct3d11");
#endif

    // Get configured values for the language
    ui->boxLanguage->setCurrentIndex(ui->boxLanguage->findData(settings.getSetting(QString("language"))));

    // Get renderer
    ui->boxRendering->setCurrentIndex(ui->boxRendering->findData(settings.getSetting(QString("rendering"))));

    // Connect event
    connect(ui->boxLanguage, SIGNAL(currentIndexChanged(int)),SLOT(slot_boxLanguage_currentIndexChanged(int)));
    connect(ui->boxRendering, SIGNAL(currentIndexChanged(int)),SLOT(slot_boxRendering_currentIndexChanged(int)));

    // Get torrent values
    ui->lineTCP->setText(settings.getSetting(QString("torPort")));
    ui->lineMaxDL->setText(settings.getSetting(QString("torDLMax")));
    ui->lineMaxUL->setText(settings.getSetting(QString("torULMax")));

    // Set http status
    if(settings.getSetting("useHTTP") == "true")
    {
        ui->checkHTTP->setChecked(true);
    }
    else
    {
        ui->checkHTTP->setChecked(false);
    }

    ui->webView_2->page()->mainFrame()->setScrollBarPolicy(Qt::Vertical, Qt::ScrollBarAlwaysOff);
    ui->webView_2->page()->mainFrame()->setScrollBarPolicy(Qt::Horizontal, Qt::ScrollBarAlwaysOff );

    // Set link handling
    ui->webView_2->page()->setLinkDelegationPolicy(QWebPage::DelegateAllLinks);
    connect(ui->webView_2, SIGNAL(linkClicked( QUrl )), this, SLOT( linkClickedSlot( QUrl ) ));

    // Retranslate to the new language
    retranslate();

    // Start
    configurePath();
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

    if(settings.getSetting("language") == "english")
    {
        translator->load("roa_eng");
        //this->ui->webView->setUrl(QUrl("http://dev-portal.annorath-game.com/launcher/"));
    }
    else
    {
        translator->load("roa_ger");
        //this->ui->webView->setUrl(QUrl("http://dev-portal.annorath-game.com/launcher/"));
    }

    QApplication::installTranslator(translator);

    ui->retranslateUi(this);

    // Hack while we can not rentranslate stuff outside this ui
    if(torrent != NULL)
    {
        if(pause)
        {
            if ( torrent->getProgress() == 100 )
                ui->buttonPause->setToolTip(tr("Resume Seeding"));
            else
                ui->buttonPause->setToolTip(tr("Resume Update"));
        }
        else
        {
            if ( torrent->getProgress() == 100 )
                ui->buttonPause->setToolTip(tr("Pause Seeding"));
            else
                ui->buttonPause->setToolTip(tr("Pause Update"));
        }
    }

    /// \todo Check this
    this->setEnabled(true);
}

void MainWindow::startLauncher()
{
    // Show main window
    this->show();

    // Check if we use http or bt
    if(QString(settings.getSetting("useHTTP")) == "true")
    {
        httpManager = new HttpDownload(QString(settings.getSetting(QString("installLocation"))));

        // Time for http status updating
        connect(&timer, SIGNAL(timeout()), this , SLOT(slot_updateHttpStatus()));
        timer.start(100);
        slot_updateHttpStatus();
    }
    else
    {
        // Start the torrent
        torrent = new TorrentClient(&settings);

        // Time for torrent status updating
        connect(&timer, SIGNAL(timeout()), this , SLOT(slot_updateTorrentStatus()));
        timer.start(100);
    }



    // Create background sound
    sound = new CustomSound(settings.getSetting(QString("installLocation")) + "launcher/sounds/background.wav");
    sound->play();
    sound->start();

    // When launcher was muted befor, mute it again
    if(settings.getSetting("launcherMuted") == "true")
    {
        on_buttonMute_clicked();
    }
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

        if(QString(settings.getSetting("firstRun")) == "true")
        {
            startLauncher();
        }
        else
        {
            firstRun = new FirstRun();
            connect(firstRun,SIGNAL(downloadMethodSelected(bool)), this, SLOT(slot_setDownloadMethod(bool)));
            firstRun->show();
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
            QMessageBox::warning(NULL,"Update error", "There were an update error, please restart the launcher!");
            QApplication::quit();
        }
    }

    httpUpdater->hide();
    delete httpUpdater;
}

void MainWindow::slot_setDownloadMethod(bool _mode)
{
    // Set firstrun to true
    settings.setSetting("firstRun", "true");

    if(_mode)
    {
        settings.setSetting("useHTTP", "true");
    }
    else
    {
        settings.setSetting("useHTTP", "false");
    }

    // Update gui
    ui->checkHTTP->setChecked(_mode);

    firstRun->hide();
    delete firstRun;

    startLauncher();

}

void MainWindow::slot_updateHttpStatus()
{
    // Set http values in the gui
    ui->progressBar->setValue(httpManager->progress());
    ui->labelDownload->setText(QString::number(int(httpManager->networkSpeed())) + tr(" kB/s"));
    ui->labelUpload->setText(tr("0 kB/s"));
    ui->labelSize->setText( QString::number(httpManager->getFullDownloadSize()) + tr(" MB") + "/" + QString::number(httpManager->getCurrentDownloadSize()) + tr(" MB"));

    // Check http status and decide what to display
    if(httpManager->getStatus() == "checking")
    {
        this->ui->progressBar->setFormat(QString(tr("Checking files, please be patient...")));
        this->ui->buttonPlay->setText(tr("Wait..."));
    }
    else if(httpManager->getStatus() == "downloading")
    {
        // Calculate the hours and minutes
        int hours = (httpManager->getFullDownloadSize() - httpManager->getCurrentDownloadSize() ) * 1024 / (httpManager->networkSpeed() + 1) / 60 / 60;
        int minutes = ((httpManager->getFullDownloadSize() - httpManager->getCurrentDownloadSize() ) * 1024 / (httpManager->networkSpeed() + 1)) / 60 - (hours * 60);

        if(hours >= 1)
        {
            this->ui->progressBar->setFormat(QString(tr("Progress: %p% done") + tr(" - Files left: ") + QString::number(httpManager->getFilesLeft())));
        }
        else if ( minutes >= 1)
        {
            this->ui->progressBar->setFormat(QString(tr("Progress: %p% done") + tr(" - Files left: ") + QString::number(httpManager->getFilesLeft())));
        }
        else
        {
            this->ui->progressBar->setFormat(QString(tr("Progress: %p% done") + tr(" - Files left: ") + QString::number(httpManager->getFilesLeft())));
        }

        this->ui->buttonPlay->setText(tr("Wait..."));
    }
    else if(httpManager->getStatus() == "done")
    {
        this->ui->progressBar->setFormat(QString(tr("Progress: %p% done")));
        this->ui->buttonPlay->setText(tr("PLAY"));
        this->ui->buttonPlay->setEnabled(1);
        ui->labelDownload->setText(QString("0 kB/s"));
    }

    ui->labelStatus->setText(tr("Downloading") );
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

            this->ui->buttonPlay->setText(tr("PLAY"));
            this->ui->buttonPlay->setEnabled(1);

            // While we are seeding the text is a bit different
            if(!torrentDone)
            {
                torrentDone = 1;
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
    if(QString(settings.getSetting("useHTTP")) == "true")
    {
        /// \todo Fix this
        //httpManager->toggleStatus();
    }
    else
    {
        torrent->toggleStatus();
    }
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
    QDesktopServices::openUrl(QUrl("https://portal.annorath-game.com/"));
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
            settings.setSetting("launcherMuted", "false");
            break;
        case false:
            sound->stop();
            muted = true;
            this->ui->buttonMute->setStyleSheet("background-image: url(:/images/mute1.png);");
            settings.setSetting("launcherMuted", "true");
            break;
    }
}

void MainWindow::on_buttonPlay_clicked()
{
    // The arguments to start the engine
    QStringList arguments;

    QString x, y, fscreen = "1";

    // Get screen resolution
    x = QString::number(QApplication::desktop()->screen()->width());
    y = QString::number(QApplication::desktop()->screen()->height());

    // Open roa configuration
    QDomDocument doc("roa");
    QFile file(settings.getSetting(QString("installLocation")) + "game/data/relics_of_annorath/configuration/gui/settings.xml");

    // Try to open the file
    if( file.open( QIODevice::ReadOnly ) )
    {
        // Set content
        if( doc.setContent( &file ) )
        {
            // Close file
            file.close();

            // Get DomElement
            QDomElement root = doc.documentElement();

            // Check if it the right structure
            if( root.tagName() == "config" )
            {
                // Loop through it
                QDomNode n = root.firstChild();
                n = n.firstChild();

                while( !n.isNull() )
                {
                    QDomElement e = n.toElement();

                    if( !e.isNull() )
                    {
                        if( e.tagName() == "roa_graphics_window" )
                        {
                            // Save value
                            fscreen = e.attribute("value");
                        }
                    }

                    // Try next one
                    n = n.nextSibling();
                }
            }
        }
    }

    // Check for right value
    if(fscreen != "Windowed Mode" && fscreen != "Fullscreen")
    {
#ifdef Q_OS_LINUX
        fscreen = "2";
#else
        fscreen = "1";
#endif
    }
    else if(fscreen == "Windowed Mode")
    {
        fscreen = "0";
    }
    else if(fscreen == "Fullscreen")
    {
#ifdef Q_OS_LINUX
        fscreen = "2";
#else
        fscreen = "1";
#endif
    }

#ifdef Q_OS_WIN
     QString _path = getenv("PATH");
    _path += QString(";" + settings.getSetting(QString("installLocation")) + "game/bin");
    _path += QString(";" + settings.getSetting(QString("installLocation")) + "game/lib");

    _path.replace('/','\\');

    LPCWSTR path = reinterpret_cast<const wchar_t *>(_path.utf16());
    LPCWSTR envVal = reinterpret_cast<const wchar_t *>(QString("PATH").utf16());

    SetEnvironmentVariable(envVal,path);

    ShellExecute(0, 0, reinterpret_cast<const WCHAR*>(QString(settings.getSetting(QString("installLocation")) + "game/bin/roa.exe").utf16()), reinterpret_cast<const WCHAR*>(QString(" " + settings.getSetting(QString("rendering")) + " " + x + " " + y + " " + fscreen).utf16()), 0, SW_NORMAL);
#endif

#ifdef Q_OS_LINUX

    // Get the current env
    QProcessEnvironment env = QProcessEnvironment::systemEnvironment();

    // Add roa to the ld path
    env.insert("LD_LIBRARY_PATH",QString(settings.getSetting(QString("installLocation")) + "game/lib"));

    // Create the arguments
    arguments << "opengl" << QString(x) << QString(y) << QString(fscreen);

    // Set the new nev
    game.setProcessEnvironment(env);

    // Star the process
    game.start(settings.getSetting("installLocation") + "game/bin/roa", arguments );
#endif

    // Hide and mute launcher
    toggleVisibility();

}

void MainWindow::slot_boxLanguage_currentIndexChanged(int index)
{
    settings.setSetting("language", ui->boxLanguage->itemData(index).toString());
    retranslate();
}

void MainWindow::slot_boxRendering_currentIndexChanged(int index)
{
    settings.setSetting("rendering", ui->boxRendering->itemData(index).toString());
}

void MainWindow::on_lineMaxDL_editingFinished()
{
    settings.setSetting("torDLMax", ui->lineMaxDL->text());
    if(torrent != NULL)
        torrent->applySettings();
}

void MainWindow::on_lineMaxUL_editingFinished()
{
    settings.setSetting("torULMax", ui->lineMaxUL->text());
    if(torrent != NULL)
        torrent->applySettings();
}

void MainWindow::on_lineTCP_editingFinished()
{
    settings.setSetting("torPort", ui->lineTCP->text());
    if(torrent != NULL)
        torrent->applySettings();
}

void MainWindow::on_checkHTTP_clicked()
{
    if(ui->checkHTTP->isChecked())
    {
        settings.setSetting("useHTTP", "true");
    }
    else
    {
        settings.setSetting("useHTTP", "false");
    }

    QMessageBox::information(this,tr("Please restart the launcher"),tr("Please restart the launcher to switch the download mode."));
}

void MainWindow::on_buttonContact_clicked()
{
    QDesktopServices::openUrl(QUrl("https://portal.annorath-game.com/"));
}

void MainWindow::on_buttonHelp_clicked()
{
    QDesktopServices::openUrl(QUrl("https://portal.annorath-game.com/"));
}

void MainWindow::on_buttonFAQ_clicked()
{
    QDesktopServices::openUrl(QUrl("https://portal.annorath-game.com/"));
}

void MainWindow::linkClickedSlot(QUrl _url)
{
    if(!_url.toString().contains("#"))
    {
        QDesktopServices::openUrl( _url );
    }
}

void MainWindow::slot_error(QProcess::ProcessError _error)
{
    if(_error == QProcess::FailedToStart)
    {
        QMessageBox::warning(this,"","Start failed");
    }
    if(_error == QProcess::Crashed)
    {
        QMessageBox::warning(this,"","crashed failed");
    }
    if(_error == QProcess::Timedout)
    {
        QMessageBox::warning(this,"","time failed");
    }
    if(_error == QProcess::WriteError)
    {
        QMessageBox::warning(this,"","wr failed");
    }
    if(_error == QProcess::ReadError)
    {
        QMessageBox::warning(this,"","re failed");
    }
    if(_error == QProcess::UnknownError)
    {
        QMessageBox::warning(this,"","un failed");
    }
}

