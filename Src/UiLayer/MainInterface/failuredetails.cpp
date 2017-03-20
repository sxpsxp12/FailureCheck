#include "failuredetails.h"
#include "ui_failuredetails.h"

#include <QDebug>
#include "enumtypeheader.h"

FailureDetails::FailureDetails(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FailureDetails)
{
    ui->setupUi(this);
}

FailureDetails::~FailureDetails()
{
    delete ui;
}

void FailureDetails::slot_receiveImageName(QString imageName)
{
//    qDebug() << QString("%1faultDetail/%2.png").arg(IMAGEPATH,imageName);
    ui->label_failureDetails->setPixmap(QPixmap(QString("%1faultDetail/%2.png").arg(IMAGEPATH,imageName)));
}
