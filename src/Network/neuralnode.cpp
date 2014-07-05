#include "neuralnode.h"

NeuralNode::NeuralNode()
{
}

qreal NeuralNode::Process(qreal w)
{
    _w = w;
    qreal result = _params.last();
    for(int i = 0; i < _inputs.size(); i++)
        result += (*(_inputs[i])*(_params[i]));
    return result*w;
}

qreal NeuralNode::getLinComb() const
{
    qreal result = _params[_inputs.size()];
    for(int i = 0; i < _inputs.size(); i++)
        result += *(_inputs[i])*_params[i];
    return result;
}

QVector<qreal> NeuralNode::getParams()const
{
    return _params;
}

QVector<qreal> NeuralNode::getInputs()const
{
    QVector<qreal> result;
    for(auto& it: _inputs)
        result.append(*it);
    result.append(1.0);
    return result;
}
