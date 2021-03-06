/* 
 * Copyright (c) 2006 Sean C. Rhea (srhea@srhea.net)
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the Free
 * Software Foundation; either version 2 of the License, or (at your option)
 * any later version.
 * 
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 * 
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc., 51
 * Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */

#ifndef _GC_Settings_h
#define _GC_Settings_h 1
    
#define GC_SETTINGS_CO              "goldencheetah.org"
#define GC_SETTINGS_APP             "GoldenCheetah"
#define GC_SETTINGS_LAST            "mainwindow/lastOpened"
#define GC_SETTINGS_MAIN_WIDTH      "mainwindow/width"
#define GC_SETTINGS_MAIN_HEIGHT     "mainwindow/height"
#define GC_SETTINGS_MAIN_X          "mainwindow/x"
#define GC_SETTINGS_MAIN_Y          "mainwindow/y"
#define GC_SETTINGS_MAIN_GEOM       "mainwindow/geometry"
#define GC_SETTINGS_SPLITTER_SIZES  "mainwindow/splitterSizes"
#define GC_SETTINGS_CALENDAR_SIZES  "mainwindow/calendarSizes"
#define GC_DATETIME_FORMAT          "ddd MMM dd, yyyy, hh:mm AP"
#define GC_UNIT                     "unit"
#define GC_SETTINGS_LAST_IMPORT_PATH "mainwindow/lastImportPath"
#define GC_ALLRIDES_ASCENDING       "allRidesAscending"
#define GC_CRANKLENGTH              "crankLength"
#define GC_BIKESCOREDAYS	    "bikeScoreDays"
#define GC_BIKESCOREMODE	    "bikeScoreMode"
#define GC_INITIAL_LTS		    "initialLTS"
#define GC_INITIAL_STS		    "initialSTS"
#define GC_LTS_DAYS		    "LTSdays"
#define GC_STS_DAYS		    "STSdays"
#define GC_STS_NAME			"STSname"
#define GC_STS_ACRONYM			"STS"
#define GC_LTS_NAME			"LTSname"
#define GC_LTS_ACRONYM			"LTS"
#define GC_SB_NAME			"SBname"
#define GC_SB_ACRONYM			"SB"

#include <QSettings>
#include <boost/shared_ptr.hpp>

inline boost::shared_ptr<QSettings> GetApplicationSettings()
{ 
  boost::shared_ptr<QSettings> settings;
  QDir home = QDir();
    //First check to see if the Library folder exists where the executable is (for USB sticks)
  if(!home.exists("Library/GoldenCheetah"))
    settings = boost::shared_ptr<QSettings>(new QSettings(GC_SETTINGS_CO, GC_SETTINGS_APP));
  else
    settings = boost::shared_ptr<QSettings>(new QSettings(home.absolutePath()+"/gc", QSettings::IniFormat));
  
  return settings;
}

#endif // _GC_Settings_h

