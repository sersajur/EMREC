#ifndef PRESENTER_H
#define PRESENTER_H
#include <QObject>
#include "View/iView.h"
#include "Network/network.h"

//#include <vector>

class Presenter: public QObject
{
    Q_OBJECT
public:
    Presenter();
    Presenter(iView* userInterface);
signals:
    void result_returned(QVector<qreal>);
    void message_showing(QString str);
    void message_sending(QString);
    void message_hiding();
private slots:
    //from view
    void testing(QVector<qreal> features);
    void training(QVector<QPair<QVector<qreal>, QVector<qreal> > >& sample);
private:
    QVector<qreal> _normed(QVector<qreal>);
    QVector<qreal> _trunk(QVector<qreal>);
    qreal _effectiveness(Emotions::Emotions em);
    QVector<QPair<QVector<qreal>, QVector<qreal> > > _training_data;
    QVector<QPair<QVector<qreal>, QVector<qreal> > > _checking_data;
    Network ANFIS;
    static long long int iteration;//
};



#endif // PRESENTER_H
