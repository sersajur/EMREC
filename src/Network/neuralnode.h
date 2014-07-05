#ifndef NEURALNODE_H
#define NEURALNODE_H

#include<QVector>
#include <QObject>

class NeuralNode
{
public:
    NeuralNode();
    void setInputs(const QVector<qreal*>& inputs){_inputs = inputs; _params.resize(inputs.size()+1);}
    QVector<qreal> getInputs()const;
    void setParams(const QVector<qreal>& params){_params = params;}
    QVector<qreal> getParams()const;
    qreal getV(int i)const {return _w*((i<_inputs.size())?(*_inputs[i]):1.0);}
    qreal Process(qreal w);
    qreal getLinComb() const;
private:
    qreal _w;
    QVector<qreal*> _inputs;
    QVector<qreal> _params;
};


#endif // NEURALNODE_H
