#ifndef VOICEPLAYTHREAD_H
#define VOICEPLAYTHREAD_H

#include <QThread>
#include <QTimer>

class VoicePlayThread : public QThread
{
    Q_OBJECT
public:
    explicit VoicePlayThread(QObject *parent = 0);
    ~VoicePlayThread();
    static QStringList m_playContentList;
protected:
    void run();
private slots:
    void slot_voicePlay();
private:
    QTimer *m_timerVoicePlay;       //语音播报定时器
};

#endif // VOICEPLAYTHREAD_H
