#include "Network/network.h"
#include <QXmlStreamWriter>
#include <QXmlStreamReader>
#include <QTextStream>
#include <QFile>
#include <qmath.h>


Network::Network():
    brows{5,8,3},
    eyes{5,4,3},
    nose{1,2,3},
    lips{5,3,3}
{
    f.resize(21);
    cons_nodes.resize(5);
    cons_nodes[Emotions::SU].resize(3);
    cons_nodes[Emotions::HA].resize(3);
    cons_nodes[Emotions::SA].resize(3);
    cons_nodes[Emotions::DI].resize(4);
    cons_nodes[Emotions::AN].resize(3);

}

void Network::Init()
{

    brows.setInputs(QVector<qreal*>{&(f[0]), &(f[1]), &(f[15]), &(f[2]), &(f[3]), &(f[16]), &(f[12]), &(f[17])});
    eyes.setInputs(QVector<qreal*>{&(f[4]), &(f[5]), &(f[6]), &(f[7])/*, &(f[19])*/});
    nose.setInputs(QVector<qreal*>{&(f[8]), &(f[9])});
    lips.setInputs(QVector<qreal*>{&(f[10]), &(f[11]), &(f[14])});
    QVector<qreal*> without_nose = {&(f[0]),&(f[1]),&(f[2]),&(f[3]),&(f[4]),&(f[5]),&(f[6]),&(f[7]),&(f[10]),&(f[11]),&(f[12]),&(f[14]),&(f[15]),&(f[16]),&(f[17])};
    QVector<qreal*> with_nose = {&(f[0]),&(f[1]),&(f[2]),&(f[3]),&(f[4]),&(f[5]),&(f[6]),&(f[7]),&(f[8]),&(f[9]),&(f[10]),&(f[11]),&(f[12]),&(f[14]),&(f[15]),&(f[16]),&(f[17])};
    for(int i = 0; i < 3; i++){
        cons_nodes[Emotions::SU][i].setInputs(without_nose);
        cons_nodes[Emotions::HA][i].setInputs(without_nose);
        cons_nodes[Emotions::SA][i].setInputs(without_nose);
        cons_nodes[Emotions::AN][i].setInputs(without_nose);
    }
    for(int i = 0; i < 4; i++)
        cons_nodes[Emotions::DI][i].setInputs(with_nose);
}

QVector<qreal> Network::Check(QVector<qreal> normed_features)
{
    QVector<qreal> result;
    setInput(normed_features);
    result.append(Process(Emotions::SU));
    result.append(Process(Emotions::HA));
    result.append(Process(Emotions::SA));
    result.append(Process(Emotions::DI));
    result.append(Process(Emotions::AN));
    return result;
}

void Network::setInput(QVector<qreal> normed_features)
{
    for(int i=0; i < f.size(); i++)
        f[i] = normed_features[i];
}

