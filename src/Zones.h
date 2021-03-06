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

#ifndef _Zones_h
#define _Zones_h

#include <QtCore>

class Zones : public QObject
{
    Q_OBJECT

    protected:
        struct ZoneInfo {
            QString name, desc;
            int lo, hi;
            ZoneInfo(const QString &n, const QString &d, int l, int h) :
                name(n), desc(d), lo(l), hi(h) {}
        };
    
        struct ZoneRange {
            QDate begin, end;
            int cp;
            QList<ZoneInfo*> zones;
            bool zonesSetFromCP;
            ZoneRange(const QDate &b, const QDate &e) : 
	        begin(b), end(e), cp(0), zonesSetFromCP(false) { fprintf(stderr, "Generating CP=0 range\n"); }
  	    ZoneRange(const QDate &b, const QDate &e, int _cp) : 
   	        begin(b), end(e), cp(_cp), zonesSetFromCP(false) { fprintf(stderr, "Generating range with CP=%d\n", _cp); }
            ~ZoneRange() {
                QListIterator<ZoneInfo*> i(zones); 
       	        while (i.hasNext()) 
    	            delete i.next();
            }
        };
    

        QList<ZoneRange*> ranges;
        QString err, warning;

	void setZonesFromCP(ZoneRange *range);

	static bool zoneptr_lessthan(ZoneInfo *z1, ZoneInfo *z2);
	static bool rangeptr_lessthan(ZoneRange *r1, ZoneRange *r2);
	static bool zone_default_index_lessthan(int i1, int i2);

	bool defaults_from_user;

    public:
        Zones() : defaults_from_user(false) {}

        ~Zones() {
            QListIterator<ZoneRange*> i(ranges); 
            while (i.hasNext()) 
                delete i.next();
        }

        void addZoneRange(QDate _start, QDate _end, int _cp);
        void addZoneRange(int _cp);
        void addZoneRange();

        bool read(QFile &file);
        void write(QDir home);
        const QString &errorString() const { return err; }
        const QString &warningString() const { return warning; }

        int whichRange(const QDate &date) const;
        int numZones(int range) const;
        int whichZone(int range, double value) const;
        void zoneInfo(int range, int zone,
                      QString &name, QString &description,
                      int &low, int &high) const;
        QString summarize(int rnum, QVector<double> &time_in_zone) const;
        int getCP(int rnum) const;
        void setCP(int rnum, int cp);
	QString getDefaultZoneName(int z);
	QString getDefaultZoneDesc(int z);
	void setZonesFromCP(int rnum);
	int lowsFromCP(QList <int> *lows, int CP);
	QList <int> getZoneLows(int rnum);
	QList <int> getZoneHighs(int rnum);
	QList <QString> getZoneNames(int rnum);
        QDate getStartDate(int rnum);
        QDate getEndDate(int rnum);
        QString getStartDateString(int rnum);
        QString getEndDateString(int rnum);
        void setEndDate(int rnum, QDate date);
        void setStartDate(int rnum, QDate date);
        int getRangeSize();
	QDateTime modificationTime;

	// set zone parameters to either user-specified defaults
	// or to defaults using Coggan's coefficients
	static void initializeZoneParameters();

	// delete the range rnum, and adjust dates on adjacent zone; return 
	// the range number of the range extended to cover the deleted zone
	int deleteRange(const int rnum);

	// insert a range from the given date to the end date of the range
	// presently including the date
	int insertRangeAtDate(QDate date, int cp = 0);
};

QColor zoneColor(int zone, int num_zones);

#endif // _Zones_h
