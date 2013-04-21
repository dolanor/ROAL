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
 * \brief       About dialog
 *
 * \file    	aboutlauncher.cpp
 *
 * \note
 *
 * \version 	2.0
 *
 * \author  	Manuel Gysin <manuel.gysin@quantum-bytes.com>
 *
 * \date        2013/04/14 19:00:00 GMT+1
 *
 */

/******************************************************************************/
/*                                                                            */
/*    Others includes                                                         */
/*                                                                            */
/******************************************************************************/
#include "../h/aboutlauncher.h"
#include "ui_aboutlauncher.h"

/******************************************************************************/
/*                                                                            */
/*    Constructor/Deconstructor                                               */
/*                                                                            */
/******************************************************************************/
AboutLauncher::AboutLauncher(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AboutLauncher)
{
    // Setup ui
    ui->setupUi(this);

    // Center the widget
    this->setGeometry(QStyle::alignedRect(
                          Qt::LeftToRight,
                          Qt::AlignCenter,
                          this->size(),
                          qApp->desktop()->availableGeometry()
                          ));

    // Remove window borders
    setWindowFlags(Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground, true);

    displayText(ABOUT_PAGE_LICENSE);
}

AboutLauncher::~AboutLauncher()
{
    // Clean up
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

void AboutLauncher::displayText(int _index)
{
    QString text;
    QString title;

    switch(_index)
    {
        case ABOUT_PAGE_LICENSE:
            text = "Relics of Annorath Launcher is free software: "
                    "you can redistribute it and/or modify it under the terms "
                    "of the GNU General Public License as published by the Free "
                    "Software Foundation, either version 3 of the License, or any later version."
                    "\n\n"
                    "Relics of Annorath Launcher is distributed in the hope that it will be useful, "
                    "but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or "
                    "FITNESS FOR A PARTICULAR PURPOSE."
                    "\n"
                    "See the GNU General Public License for more details."
                    "\n\n"
                    "You should have received a copy of the GNU General Public License along "
                    "with Relics of Annorath Launcher. If not, see <http://www.gnu.org/licenses/>.";

            title = tr("License");

            ui->bLicense->setStyleSheet("background-color: rgb(255, 255, 255, 130);"
                                           "border-width:1px;"
                                           "border-style:solid;"
                                           "border-radius:4px;"
                                           "border-color: rgba(0, 0, 0, 120);"
                                           "color: rgb(0, 0, 0);"
                                           "background-image: url(:);");

            ui->bTranslators->setStyleSheet("background-color: rgb(255, 255, 255, 60);"
                                           "border-width:1px;"
                                           "border-style:solid;"
                                           "border-radius:4px;"
                                           "border-color: rgba(0, 0, 0, 120);"
                                           "color: rgb(0, 0, 0);"
                                           "background-image: url(:);");

            ui->bDevelopers->setStyleSheet("background-color: rgb(255, 255, 255, 60);"
                                           "border-width:1px;"
                                           "border-style:solid;"
                                           "border-radius:4px;"
                                           "border-color: rgba(0, 0, 0, 120);"
                                           "color: rgb(0, 0, 0);"
                                           "background-image: url(:);");

            break;

        case ABOUT_PAGE_DEVELOPERS:
            text = "Code\n"
                   "Manuel Gysin (QuantumBytes inc.)\n"
                   "Lars Saalbach (QuantumBytes inc.)"
                   "\n\n"
                   "Desgin\n"
                   "Pascal Winistörfer (QuantumBytes inc.)";

            title = tr("Developers");

            ui->bLicense->setStyleSheet("background-color: rgb(255, 255, 255, 60);"
                                           "border-width:1px;"
                                           "border-style:solid;"
                                           "border-radius:4px;"
                                           "border-color: rgba(0, 0, 0, 120);"
                                           "color: rgb(0, 0, 0);"
                                           "background-image: url(:);");

            ui->bTranslators->setStyleSheet("background-color: rgb(255, 255, 255, 60);"
                                           "border-width:1px;"
                                           "border-style:solid;"
                                           "border-radius:4px;"
                                           "border-color: rgba(0, 0, 0, 120);"
                                           "color: rgb(0, 0, 0);"
                                           "background-image: url(:);");

            ui->bDevelopers->setStyleSheet("background-color: rgb(255, 255, 255, 130);"
                                           "border-width:1px;"
                                           "border-style:solid;"
                                           "border-radius:4px;"
                                           "border-color: rgba(0, 0, 0, 120);"
                                           "color: rgb(0, 0, 0);"
                                           "background-image: url(:);");

            break;

        case ABOUT_PAGE_TRANSLATORS:
            text = "Crajz1 - Swedish\n"
                   "Jaxon - Polish\n"
                   "Nebi - Spanish\n"
                   "French/Raska - French\n"
                   "ilcontegis - Italian\n"
                   "booly - Greek\n"
                   "Sohara - Portuguese Brazil\n";

            title = tr("Translators");

            ui->bLicense->setStyleSheet("background-color: rgb(255, 255, 255, 60);"
                                           "border-width:1px;"
                                           "border-style:solid;"
                                           "border-radius:4px;"
                                           "border-color: rgba(0, 0, 0, 120);"
                                           "color: rgb(0, 0, 0);"
                                           "background-image: url(:);");

            ui->bTranslators->setStyleSheet("background-color: rgb(255, 255, 255, 130);"
                                           "border-width:1px;"
                                           "border-style:solid;"
                                           "border-radius:4px;"
                                           "border-color: rgba(0, 0, 0, 120);"
                                           "color: rgb(0, 0, 0);"
                                           "background-image: url(:);");

            ui->bDevelopers->setStyleSheet("background-color: rgb(255, 255, 255, 60);"
                                           "border-width:1px;"
                                           "border-style:solid;"
                                           "border-radius:4px;"
                                           "border-color: rgba(0, 0, 0, 120);"
                                           "color: rgb(0, 0, 0);"
                                           "background-image: url(:);");

            break;
    }

    ui->lTitle->setText(title);
    ui->tText->setPlainText(text);
}

/******************************************************************************/
/*                                                                            */
/*    Slots                                                                   */
/*                                                                            */
/******************************************************************************/

void AboutLauncher::on_bLicense_clicked()
{
    displayText(ABOUT_PAGE_LICENSE);
}

void AboutLauncher::on_bDevelopers_clicked()
{
    displayText(ABOUT_PAGE_DEVELOPERS);
}

void AboutLauncher::on_bTranslators_clicked()
{
    displayText(ABOUT_PAGE_TRANSLATORS);
}

void AboutLauncher::on_bClose_clicked()
{
    this->hide();
}
