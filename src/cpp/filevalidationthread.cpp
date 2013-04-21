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
 * \file    	filevalidationthread.cpp
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
#include "../h/filevalidationthread.h"

/******************************************************************************/
/*                                                                            */
/*    Constructor/Deconstructor                                               */
/*                                                                            */
/******************************************************************************/

/******************************************************************************/
/*                                                                            */
/*    Public methods                                                          */
/*                                                                            */
/******************************************************************************/

void FileValidationThread::setData(QString _installationPath, QStringList *_files, QStringList *_hashes, QStringList *_sizes)
{
    // Set values
    files = _files;
    hashes = _hashes;
    sizes = _sizes;
    installationPath = _installationPath;
}

void FileValidationThread::run()
{
    QStringList cFiles, cHashes, cSizes;

    // Loop throug each file
    for(int i = 0; i < files->size(); i++)
    {
        QFile file(installationPath + files->at(i));

        if(file.open(QIODevice::ReadOnly))
        {
            // Get bytes
            QByteArray fileData = file.readAll();

            // Get hash
            QByteArray hash = QCryptographicHash::hash(fileData, QCryptographicHash::Sha256);

            // Compare
            if(QString(hash.toHex()) != hashes->at(i))
            {
                cFiles.append(files->at(i));
                cHashes.append(hashes->at(i));
                cSizes.append(sizes->at(i));
            }
        }
        else
        {
            // Add to fiels to download
            cFiles.append(files->at(i));
            cHashes.append(hashes->at(i));
            cSizes.append(sizes->at(i));
        }
    }

    // Clear memory
    files->clear();
    hashes->clear();
    sizes->clear();

    // Copy needed files to real vectors
    /// \todo Check for a swap/copy function
    for(int i = 0; i < cFiles.size(); i++)
    {
        files->append(cFiles.at(i));
        hashes->append(cHashes.at(i));
        sizes->append(cSizes.at(i));
    }

}

/******************************************************************************/
/*                                                                            */
/*    Private methods                                                         */
/*                                                                            */
/******************************************************************************/

/******************************************************************************/
/*                                                                            */
/*    Events                                                                  */
/*                                                                            */
/******************************************************************************/

/******************************************************************************/
/*                                                                            */
/*    Slots                                                                   */
/*                                                                            */
/******************************************************************************/
