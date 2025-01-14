#include "Device.h"

Device::Device(Menu* menu, QObject *parent)
    : QObject{parent}, userMenu{menu}
{
    power = 0;
}
void Device::powerOn() {
    qDebug() << "Device powered on.";
}

void Device::powerOff() {
    qDebug() << "Device powered off.";
}


void Device::displayMenu() {
    qDebug() << "Menu displayed.";
}

// All the methods below is just methods that device as a mother class calling method in its subclass to manage the control flow as a mediator
void Device::handleNewSessionInMenu(){
    userMenu->handleNewSession();
}

void Device::handleSessionLogInMenu(){
   userMenu->handleSessionLog();
}

QString Device::handleDateTimeOptionInMenu(){
    return userMenu->handleDateTimeOption();
}

void Device::handleStartCurrentSession(){
    userMenu->startCurrentSession();
}

void Device::handlePauseCurrentSession(){
    userMenu->pauseCurrentSession();
}

void Device::handleStopCurrentSession(){
    userMenu->stopCurrentSession();
}


double Device::getBatteryLevel() { return battery; }

void Device::setBatteryLevel(double newLevel) { battery = newLevel; }

Menu* Device::getMenu() {return this->userMenu;}


