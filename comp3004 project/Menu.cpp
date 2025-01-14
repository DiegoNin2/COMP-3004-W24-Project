#include "Menu.h"

Menu::Menu(QObject *parent)
    : QObject{parent}
{

}

// When the new session started, signal will be emitted
void Menu::handleNewSession() {
    qDebug() << "Starting a new session...";
    // Pushing a new session to the sessions vector whenever a new session is started
    Session *newSession = new Session(sessions.size() + 1, this);
    sessions.push_back(newSession);

}

// Indicate that a session log will be displayed
void Menu::handleSessionLog() {
    qDebug() << "Displaying session log...";

}

// Indicate that the session date and time will be displayed
QString Menu::handleDateTimeOption() {
    qDebug() << "Setting date and time...";

    // Whenever the user select the date time option,  return the current date and time and display to the user
    QDateTime dateTime = QDateTime::currentDateTime();
    return dateTime.toString("yyyy-MM-dd HH:mm:ss");
}


// Returning the Date and time log of the current sessiom, return empty string when the session array is empty (no session started)
QString Menu::getDateTimeLog() {
    if (!sessions.empty()) {
        // Since new sessions are pushed back to the vector, so by accessing the back of the vector will get us the current session
        return sessions.back()->getSessionStartDateTime();
    } else {
        return QString(); // Return an empty QString if the vector is empty
    }
}

// Return the number of started session, used to determine when to disable the start, pause and stop button on GUI to avoid out of range access
int Menu::getNumOfSession(){
    return sessions.size();
}

// Access the currrent session (at the back of sessions vector) to control the progress of the current session
void Menu::startCurrentSession(){
    sessions.back()->startSession();
}

void Menu::pauseCurrentSession(){
    sessions.back()->pauseSession();
}

void Menu::stopCurrentSession(){
    sessions.back()->stopSession();
}
