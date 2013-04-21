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
 * \brief       The settings object for accessing and saving settings
 *
 * \file    	settings.cpp
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
#include "../h/settings.h"

/******************************************************************************/
/*                                                                            */
/*    Constructor/Deconstructor                                               */
/*                                                                            */
/******************************************************************************/

Settings::Settings()
{
    //Init Settings Object
    userSettings = new QSettings(QSettings::IniFormat, QSettings::UserScope, QCoreApplication::organizationName(), QCoreApplication::applicationName());

    //Check if client is configured
    if(userSettings->value("configured").toString() != "true")
    {
        setDefaultSettings();
    }
}

Settings::~Settings()
{
    delete userSettings;
}

/******************************************************************************/
/*                                                                            */
/*    Public methods                                                          */
/*                                                                            */
/******************************************************************************/

QString Settings::getSetting(QString _settingName)
{
    return userSettings->value(_settingName, "not_found").toString();
}

void Settings::setSetting(QString _settingName, QString _settingValue)
{
    userSettings->setValue(_settingName, _settingValue);
}

/******************************************************************************/
/*                                                                            */
/*    Private methods                                                         */
/*                                                                            */
/******************************************************************************/

void Settings::setDefaultSettings()
{
//    switch(QLocale::system().language())
//    {
//        case QLocale::English:
//            userSettings->setValue("language","english");
//            break;
//        case QLocale::German:
//            userSettings->setValue("language","german");
//            break;
//        case QLocale::Italian:
//            userSettings->setValue("language","italian");
//            break;
//        case QLocale::French:
//            userSettings->setValue("language","french");
//            break;
//        case QLocale::Spanish:
//            userSettings->setValue("language","spain");
//            break;
//        default:
//            userSettings->setValue("language","english");
//            break;
//    }

    userSettings->setValue("torPort","6881");
    userSettings->setValue("torDLMax", "0");
    userSettings->setValue("torULMax","0");
    userSettings->setValue("torConMax","1000");

    userSettings->setValue("configured","true");
    userSettings->setValue("version","0");
    userSettings->setValue("launcherVersion","0");

    userSettings->setValue("launcherMuted","false");

#ifdef Q_OS_LINUX
    userSettings->setValue("rendering","opengl");
#endif

#ifdef Q_OS_WIN
    userSettings->setValue("rendering","direct3d11");
#endif

}
