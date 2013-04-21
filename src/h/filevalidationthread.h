#ifndef FILEVALIDATIONTHREAD_H
#define FILEVALIDATIONTHREAD_H

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
 * \brief       Non-blocking file validation as thread.
 *
 * \file    	filevalidationthread.h
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
#include <QThread>
#include <QStringList>
#include <QFile>
#include <QCryptographicHash>

/******************************************************************************/
/*                                                                            */
/*    Others includes                                                         */
/*                                                                            */
/******************************************************************************/

/**
 * @brief Non-blocking file validation as thread
 */
class FileValidationThread : public QThread
{
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
         * @brief Set data for checking
         * @param _installationPath The installation path
         * @param _files Files to check
         * @param _hashes Hashes for validation
         * @param _sizes Sizes
         */
        void setData(QString _installationPath, QStringList *_files, QStringList *_hashes, QStringList *_sizes);

        /**
         * @brief Start thread
         */
        void run();

        /**
         * @brief Result
         * @return The result
         */
        bool getResult();

    private:

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
         * @brief The result
         */
        bool result;

        /**
         * @brief Installation path
         */
        QString installationPath;

        /**
         * @brief Stringlists with files, hashes and sizes.
         */
        QStringList *files, *hashes, *sizes;
};

#endif // FILEVALIDATIONTHREAD_H
