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
 * \brief       The world famous main
 *
 * \file    	main.cpp
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
/*    Qt includes                                                             */
/*                                                                            */
/******************************************************************************/
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX

#include <QApplication>
#include <QtGui>
#include <QFontDatabase>
#include "../external/qtsingleapplication.h"

/******************************************************************************/
/*                                                                            */
/*    Others includes                                                         */
/*                                                                            */
/******************************************************************************/
#include "../h/mainwindow.h"

/**
 * \brief The main loop-
 *
 * The main class of the appliaction.
 *
 * \param[in] argc Count of arguments.
 * \param[in] *argv Array with the arguments.
 *
 * \return Returns QAppliaction state.
 *
*/
int main(int argc, char *argv[])
{
    // Create the QAppliaction object
    QtSingleApplication a(argc, argv);

    // Set appliaction properties
    a.setApplicationName("Relics of Annorath Launcher");
    a.setApplicationDisplayName("Relics of Annorath Launcher"); // This seems to have no affect yet, wait for qt5 realese version

    a.setApplicationVersion("003.000.005"); // Do not fuck this up, it is used for application update process

    a.setOrganizationName("QuantumBytes inc.");
    a.setOrganizationDomain("quantum-bytes.com");

    // Add custom font
    QFontDatabase::addApplicationFont(":/font/ModernAntiqua.ttf");

    // Check if launcer is running
    if (a.isRunning())
    {
        return 0;
    }

    // Create the MainWindow
    MainWindow w;

    // Activate window
    a.setActivationWindow(&w);

    return a.exec();
}

