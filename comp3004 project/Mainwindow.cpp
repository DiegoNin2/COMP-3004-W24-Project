#include "Mainwindow.h"
#include "ui_Mainwindow.h"
#include <QDateTime>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)

{
    ui->setupUi(this);

    menu = new Menu(this);
    device = new Device(menu, this);

    // Several ui elements will be visible on screen initially, eg. the session progress page, will be enable sequentially
    ui->progressBar->setVisible(false);
    ui->sessionLog->setVisible(false);
    ui->downButton->setEnabled(false);
    ui->okButton->setEnabled(false);
    ui->upButton->setEnabled(false);
    ui->returnToMenuButton->setEnabled(false);
    ui->poweroffButton->setEnabled(false);
    ui->pauseButton->setEnabled(false);
    ui->startButton->setEnabled(false);
    ui->stopButton->setEnabled(false);

    //Timebar and time setting
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MainWindow::updateTimer);
    remainingTime = 29;
    ui->progressBar->setMaximum(29);
    ui->progressBar->setValue(0);

    // Connecting button to corresponding methods
    connect(ui-> upButton, SIGNAL(clicked()), SLOT(moveUpInMenu()));
    connect(ui-> downButton, SIGNAL(clicked()), SLOT(moveDownInMenu()));
    connect(ui-> okButton, SIGNAL(clicked()), SLOT(handleMenuSelection()));
    connect(ui->returnToMenuButton, SIGNAL(clicked()), SLOT(handleReturnToMenu()));
    connect(ui->rechargeButton, SIGNAL(clicked()), SLOT(rechargeBattery()));
    connect(ui->stopButton, SIGNAL(clicked()), SLOT(handleStopButton()));
    connect(ui->startButton, SIGNAL(clicked()), SLOT(handleStartButton()));
    connect(ui->pauseButton, SIGNAL(clicked()), SLOT(handlePauseButton()));
    connect(ui->poweronButton, &QPushButton::clicked, this, &MainWindow::turnOn);
    connect(ui->poweroffButton, &QPushButton::clicked, this, &MainWindow::turnOff);

}

MainWindow::~MainWindow()
{
    delete ui;
}

// Moving the selection up  or down for the user in menu page
void MainWindow::moveUpInMenu(){
    int nextRow = ui->menu->currentRow() - 1;
    if(nextRow < 0){
        nextRow = 0;
    }

    ui->menu->setCurrentRow(nextRow);
}

void MainWindow::moveDownInMenu(){
    int nextRow = ui->menu->currentRow() + 1;
    if(nextRow > 2){
        nextRow = 2;
    }

    ui->menu->setCurrentRow(nextRow);
}

// Handling the menu selection of user
void MainWindow::handleMenuSelection(){

    if (ui->menu->currentItem() == nullptr) {
        return;
    }

    QString currRow = ui->menu->currentItem()->text();
    // If user selection the option for new session
    if(currRow == "NEW SESSION"){
        device->handleNewSessionInMenu();
        ui->progressBar->setVisible(true);
        ui->menu->setVisible(false);
        inactiveBluelight();
        inactiveGreenlight();
        inactiveRedlight();
        // Adding the start date & time to the sessiion log whenever a new session is started
        ui->sessionLog->addItem(menu->getDateTimeLog());

    // If the user selected session log
    }else if (currRow == "SESSION LOG"){
        device->handleSessionLogInMenu();
        ui->menu->setVisible(false);
        ui->sessionLog->setVisible(true);

    }else{
        Menu* currMenu = device->getMenu(); // variable to hold the menu from our device class
        // If the user selected date and time option
        // Output the current date time
         ui->textEdit->setText(currMenu->handleDateTimeOption());
    }
}

// When the .. button is pressed, the device will direct the user back to menu, and all the interface that the user is directed  to will be temporarily disabled
void MainWindow::handleReturnToMenu(){
    ui->progressBar->setVisible(false);
    ui->sessionLog->setVisible(false);
    ui->menu->setVisible(true);

}

// Handling the start, stop and pause button, whenever these button are press, there will be signal sent by each of corresponding method in session class
// The wave form and session progress indicators (timer and progress bar)and a global timer will be triggered/paused /stopped depend on what signal they receive
// Buttons are only accesible when there are more than or equalss to 1 session in the session vector
void MainWindow::handleStartButton(){
    if(device->getMenu()->getNumOfSession() > 0){
        device->handleStartCurrentSession();
        inactiveBluelight();
        activeGreenlight();
        inactiveRedlight();
        remainingTime = 29;
        ui->progressBar->setMaximum(29);
        timer->start(1000);
    }else{
        qDebug()<<"No currently ongoingg session.";
    }

}

