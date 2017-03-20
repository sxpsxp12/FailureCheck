#include "custommsgbox.h"
#include "ui_custommsgbox.h"
#include <QApplication>
#include <QDesktopWidget>

CustomMsgBox::CustomMsgBox(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CustomMsgBox)
{
    ui->setupUi(this);

    setFixedSize(220,120);
    move((QApplication::desktop()->width() - this->width())/2,
         (QApplication::desktop()->height() - this->height())/2);
    setWindowFlags(Qt::WindowMinMaxButtonsHint|Qt::WindowCloseButtonHint);
}

CustomMsgBox::~CustomMsgBox()
{
    delete ui;
}

void CustomMsgBox::setLabelContent(const QString &content)
{
    ui->textBrowser_content->setText(content);
}

void CustomMsgBox::setTitle(const QString &title)
{
    setWindowTitle(title);
}

void CustomMsgBox::setPushButtonEnable(const bool &okPbtEnable, const bool &canclePbtEnable)
{
    ui->pbt_ok->setEnabled(okPbtEnable);
    ui->pbt_cancle->setEnabled(canclePbtEnable);
}

void CustomMsgBox::setPushButtonName(const QString &okPbt, const QString &canclePbt)
{
    ui->pbt_ok->setText(okPbt);
    ui->pbt_cancle->setText(canclePbt);
}

void CustomMsgBox::setOkPushButtonHide(const bool &isHidden)
{
    ui->pbt_ok->setHidden(isHidden);
}

int CustomMsgBox::getOkFunctionFlag() const
{
    return m_okFunctionFlag;
}

void CustomMsgBox::setOkFunctionFlag(const int &okFunctionFlag)
{
    m_okFunctionFlag = okFunctionFlag;
}

int CustomMsgBox::getCancleFunctionFlag() const
{
    return m_cancleFunctionFlag;
}

void CustomMsgBox::setCancleFunctionFlag(const int &cancleFunctionFlag)
{
    m_cancleFunctionFlag = cancleFunctionFlag;
}

void CustomMsgBox::on_pbt_ok_clicked()
{
    emit signal_sendOkFunctionFlag(m_okFunctionFlag);
}

void CustomMsgBox::on_pbt_cancle_clicked()
{
    emit signal_sendCancleFunctionFlag(m_cancleFunctionFlag);
}

void CustomMsgBox::setCanclePushButtonHide(const bool &isHidden)
{
    ui->pbt_cancle->setHidden(isHidden);
}

