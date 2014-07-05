#ifndef INIT_DATA_H
#define INIT_DATA_H
#include <QVector>
#include <qmath.h>

//L1 nod's function:
qreal member_fun(qreal in, QVector<qreal> par)
{
    if(!par.size()){
        return -1;
    }

    return 1/(1 + qPow(qAbs((in - par[2])/par[0]),2*par[1]));
}
//qreal get_d_a()
//L1 nod's weights:
QVector<qreal> L1_node_weights()
{
    QVector<qreal> res;
    res.resize(48*3);

    for(int i = 0; i < res.size(); i++)
        res[i] = 1 + 0.01*(i/3);
    return res;
}
//L2
QVector<qreal> L2_node_weights()
{
    QVector<qreal> res;
    res.resize(9*5 + 6*5 + 3 + 4*5);

    for(int i = 0; i < res.size(); i++)
        res[i] = 1 + 0.01*i;
    return res;
}
#endif // INIT_DATA_H