qreal Network::One_lesson(const Emotions::Emotions em, const qreal true_result, qreal rate_con, qreal rate_a, qreal rate_b, qreal rate_c)
{
    qreal max_delta=0;
    qreal delta;
    auto fst_result = Process(em);
    //ADALINE for L2:
    for(auto& it: cons_nodes[em])
    {
        QVector<qreal> params = it.getParams();
        for(int i=0; i<params.size(); i++){
            delta = it.getV(i)*(true_result - fst_result)*rate_con;
            params[i] += delta;
            max_delta = qMax(max_delta,qAbs(delta));
        }
        it.setParams(params);
    }
    //generalized delta rule (back propagation)
    auto vec_fuzzy = brows.getParams();
    for(int i=0; i < cons_nodes[em].size(); i++)
    {
        for(int j=0; j<vec_fuzzy[em].size(); j++){
           delta = -brows.get_d_a(j,em)*(cons_nodes[em][i].getLinComb() - Process(em))*rate_a*(true_result - fst_result);
           max_delta = qMax(max_delta, qAbs(delta));
           vec_fuzzy[em][j][0] += delta;
           delta = -brows.get_d_b(j,em)*(cons_nodes[em][i].getLinComb() - Process(em))*rate_b*(true_result - fst_result);
           max_delta = qMax(max_delta, qAbs(delta));
           vec_fuzzy[em][j][1] += delta;
           delta = -brows.get_d_c(j,em)*(cons_nodes[em][i].getLinComb() - Process(em))*rate_c*(true_result - fst_result);
           max_delta = qMax(max_delta, qAbs(delta));
           vec_fuzzy[em][j][2] += delta;
        }
    }
    brows.setParams(vec_fuzzy);
    vec_fuzzy = eyes.getParams();
    for(int i=0; i < cons_nodes[em].size(); i++)
    {
        for(int j=0; j<vec_fuzzy[em].size(); j++){
           delta = -eyes.get_d_a(j,em)*(cons_nodes[em][i].getLinComb() - Process(em))*rate_a*(true_result - fst_result);
           max_delta = qMax(max_delta, qAbs(delta));
           vec_fuzzy[em][j][0] += delta;
           delta = -eyes.get_d_b(j,em)*(cons_nodes[em][i].getLinComb() - Process(em))*rate_b*(true_result - fst_result);
           max_delta = qMax(max_delta, qAbs(delta));
           vec_fuzzy[em][j][1] += delta;
           delta = -eyes.get_d_c(j,em)*(cons_nodes[em][i].getLinComb() - Process(em))*rate_c*(true_result - fst_result);
           max_delta = qMax(max_delta, qAbs(delta));
           vec_fuzzy[em][j][2] += delta;
        }
    }
    eyes.setParams(vec_fuzzy);
    if(em==Emotions::DI){
        vec_fuzzy = nose.getParams();
        for(int i=0; i < cons_nodes[em].size(); i++)
        {
            for(int j=0; j<vec_fuzzy[0].size(); j++){
                delta = -nose.get_d_a(j,0)*(cons_nodes[em][i].getLinComb() - Process(em))*rate_a*(true_result - fst_result);
                max_delta = qMax(max_delta, qAbs(delta));
                vec_fuzzy[0][j][0] += delta;
                delta = -nose.get_d_b(j,0)*(cons_nodes[em][i].getLinComb() - Process(em))*rate_b*(true_result - fst_result);
                max_delta = qMax(max_delta, qAbs(delta));
                vec_fuzzy[0][j][1] += delta;
                delta = -nose.get_d_c(j,0)*(cons_nodes[em][i].getLinComb() - Process(em))*rate_c*(true_result - fst_result);
                max_delta = qMax(max_delta, qAbs(delta));
                vec_fuzzy[0][j][2] += delta;
            }
        }
        nose.setParams(vec_fuzzy);
    }
    vec_fuzzy = lips.getParams();
    for(int i=0; i < cons_nodes[em].size(); i++)
    {
        for(int j=0; j<vec_fuzzy[em].size(); j++){
           delta = -lips.get_d_a(j,em)*(cons_nodes[em][i].getLinComb() - Process(em))*rate_a*(true_result - fst_result);
           max_delta = qMax(max_delta, qAbs(delta));
           vec_fuzzy[em][j][0] += delta;
           delta = -lips.get_d_b(j,em)*(cons_nodes[em][i].getLinComb() - Process(em))*rate_b*(true_result - fst_result);
           max_delta = qMax(max_delta, qAbs(delta));
           vec_fuzzy[em][j][1] += delta;
           delta = -lips.get_d_c(j,em)*(cons_nodes[em][i].getLinComb() - Process(em))*rate_c*(true_result - fst_result);
           max_delta = qMax(max_delta, qAbs(delta));
           vec_fuzzy[em][j][2] += delta;
        }
    }
    lips.setParams(vec_fuzzy);
    return max_delta;
}

qreal Network::Process(Emotions::Emotions em)
{
         qreal tmp = 0;
         QVector<qreal> f_v;
         qreal result = 0;
         switch (em) {
         case Emotions::SU:
             f_v.append(brows.Process(EyeBrowsValue::su_stretched));
             f_v.append(eyes.Process(EyesValue::su_extra_opened));
             f_v.append(lips.Process(LipsValue::su_open));
             break;
         case Emotions::HA:
             f_v.append(brows.Process(EyeBrowsValue::ha_stretched));
             f_v.append(eyes.Process(EyesValue::ha_pressed_closed));
             f_v.append(lips.Process(LipsValue::ha_stretched));
             break;
         case Emotions::SA:
             f_v.append(brows.Process(EyeBrowsValue::sa_centered));
             f_v.append(eyes.Process(EyesValue::sa_pressed_closed));
             f_v.append(lips.Process(LipsValue::sa_normal));
             break;
         case Emotions::DI:
             f_v.append(brows.Process(EyeBrowsValue::di_centered));
             f_v.append(eyes.Process(EyesValue::di_pressed_closed));
             f_v.append(nose.Process(NoseValue::di_radical));
             f_v.append(lips.Process(LipsValue::di_twisted));
             break;
         case Emotions::AN:
             f_v.append(brows.Process(EyeBrowsValue::an_centered));
             f_v.append(eyes.Process(EyesValue::an_normal));
             f_v.append(lips.Process(LipsValue::an_pressed_closed));
             break;
         default:
             break;
         }
    for(auto& q: f_v)
        tmp += q;
    for(int i=0; i<cons_nodes[em].size(); i++)
        result += cons_nodes[em][i].Process(f_v[i]/tmp);
    return result;
}

