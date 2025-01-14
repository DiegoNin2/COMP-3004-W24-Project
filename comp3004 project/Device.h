#ifndef DEVICE_H
#define DEVICE_H

#include <QObject>
#include <QTimer>
#include <QDebug>
#include "Menu.h"

class Device : public QObject
{
    Q_OBJECT
public:
    explicit Device(Menu* menu, QObject *parent = nullptr);


public slots:
    void powerOn();
    void powerOff();
    void displayMenu();
    double getBatteryLevel();
    void setBatteryLevel(double);
    Menu* getMenu();

    // Methods to call methods in its subclasses for better flow control as a mediator
    void handleNewSessionInMenu();
    void handleSessionLogInMenu();
    QString handleDateTimeOptionInMenu();
    void handleStartCurrentSession();
    void handlePauseCurrentSession();
    void handleStopCurrentSession();


private:
    int power = 0;
    double battery;
    Menu* userMenu;

};

#endif // DEVICE_H
