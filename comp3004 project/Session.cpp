#include "Session.h"

Session::Session(int id, QObject *parent)
    : QObject{parent}
{
    sessionId = id;

}

// When a session has been started, paused and stopped, there will be a signal being emitted and will eventually get caught by the progress handler in mainwindow class
void Session::startSession() {
    qDebug() << "Session started.";
    emit sessionStarted();
}

void Session::pauseSession() {
    qDebug() << "Session paused.";
    emit sessionPaused();

}

void Session::stopSession(){
    qDebug()<<"Session stopped.";
    emit sessionStopped();
}

// Returning the session id, each session will be givens its own unique id when it's created
int Session::getSessionId(){
    return sessionId;
}

// Returing the session start date and time
QString Session::getSessionStartDateTime(){
    // Setting the date and time for whenever a session is created
    sessionDateTime = QDateTime::currentDateTime();
    return sessionDateTime.toString("yyyy-MM-dd HH:mm:ss");
}

//randomly generate stuff for the amplitude and frequency vectors based on the band
//only generate a max of 3 of each
void Session::receiveEEGSignal(QString band) {
    std::random_device rand;
    std::mt19937 gen(rand());

    //this is the gamma wave vals
    double maxFreq = 140.0;
    double minFreq = 25.0;
    double maxAmp = 10.0;
    double minAmp = 1.0;

    if (band == "alpha") {
        maxFreq = 12.0;
        minFreq = 8.0;
        maxAmp = 200.0;
        minAmp = 20.0;

        std::uniform_real_distribution<> freqDist(minFreq, maxFreq);
        std::uniform_real_distribution<> ampDist(minAmp, maxAmp);

        frequencies.push_back(freqDist(gen));
        amplitudes.push_back(ampDist(gen));

    } else if (band == "beta") {
        maxFreq = 30.0;
        minFreq = 12.0;
        maxAmp = 30.0;
        minAmp = 5.0;

        std::uniform_real_distribution<> freqDist(minFreq, maxFreq);
        std::uniform_real_distribution<> ampDist(minAmp, maxAmp);

        frequencies.push_back(freqDist(gen));
        amplitudes.push_back(ampDist(gen));

    } else if (band == "delta") {
        maxFreq = 4.0;
        minFreq = 0.5;
        maxAmp = 200.0;
        minAmp = 20.0;

        std::uniform_real_distribution<> freqDist(minFreq, maxFreq);
        std::uniform_real_distribution<> ampDist(minAmp, maxAmp);

        frequencies.push_back(freqDist(gen));
        amplitudes.push_back(ampDist(gen));

    } else if (band == "theta") {
        maxFreq = 8.0;
        minFreq = 4.0;
        maxAmp = 200.0;
        minAmp = 20.0;

        std::uniform_real_distribution<> freqDist(minFreq, maxFreq);
        std::uniform_real_distribution<> ampDist(minAmp, maxAmp);

        frequencies.push_back(freqDist(gen));
        amplitudes.push_back(ampDist(gen));

    } else {
        std::uniform_real_distribution<> freqDist(minFreq, maxFreq);
        std::uniform_real_distribution<> ampDist(minAmp, maxAmp);

        frequencies.push_back(freqDist(gen));
        amplitudes.push_back(ampDist(gen));
    }
}

//get baseline frequency via getting the dominate frequency
//update with error handling later (if necessary)
double Session::calculateBaseline() {
    //variables for dominate frequency formula
    double numerator = 0;
    double denominator = 0;

    for (int i = 0; i < frequencies.size(); i++) {
        numerator += frequencies[i] * (amplitudes[i] * amplitudes[i]);
        denominator += amplitudes[i] * amplitudes[i];
    }

    return numerator/denominator;
}

//apply the treatment (offset)
void Session::applyTreatment(double currentBaseline) {
    double currentFrequency = currentBaseline;

    //should maybe make this better idk
    for (double t = 0; t < 1.0; t += (1.0/16.0)) {
       currentFrequency += 5.0;

       qDebug() << "applying treatment";

       //i should apply a wait here but idk how to do that really
    }
}

//get the overall baselines
void Session::calculateOverallBaselines(QString band) {
    //start baselines calculation
    double sum = 0;

    for (int i = 0; i < baselines.size(); i++) {
        sum += baselines[i];
    }

    startBaseline = sum / baselines.size();

    //end baselines calculation
    frequencies.clear();
    amplitudes.clear();
    baselines.clear();

    for (int i = 0; i < 3; i++) {
        this->receiveEEGSignal(band);
    }

    baselines.push_back(this->calculateBaseline());

    sum = 0;

    for (int i = 0; i < baselines.size(); i++) {
        sum += baselines[i];
    }

    endBaseline = sum / baselines.size();
}
