#ifndef CONFIGDIALOG_H
#define CONFIGDIALOG_H

#include <QDialog>
#include <QSettings>
#include "Pages.h"
#include "MainWindow.h"

class QListWidget;
class QListWidgetItem;
class QStackedWidget;
class Zones;

class ConfigDialog : public QDialog
{
    Q_OBJECT
    public:
        ConfigDialog(QDir home, Zones **zones);
        ~ConfigDialog();

    public slots:
        void changePage(QListWidgetItem *current, QListWidgetItem *previous);
        void save_Clicked();
        void back_Clicked();
        void forward_Clicked();
        void delete_Clicked();
	void calendarDateChanged();

    private:
        void createIcons();
        void calculateZones();
	void createNewRange();
	void moveCalendarToCurrentRange();

        ConfigurationPage *configPage;
        CyclistPage *cyclistPage;
        QPushButton *saveButton;
        QStackedWidget *pagesWidget;
	QPushButton *closeButton;
	QHBoxLayout *horizontalLayout;
	QHBoxLayout *buttonsLayout;
	QVBoxLayout *mainLayout;
        QListWidget *contentsWidget;

        QSettings *settings;
        QDir home;
        Zones **zones;
};

#endif
