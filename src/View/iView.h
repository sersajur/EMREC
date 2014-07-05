#ifndef IVIEW_H
#define IVIEW_H
#include <QVector>
#include <QPair>

class iView
{

public:
    virtual void test_inputed(QVector<qreal> features)=0;
    virtual void train_sample_inputed(QVector<QPair<QVector<qreal>, QVector<qreal> > >& sample)=0;
public:
    virtual void show_results(const QVector<qreal>& results)=0;
    virtual void show_message(QString msg)=0;
    virtual void hide_message()=0;
    virtual void show_dlg_message(QString)=0;
};

#endif // IVIEW_H
