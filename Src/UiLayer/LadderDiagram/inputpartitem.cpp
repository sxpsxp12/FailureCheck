#include "inputpartitem.h"
#include "ui_inputpartitem.h"

#include "enumtypeheader.h"

InputPartItem::InputPartItem(const LadderDiagramClass &LadderDiagramInfo,
                             const int &partState, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::InputPartItem)
{
    ui->setupUi(this);

    m_LadderDiagramInfo = LadderDiagramInfo;
    m_partState = partState;
    m_flagSelect = false;

    ui->lbl_busId->setText(m_LadderDiagramInfo.getBusId());

    switch (partState)
    {
        case DISCONNECT:
            m_imageSuffix = "W";
            m_color = "white";
            break;
        case CONNECT:
            m_imageSuffix = "G";
            m_color = "#00fe00";
            break;
        default:
            break;
    }

    setStyleSheet(QString("\
                  QWidget#widget_part{\
                      image: url(:/LadderDiagram/Image/LadderDiagram/%1%2.png);\
                      border:none;\
                  }\
                  QLabel#lbl_busId{\
                      color:%3;\
                      font: 12px;\
                  }"\
                  ).arg( m_LadderDiagramInfo.getPartTypeStr(), m_imageSuffix, m_color));
}

InputPartItem::~InputPartItem()
{
    delete ui;
}

//设置选中的颜色
void InputPartItem::setSelectedItemBorderColor()
{
    m_flagSelect = true;
    ui->widget_part->setStyleSheet("\
                                   QWidget#widget_part{\
                                   border:1px solid yellow;\
                                } "\
                                );
}

void InputPartItem::resetSelectedItemBorderColor()
{
    m_flagSelect = false;
    ui->widget_part->setStyleSheet("\
                                   QWidget#widget_part{\
                                   border:none;\
                                }"\
                                );
}

void InputPartItem::setDataUpdateColor(int color)
{
    if (m_flagSelect)
    {
        switch (color)
        {
        case WHITE:
            m_color = "white";
            m_imageSuffix = "W";
            setStyleSheet(QString("\
                          QWidget#widget_part{\
                              image: url(:/LadderDiagram/Image/LadderDiagram/%1%2.png);\
                              border:none;\
                          }\
                          QLabel#lbl_busId{\
                              color:%3;\
                              font: 12px;\
                          }"\
                          ).arg( m_LadderDiagramInfo.getPartTypeStr(), m_imageSuffix, m_color));
            break;
        case GREEN:
            m_color = "#00fe00";
            m_imageSuffix = "G";
            setStyleSheet(QString("\
                        QWidget#widget_part{\
                            image: url(:/LadderDiagram/Image/LadderDiagram/%1%2.png);\
                            border:none;\
                        }\
                        QLabel#lbl_busId{\
                            color:%3;\
                            font: 12px;\
                        }"\
                        ).arg( m_LadderDiagramInfo.getPartTypeStr(), m_imageSuffix, m_color));
            break;
        default:
            break;
        }
    } else
    {
        switch (color)
        {
        case WHITE:
            m_color = "white";
            m_imageSuffix = "W";
            setStyleSheet(QString("\
                          QWidget#widget_part{\
                              image: url(:/LadderDiagram/Image/LadderDiagram/%1%2.png);\
                              border:none;\
                          }\
                          QLabel#lbl_busId{\
                              color:%3;\
                              font: 12px;\
                          } "\
                          ).arg( m_LadderDiagramInfo.getPartTypeStr(), m_imageSuffix, m_color));
            break;
        case GREEN:
            m_color = "#00fe00";
            m_imageSuffix = "G";
            setStyleSheet(QString("\
                        QWidget#widget_part{\
                            image: url(:/LadderDiagram/Image/LadderDiagram/%1%2.png);\
                            border:none;\
                        }\
                        QLabel#lbl_busId{\
                            color:%3;\
                            font: 12px;\
                        }"\
                        ).arg( m_LadderDiagramInfo.getPartTypeStr(), m_imageSuffix, m_color));
            break;
        default:
            break;
        }
    }
}
