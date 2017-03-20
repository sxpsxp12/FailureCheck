#include "voiceplaythread.h"
#include "QDebug"
#include "cbaseoperate.h"
#include "enumtypeheader.h"
QStringList VoicePlayThread::m_playContentList;

VoicePlayThread::VoicePlayThread(QObject *parent) :
    QThread(parent)
{
    m_playContentList.clear();
}

VoicePlayThread::~VoicePlayThread()
{
    if(m_timerVoicePlay != NULL)
    {
        delete m_timerVoicePlay;
        m_timerVoicePlay = NULL;
    }

}

void VoicePlayThread::run()
{
    m_timerVoicePlay = new QTimer;
    m_timerVoicePlay->start(VOICE_PLAY_TIME);
    connect(m_timerVoicePlay, SIGNAL(timeout()),this, SLOT(slot_voicePlay()), Qt::DirectConnection);
    this->exec();
}

//语音播报
void VoicePlayThread::slot_voicePlay()
{
    QString content;
    //播报内容列表
    if(m_playContentList.count() != 0)
    {
        content = m_playContentList.at(0);
        content = content.append("[v0]");

        if(content.contains("KM"))
        {
            content.prepend("[n1]");
        }

        CBaseOperate::voicePlay(content);
        m_playContentList.removeFirst();
    }
}
