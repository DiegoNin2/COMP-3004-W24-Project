#ifndef SESSION_H
#define SESSION_H

#include <QObject>
#include <QDebug>
#include <QDateTime>
#include <QVector>
#include <random>

class Session : public QObject
{
    Q_OBJECT
public:
    explicit Session(int id, QObject *parent = nullptr);
    int getSessionId();
    QString getSessionStartDateTime();

    void receiveEEGSignal(QString);
    double calculateBaseline();
    void applyTreatment(double);
    void calculateOverallBaselines(QString);

public slots:
    void startSession();
    void pauseSession();
    void stopSession();

signals:
    void sessionStarted();
    void sessionPaused();
    void sessionStopped();


private:
     QDateTime sessionDateTime;
    int sessionId;
    QVector<double> baselines;
    QVector<double> frequencies;
    QVector<double> amplitudes;
    double startBaseline;
    double endBaseline;

};

#endif // SESSION_H