void MainWindow::handleStopButton(){
    if(device->getMenu()->getNumOfSession() > 0){
      device->handleStopCurrentSession();
      inactiveBluelight();
      inactiveGreenlight();
      activeRedlight();
      timer->stop();
      remainingTime = 29;
      ui->progressBar->setValue(0);
    }else{
      qDebug()<<"No currently ongoing session.";
    }

}

void MainWindow::handlePauseButton(){
    if(device->getMenu()->getNumOfSession()> 0){
       device->handlePauseCurrentSession();
       activeBluelight();
       inactiveGreenlight();
       inactiveRedlight();
       timer->stop();
    }else{

     qDebug()<<"No currently ongoingg session.";
    }
}


void MainWindow::changeBatteryIndicator(double newLevel) {
    if (newLevel >= 0.0 && newLevel <= 100.0) {
        if (newLevel == 0.0) {
            device->setBatteryLevel(0);
            // turn off device here
        } else {
            device->setBatteryLevel(newLevel);
        }

        int newLevelInt = int (newLevel);
        ui->batteryIndicator->setValue(newLevelInt);

        QString highHealth = "QProgressBar { selection-background-color: rgb(38, 162, 105) }";
        QString mediumHealth = "QProgressBar { selection-background-color: rgb(229, 165, 10) }";
        QString lowHealth = "QProgressBar { selection-background-color: rgb(165, 29, 45) }";

        if (newLevelInt >= 50) {
            ui->batteryIndicator->setStyleSheet(highHealth);
        } else if (newLevelInt >= 20) {
            ui->batteryIndicator->setStyleSheet(mediumHealth);
        } else {
            ui->batteryIndicator->setStyleSheet(lowHealth);
        }
    }
}

void MainWindow::rechargeBattery() {
    //qDebug() << "recharging battery";
    changeBatteryIndicator(100.0);
}

//this should be called whenever we update the timer (globally), already called
void MainWindow::drainBattery() {
    double newBatteryLevel = device->getBatteryLevel() - 1;
    changeBatteryIndicator(newBatteryLevel);
    device->setBatteryLevel(newBatteryLevel);
    if (newBatteryLevel <= 0) {
        handleStopButton();
    }
}

//power on/off functionality here
//this might work but idk it depends on what is set up on the gui
void MainWindow::turnOn() {
    //ui->progressBar->setVisible(true);
    //ui->timer->setVisible(true);
    //ui->sessionLog->setVisible(true);

    ui->downButton->setEnabled(true);
    ui->okButton->setEnabled(true);
    ui->pauseButton->setEnabled(true);
    ui->startButton->setEnabled(true);
    ui->stopButton->setEnabled(true);
    ui->upButton->setEnabled(true);
    ui->returnToMenuButton->setEnabled(true);
    ui->poweroffButton->setEnabled(true);
    activeBluelight();
    activeGreenlight();
    activeRedlight();
}

void MainWindow::turnOff() {
    ui->downButton->setEnabled(false);
    ui->okButton->setEnabled(false);
    ui->pauseButton->setEnabled(false);
    ui->startButton->setEnabled(false);
    ui->stopButton->setEnabled(false);
    ui->upButton->setEnabled(false);
    ui->returnToMenuButton->setEnabled(false);
    ui->poweroffButton->setEnabled(false);
    handleStopButton();
    inactiveBluelight();
    inactiveGreenlight();
    inactiveRedlight();
}

//Light control function part
void MainWindow::activeBluelight() {
    ui->contactLight->setStyleSheet("background-color: blue;");
}

void MainWindow::activeGreenlight() {
    ui->treatLight->setStyleSheet("background-color: green;");
}

void MainWindow::activeRedlight() {
    ui->contactLostLight->setStyleSheet("background-color: red;");
}

void MainWindow::inactiveBluelight() {
    ui->contactLight->setStyleSheet("background-color: grey;");
}

void MainWindow::inactiveGreenlight() {
    ui->treatLight->setStyleSheet("background-color: grey;");
}

void MainWindow::inactiveRedlight() {
    ui->contactLostLight->setStyleSheet("background-color: grey;");
}

//Time set function
void MainWindow::updateTimer() {
    if (remainingTime > 0) {
        remainingTime--;
        int progressValue = 29 - remainingTime;
        ui->progressBar->setValue(progressValue);
        drainBattery();
    } else {
        timer->stop();
        ui->progressBar->setValue(29);
    }
}
