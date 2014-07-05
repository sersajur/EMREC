#ifndef FUZZYNODE_H
#define FUZZYNODE_H
#include <QVector>

class FuzzyNode
{
public:
    FuzzyNode(int v_type_number = 1, int f_number = 1 , int mf_p_number = 3);
    void setInputs(QVector<qreal*> in){_inputs = in;}
    void setParams(QVector<QVector<QVector<qreal> > >& par){_fuzzy_set_params = par;}
    QVector<QVector<QVector<qreal> > > getParams()const{return _fuzzy_set_params;}
    QVector<QVector<qreal> > getParams(int i)const{return _fuzzy_set_params[i];}
    qreal Process(int fuzzy_id);
    qreal get_d_a(int feature_id, int fuzzy_id);
    qreal get_d_b(int feature_id, int fuzzy_id);
    qreal get_d_c(int feature_id, int fuzzy_id);
private:
    qreal _member_fun(qreal x, qreal a, qreal b, qreal c);
private:
    QVector<qreal*> _inputs;
    QVector<QVector<QVector<qreal> > > _fuzzy_set_params;

};

#endif // FUZZYNODE_H
