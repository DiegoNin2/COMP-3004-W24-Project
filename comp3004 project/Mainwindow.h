#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>
#include "Device.h"
#include "Menu.h"
#include "Session.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();


    void drainBattery();

    void turnOn();
    void turnOff();

    void activeBluelight();
    void activeGreenlight();
    void activeRedlight();
    void inactiveBluelight();
    void inactiveGreenlight();
    void inactiveRedlight();

private slots:
    void moveUpInMenu();
    void moveDownInMenu();
    void handleMenuSelection();
    void handleReturnToMenu();
    void handleStartButton();
    void handleStopButton();
    void handlePauseButton();
    void rechargeBattery();
    void changeBatteryIndicator(double);
    void updateTimer();

private:
    Ui::MainWindow *ui;
    Menu *menu;
    Device *device;
    Session *session;
    QTimer *timer;
    int remainingTime;
};
#endif // MAINWINDOW_H
