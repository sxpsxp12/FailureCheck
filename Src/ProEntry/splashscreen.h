#ifndef SPLASHSCREEN_H
#define SPLASHSCREEN_H

#include <QWidget>
#include "enumtypeheader.h"
#include "requestdatathread.h"
#include "guicontainer.h"
#include <QString>
#include <QDebug>
#include "voiceplaythread.h"

namespace Ui {
class SplashScreen;
}

class SplashScreen : public QWidget
{
    Q_OBJECT
    
public:
    explicit SplashScreen(QWidget *parent = 0);
    ~SplashScreen();
private slots:
    void slot_showMainWidget();
private:
    Ui::SplashScreen *ui;
    GuiContainer *m_guiContainer;
    RequestDataThread *m_requestDataThread;
    VoicePlayThread *m_voicePlayThread;
};

#endif // SPLASHSCREEN_H
