#include "outputpartitem.h"
#include "ui_outputpartitem.h"

#include "enumtypeheader.h"

OutputPartItem::OutputPartItem(const LadderDiagramClass &LadderDiagramInfo,
                               const int &partState, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::OutputPartItem)
{
    ui->setupUi(this);

    m_LadderDiagramInfo = LadderDiagramInfo;
    m_partState = partState;
    m_flagSelect = false;

    ui->lbl_busId->setText(m_LadderDiagramInfo.getBusId());
    ui->lbl_meaning->setText(m_LadderDiagramInfo.getMeaning());

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

    this->setStyleSheet(QString(" \
                          QWidget#widget_part{\
                            image: url(:/LadderDiagram/Image/LadderDiagram/%1%2.png);\
                            border:none;\
                          }\
                          QWidget#widget{\
                            border:none;\
                          }\
                          QLabel#lbl_busId, #lbl_meaning{\
                            color:%3;\
                            font: 12px;\
                          }" \
                          ).arg( m_LadderDiagramInfo.getPartTypeStr(), m_imageSuffix, m_color ));
}

OutputPartItem::~OutputPartItem()
{
    delete ui;
}

void OutputPartItem::setSelectedItemBorderColor()
{
    m_flagSelect = true;
    ui->widget->setStyleSheet("\
                              QWidget#widget{\
                              border:1px solid yellow;\
                               }");
}

void OutputPartItem::resetSelectedItemBorderColor()
{
    m_flagSelect = false;
    ui->widget->setStyleSheet("\
                               QWidget#widget{\
                               border:none;\
                                }"\
                              );
}

void OutputPartItem::setDataUpdateColor(int color)
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
                          QLabel#lbl_busId, #lbl_meaning{\
                            color:%3;\
                            font: 12px;}"\
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
                          QLabel#lbl_busId, #lbl_meaning{\
                            color:%3;\
                            font: 12px;}" \
                          ).arg( m_LadderDiagramInfo.getPartTypeStr(), m_imageSuffix, m_color));
            break;
        default:
            break;
        }
    }else
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
                          QWidget#widget{\
                            border:none;\
                          }\
                          QLabel#lbl_busId, #lbl_meaning{\
                            color:%3;\
                            font: 12px;}"\
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
                          QWidget#widget{\
                            border:none;\
                          }\
                          QLabel#lbl_busId, #lbl_meaning{\
                            color:%3;\
                            font: 12px;}"\
                          ).arg( m_LadderDiagramInfo.getPartTypeStr(), m_imageSuffix, m_color));
            break;
        default:
            break;
        }
    }
}
