/* 
 * Copyright (c) 2008 Sean C. Rhea (srhea@srhea.net)
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

#ifndef _GC_RideMetric_h
#define _GC_RideMetric_h 1

#include <QHash>
#include <QString>
#include <QVector>
#include <assert.h>

#include "RideFile.h"

class Zones;

struct RideMetric {
    virtual ~RideMetric() {}
    virtual QString name() const = 0;
    virtual QString units(bool metric) const = 0;
    virtual double value(bool metric) const = 0;
    virtual void compute(const RideFile *ride, 
                         const Zones *zones, 
                         int zoneRange,
                         const QHash<QString,RideMetric*> &deps) = 0;
    virtual bool canAggregate() const { return false; }
    virtual void aggregateWith(RideMetric *other) { 
        (void) other; 
        assert(false); 
    }
    virtual RideMetric *clone() const = 0;
};

struct PointwiseRideMetric : public RideMetric {
    void compute(const RideFile *ride, const Zones *zones, int zoneRange,
                 const QHash<QString,RideMetric*> &) {
        foreach (const RideFilePoint *point, ride->dataPoints())
            perPoint(point, ride->recIntSecs(), ride, zones, zoneRange);
    }
    virtual void perPoint(const RideFilePoint *point, double secsDelta,
                          const RideFile *ride, const Zones *zones,
                          int zoneRange) = 0;
};

class AvgRideMetric : public PointwiseRideMetric {

    protected:

    int count;
    double total;

    public:

    AvgRideMetric() : count(0), total(0.0) {}
    double value(bool) const {
        if (count == 0) return 0.0;
        return total / count;
    }
    void aggregateWith(RideMetric *other) { 
        assert(name() == other->name());
        AvgRideMetric *as = dynamic_cast<AvgRideMetric*>(other);
        count += as->count;
        total += as->total;
    }
};

class RideMetricFactory {

    static RideMetricFactory *_instance;
    static QVector<QString> noDeps;

    QVector<QString> metricNames;
    QHash<QString,RideMetric*> metrics;
    QHash<QString,QVector<QString>*> dependencyMap;

    RideMetricFactory() {}
    RideMetricFactory(const RideMetricFactory &other);
    RideMetricFactory &operator=(const RideMetricFactory &other);

    public:

    static RideMetricFactory &instance() {
        if (!_instance)
            _instance = new RideMetricFactory();
        return *_instance;
    }

    int metricCount() const { return metricNames.size(); }

    const QString &metricName(int i) const { return metricNames[i]; }

    RideMetric *newMetric(const QString &name) const {
        assert(metrics.contains(name));
        return metrics.value(name)->clone();
    }

    bool addMetric(const RideMetric &metric,
                   const QVector<QString> *deps = NULL) {
        assert(!metrics.contains(metric.name()));
        metrics.insert(metric.name(), metric.clone());
        metricNames.append(metric.name());
        if (deps) {
            QVector<QString> *copy = new QVector<QString>;
            for (int i = 0; i < deps->size(); ++i) {
                assert(metrics.contains((*deps)[i]));
                copy->append((*deps)[i]);
            }
            dependencyMap.insert(metric.name(), copy);
        }
        return true;
    }

    const QVector<QString> &dependencies(const QString &name) const {
        assert(metrics.contains(name));
        QVector<QString> *result = dependencyMap.value(name);
        return result ? *result : noDeps;
    }
};

#endif // _GC_RideMetric_h

