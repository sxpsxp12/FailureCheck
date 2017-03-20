#ifndef FAILUREDETAILS_H
#define FAILUREDETAILS_H

#include <QWidget>
#include <QString>

namespace Ui {
class FailureDetails;
}

class FailureDetails : public QWidget
{
    Q_OBJECT
    
public:
    explicit FailureDetails(QWidget *parent = 0);
    ~FailureDetails();
    
private slots:
        void slot_receiveImageName(QString imageName);

private:
    Ui::FailureDetails *ui;
};

#endif // FAILUREDETAILS_H
