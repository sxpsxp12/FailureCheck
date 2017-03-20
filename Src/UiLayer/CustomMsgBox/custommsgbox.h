#ifndef CUSTOMMSGBOX_H
#define CUSTOMMSGBOX_H

#include <QWidget>

namespace Ui {
class CustomMsgBox;
}

class CustomMsgBox : public QWidget
{
    Q_OBJECT
    
public:
    explicit CustomMsgBox(QWidget *parent = 0);
    ~CustomMsgBox();

    void setLabelContent(const QString &content);
    
    void setTitle(const QString &title);
    void setPushButtonEnable(const bool &okPbtEnable,const bool &canclePbtEnable);
    void setPushButtonName(const QString &okPbt,const QString &canclePbt);

    void setOkPushButtonHide(const bool &isHidden);
    void setCanclePushButtonHide(const bool &isHidden);

    int getOkFunctionFlag() const;
    void setOkFunctionFlag(const int &okFunctionFlag);

    int getCancleFunctionFlag() const;
    void setCancleFunctionFlag(const int &cancleFunctionFlag);
private slots:
    void on_pbt_ok_clicked();

    void on_pbt_cancle_clicked();
signals:
    void signal_sendOkFunctionFlag(const int okFunctionFlag);
    void signal_sendCancleFunctionFlag(const int cancleFunctionFlag);
private:
    Ui::CustomMsgBox *ui;
    int m_okFunctionFlag;
    int m_cancleFunctionFlag;
};

#endif // CUSTOMMSGBOX_H