void Network::Serialize(const QString& filePath, int digit_num)
{
    QFile file(filePath);
    QString serial;
    QXmlStreamWriter stream(&serial);
    QVector<QString> name_str = {"surprized", "happy", "sad", "disgusted", "angry"};
    stream.setAutoFormatting(true);
    stream.writeStartDocument();
    stream.writeStartElement("Weights");
    stream.writeStartElement("Premise");
    QString str = "\n";
    auto params = brows.getParams();
    for(auto eb: params)
        for(auto& al: eb){
            for(auto& it: al)
                str.append(QString::number(it,'f',digit_num) + " ");
           str.append("\n");
        }
    stream.writeTextElement("eye_brows",str);
    str = "\n";
    params = eyes.getParams();
    for(auto& eb: params)
        for(auto& al: eb){
            for(auto& it: al)
                str.append(QString::number(it,'f',digit_num) + " ");
           str.append("\n");
        }
    stream.writeTextElement("eyes",str);
    str = "\n";
    params = nose.getParams();
    for(auto& eb: params)
        for(auto& al: eb){
            for(auto& it: al)
                str.append(QString::number(it,'f',digit_num) + " ");
           str.append("\n");
        }
    stream.writeTextElement("nose",str);
    str = "\n";
    params = lips.getParams();
    for(auto& eb: params)
        for(auto& al: eb){
            for(auto& it: al)
                str.append(QString::number(it,'f',digit_num) + " ");
           str.append("\n");
        }
    stream.writeTextElement("lips",str);

    stream.writeEndElement();

    stream.writeStartElement("Consequent");

    str = "\n";
    for(int i = 0; i < cons_nodes.size(); i++)
    {
        for(auto& n: cons_nodes[i])
        {
            auto vp = n.getParams();
                for(auto& p: vp)
                    str.append(QString::number(p,'f',digit_num) + " ");
            str.append("\n");
        }
        stream.writeTextElement(name_str[i],str);
        str = "\n";
    }
    stream.writeEndElement();
    stream.writeEndDocument();
    file.open(QIODevice::WriteOnly);
    file.write(serial.toLatin1());
    file.close();
}

void Network::Deserialize(const QString& filePath)
{
    QFile file(filePath);
    file.open(QIODevice::ReadOnly);
    QXmlStreamReader stream(&file);

    QString strst;
    QTextStream str(&strst);
    while(!stream.atEnd())
    {
        /*QXmlStreamReader::TokenType token=*/stream.readNext();
        if (stream.isStartElement())
        {
            str.seek(0);
            if(stream.name() == "eye_brows"){
                strst = stream.readElementText();//to be cont...
                auto brows_params = brows.getParams();
                for(auto& t: brows_params)
                    for(auto& f: t)
                        for(auto& p: f)
                            str >> p;
                brows.setParams(brows_params);
            }
            if(stream.name() == "eyes"){
                strst = stream.readElementText();
                auto eyes_params = eyes.getParams();
                for(auto& t: eyes_params)
                    for(auto& f: t)
                        for(auto& p: f)
                            str >> p;
                eyes.setParams(eyes_params);
            }
            if(stream.name() == "nose"){
                strst = stream.readElementText();
                auto nose_params = nose.getParams();
                for(auto& t: nose_params)
                    for(auto& f: t)
                        for(auto& p: f)
                            str >> p;
                nose.setParams(nose_params);
            }
            if(stream.name() == "lips"){
                strst = stream.readElementText();
                auto lips_params = lips.getParams();
                for(auto& t: lips_params)
                    for(auto& f: t)
                        for(auto& p: f)
                            str >> p;
                lips.setParams(lips_params);
            }
            QVector<NeuralNode>* nodes = 0;
            if(stream.name() == "surprized")
            {
                strst = stream.readElementText();
                nodes = &cons_nodes[Emotions::SU];
            }
            if(stream.name() == "happy")
            {
                strst = stream.readElementText();
                nodes = &cons_nodes[Emotions::HA];
            }
            if(stream.name() == "sad")
            {
                strst = stream.readElementText();
                nodes = &cons_nodes[Emotions::SA];
            }
            if(stream.name() == "disgusted")
            {
                strst = stream.readElementText();
                nodes = &cons_nodes[Emotions::DI];
            }
            if(stream.name() == "angry")
            {
                strst = stream.readElementText();
                nodes = &cons_nodes[Emotions::AN];
            }
            if (nodes){
                for(auto& n: *nodes)
                {
                    auto vp = n.getParams();
                    for(auto& p: vp)
                        str >> p;
                    n.setParams(vp);
                }

            }
        }
    }
    file.close();
}

QVector<qreal> Network::getParams()const
{
    QVector<qreal> result;
    QVector<const FuzzyNode*> fuzzy = {&brows,&eyes,&nose,&lips};
    for(auto& fn: fuzzy)
    {
        auto f_s_p = fn->getParams();
        for(auto& f: f_s_p)
            for(auto& s: f)
                for(auto& p: s)
                    result.append(p);
    }
    for(auto& em: cons_nodes)
        for(auto& n: em)
        {
            auto vp = n.getParams();
            for(auto& p: vp)
                result.append(p);
        }
    return result;
}

void Network::setParams(QVector<qreal>& params)
{
    QVector<FuzzyNode*> fuzzy = {&brows,&eyes,&nose,&lips};
    for(auto& fn: fuzzy)
    {
        auto f_s_p = fn->getParams();
        for(auto& f: f_s_p)
            for(auto& s: f)
                for(auto& p: s)
                    p = params.takeFirst();
        fn->setParams(f_s_p);
    }
    for(auto& em: cons_nodes)
        for(auto& n: em)
        {
            auto vp = n.getParams();
            for(auto& p: vp)
                p = params.takeFirst();
            n.setParams(vp);
        }
}
