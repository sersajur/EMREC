#ifndef NETWORK_H
#define NETWORK_H

#include <QVector>
#include "Network/neuralnode.h"
#include "Network/fuzzynode.h"

    //fuzzy values of linguistic variables:
    namespace EyeBrowsValue{
        enum EyeBrowsValue{su_stretched, ha_stretched, sa_centered, di_centered, an_centered};
    }
    namespace EyesValue{
        enum EyesValue{su_extra_opened, ha_pressed_closed, sa_pressed_closed, di_pressed_closed, an_normal};
    }
    namespace NoseValue{
        enum NoseValue{di_radical};
    }
    namespace LipsValue{
        enum LipsValue{su_open, ha_stretched, sa_normal, di_twisted, an_pressed_closed};
    }

    namespace Emotions{
        enum  Emotions{SU, HA, SA, DI, AN};
    }

class Network
{
public:
    Network();
    void Init();
    void Serialize(const QString& filePath, int digit_num);
    void Deserialize(const QString& filePath);
    QVector<qreal> getParams()const;
    void setParams(QVector<qreal> &params);
    QVector<qreal> Check(QVector<qreal> normed_features);
    qreal One_lesson(const Emotions::Emotions em, const qreal true_result, qreal rate_con, qreal rate_a, qreal rate_b, qreal rate_c);
    void setInput(QVector<qreal> normed_features);
    qreal Process(Emotions::Emotions em);
private:
    QVector<qreal> f;
    FuzzyNode brows,
              eyes,
              nose,
              lips;
    QVector<QVector<NeuralNode> > cons_nodes;
};


#endif // NETWORK_H
