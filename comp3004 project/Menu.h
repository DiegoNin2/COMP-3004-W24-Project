#ifndef MENU_H
#define MENU_H

#include <QObject>
#include <QDebug>
#include <QString>
#include "Session.h"
#include <QDateTime>

class Menu : public QObject
{
    Q_OBJECT
public:
    explicit Menu(QObject *parent = nullptr);
    QString getDateTimeLog();
    int getNumOfSession();
public slots:
    void handleNewSession();
    void handleSessionLog();
    QString handleDateTimeOption();
    void startCurrentSession();
    void pauseCurrentSession();
    void stopCurrentSession();


private:
    std::vector<Session*> sessions;
    std::vector<QString> sessionDateTimeLogs;
};

#endif // MENU_H
