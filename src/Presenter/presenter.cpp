#include "Presenter/presenter.h"
#include "Presenter/simplex.h"
#include <qmath.h>

#define PARAM_NUMBER 519
long long int Presenter::iteration = 0;

Presenter::Presenter()
{
}

Presenter::Presenter(iView* i_userInterface)
{
    QObject *userInterface = dynamic_cast<QObject*>(i_userInterface);

    connect(userInterface, SIGNAL(test_inputed(QVector<qreal>)),this, SLOT(testing(QVector<qreal>)));
    connect(userInterface, SIGNAL(train_sample_inputed(QVector<QPair<QVector<qreal>,QVector<qreal> > >& )),this, SLOT(training(QVector<QPair<QVector<qreal>,QVector<qreal> > >&)));
    connect(this, SIGNAL(result_returned(QVector<qreal>)), userInterface, SLOT(show_results(QVector<qreal>)));
    connect(this, SIGNAL(message_showing(QString)), userInterface, SLOT(show_message(QString)));
    connect(this, SIGNAL(message_hiding()), userInterface, SLOT(hide_message()));
    connect(this, SIGNAL(message_sending(QString)), userInterface, SLOT(show_dlg_message(QString)));
    ANFIS.Init();
    ANFIS.Deserialize("sources/weights.xml");
}

QVector<qreal> Presenter::_trunk(QVector<qreal> result)
{
    for(auto& it: result)
        if (it<0) it=0;
    else if(it>1) it=1;
    return result;
}

QVector<qreal> Presenter::_normed(QVector<qreal> features)
{
    QVector<qreal> result = features;
    qreal& Horizontal = features[13];
    qreal& Vertical = features[19];
    //eye_brows_size_params:
    result[0]  /= Vertical;
    result[1]  /= Horizontal;
    result[2]  /= Vertical;
    result[3]  /= Horizontal;
    //eyes_size_params:
    result[4]  /= Vertical;
    result[5]  /= Horizontal;
    result[6]  /= Vertical;
    result[7]  /= Horizontal;
    //nose_size_params:
    result[8]  /= Vertical;
    result[9]  /= Horizontal;
    //lips_size_params:
    result[10] /= Vertical;
    result[11] /= Horizontal;
    //distances:
    result[12] /= Horizontal;
    result[14] /= Vertical;
    result[15] /= Vertical;
    result[16] /= Vertical;
    result[17] /= Vertical;
    result[18] /= Vertical;
    return result;
}

void Presenter::testing(QVector<qreal> features)
{
    QVector<qreal> result;
    result = ANFIS.Check(_normed(features));
    emit result_returned(_trunk(result));
}

QString makeLog(int V/*, int N*//*, qreal eps*/, QVector<QString> em, QVector<int> n, QVector<qreal> e)
{
    QString log = QString("Об'єм вибірки: %1\n").arg(V);
    log += QString("Критерій зупинки: перехресна перевірка \n ");
    log += QString("------------------------------------\n");
    log += QString("Емоція    || Кількість епох || Ефективність \n");
    log += QString("%1 %2 %3\n").arg(em[0],-14).arg(n[0],-19).arg(e[0],-9,'f');
    log += QString("%1      %2 %3\n").arg(em[1],-14).arg(n[1],-19).arg(e[1],-9,'f');
    log += QString("%1        %2 %3\n").arg(em[2],-14).arg(n[2],-19).arg(e[2],-9,'f');
    log += QString("%1       %2 %3\n").arg(em[3],-14).arg(n[3],-19).arg(e[3],-9,'f');
    log += QString("%1        %2 %3\n").arg(em[4],-14).arg(n[4],-19).arg(e[4],-9,'f');
    return log;
}
qreal Presenter::_effectiveness(Emotions::Emotions em)
{
    qreal result = 0;
    for(auto& it: _checking_data)
    {
        ANFIS.setInput(it.first);
        result += qAbs(it.second[em]-ANFIS.Process(em))/_checking_data.size()/it.second[em];
    }
    return (1 - result);
}

void Presenter::training(QVector<QPair<QVector<qreal>, QVector<qreal> > > &sample)
{

    for (auto& row: sample)
        row.first = _normed(row.first);
    int k = qRound(sample.size()/qSqrt(PARAM_NUMBER*2));
    for(int i=0; i < sample.size() - k; i++)
        _training_data.append(sample[i]);
    for(int i = _training_data.size(); i < sample.size(); i++)
        _checking_data.append(sample[i]);
    qreal l_rate_cons = 0.2;
    qreal l_rate_prem_a = 0.2;
    qreal l_rate_prem_b = 0.2;
    qreal l_rate_prem_c = 0.2;
    int epoch_counter;
    qreal prev_effect;
    QVector<qreal> save_param;
    QVector<QString> str_status = {"здивування", "щастя", "сум", "відраза", "злість"};
    QVector<int> n_epoch;
    QVector<qreal> effectivenesses;
    for(int i = 0; i < str_status.size(); i++)
    {
        epoch_counter = 0;
        prev_effect = _effectiveness(Emotions::Emotions(i));
        while((prev_effect <= _effectiveness(Emotions::Emotions(i))) || !epoch_counter)
        {
            save_param = ANFIS.getParams();
            prev_effect = _effectiveness(Emotions::Emotions(i));
            message_showing("Засвоєння емоції: " + str_status[i] + ", " + QString::number(epoch_counter) + ", ефективність" + QString::number(prev_effect,'f'));
            for(auto& it: _training_data)
            {
                ANFIS.setInput(it.first);
                ANFIS.One_lesson(Emotions::Emotions(i), it.second[i], l_rate_cons, l_rate_prem_a, l_rate_prem_b, l_rate_prem_c);
            }
            epoch_counter++;
        }
        ANFIS.setParams(save_param);
        n_epoch.append(epoch_counter);
        effectivenesses.append(prev_effect);
    }
    emit message_hiding();
    emit message_sending(makeLog(_training_data.size()/*,max_epoch_number*//*,_eps*/,str_status,n_epoch,effectivenesses));
    ANFIS.Serialize("sources/weights.xml", 6);
}
