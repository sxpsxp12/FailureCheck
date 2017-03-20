#include "splashscreen.h"
#include "ui_splashscreen.h"
#include <QTimer>
#include <QDesktopWidget>
#include <QPixmap>

SplashScreen::SplashScreen(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SplashScreen)
{
    ui->setupUi(this);
    ui->label_splashscreen->setPixmap(QPixmap(QString(":/SplashScreen/Image/SplashScreen/background.png")));

    m_guiContainer = NULL;
    m_requestDataThread = new RequestDataThread;
    m_voicePlayThread = new VoicePlayThread;
    QTimer::singleShot(STARTUP_TIME,this,SLOT(slot_showMainWidget()));
}

SplashScreen::~SplashScreen()
{
    delete ui;

    if(m_guiContainer != NULL)
    {
        delete m_guiContainer;
    }

    if(m_requestDataThread != NULL)
    {
        delete m_requestDataThread;
    }
    if(m_voicePlayThread != NULL)
    {
        delete m_voicePlayThread;
        m_voicePlayThread = NULL;
    }
}

void SplashScreen::slot_showMainWidget()
{
    if (m_guiContainer == NULL)
    {
        m_guiContainer = new GuiContainer(m_requestDataThread);
        m_guiContainer->move((QApplication::desktop()->width() - m_guiContainer->width())/2,(QApplication::desktop()->height() - m_guiContainer->height())/2);
        m_guiContainer->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowMinMaxButtonsHint);

        m_voicePlayThread->start();
        m_requestDataThread->start();

        this->hide();
        m_guiContainer->show();
    }
}
