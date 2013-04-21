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
 * \file    	aboutlauncher.h
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

#ifndef ABOUT_LAUNCHER_H
#define ABOUT_LAUNCHER_H

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
#include <QWidget>
#include <QDesktopWidget>

/******************************************************************************/
/*                                                                            */
/*    Others includes                                                         */
/*                                                                            */
/******************************************************************************/

#define ABOUT_PAGE_LICENSE 0
#define ABOUT_PAGE_DEVELOPERS 1
#define ABOUT_PAGE_TRANSLATORS 2

/**
 * \addtogroup Ui
 */
namespace Ui
{
    class AboutLauncher;
}

/**
 * \brief Displays about widget
 */
class AboutLauncher : public QWidget
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
         * @brief Constructor
         * @param parent Parent widget
         */
        explicit AboutLauncher(QWidget *parent = 0);

        /**
          * @brief Deconstructor
          */
        ~AboutLauncher();
    
    private slots:
        void on_bLicense_clicked();
        void on_bDevelopers_clicked();
        void on_bTranslators_clicked();
        void on_bClose_clicked();

    private:

        /******************************************************************************/
        /*                                                                            */
        /*    Members                                                                 */
        /*                                                                            */
        /******************************************************************************/

        /**
         * @brief The ui file aka form
         */
        Ui::AboutLauncher *ui;

        /******************************************************************************/
        /*                                                                            */
        /*    Methods                                                                 */
        /*                                                                            */
        /******************************************************************************/

        void displayText(int _index);
};

#endif // ABOUT_LAUNCHER_H
