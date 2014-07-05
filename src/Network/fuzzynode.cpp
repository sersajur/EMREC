#include "fuzzynode.h"
#include <qmath.h>

FuzzyNode::FuzzyNode(int v_type_number, int f_number, int mf_p_number)
{
    _inputs.resize(f_number);
    _fuzzy_set_params.resize(v_type_number);
    for(auto& t: _fuzzy_set_params){
        t.resize(f_number);
        for(auto& f: t)
            f.resize(mf_p_number);
    }
}

qreal FuzzyNode::_member_fun(qreal x, qreal a, qreal b, qreal c)
{
    return 1/(1 + qPow(qAbs((x - a)/c),2*b));
}

qreal FuzzyNode::get_d_a(int feature_id, int fuzzy_id)
{
    auto& x = *(_inputs[feature_id]);
    auto& a = _fuzzy_set_params[fuzzy_id][feature_id][0];
    auto& b = _fuzzy_set_params[fuzzy_id][feature_id][1];
    auto& c = _fuzzy_set_params[fuzzy_id][feature_id][2];
    return 2*b*(x - a)*qPow(qAbs((a-x)/c),2*b-2)/(c*c*qPow((qPow(qAbs((a-x)/c),2*b)+1),2));
}

qreal FuzzyNode::get_d_b(int feature_id, int fuzzy_id)
{
    auto& x = *(_inputs[feature_id]);
    auto& a = _fuzzy_set_params[fuzzy_id][feature_id][0];
    auto& b = _fuzzy_set_params[fuzzy_id][feature_id][1];
    auto& c = _fuzzy_set_params[fuzzy_id][feature_id][2];
    return -2*qPow(qAbs((a-x)/c),2*b)*qLn(qAbs((a-x)/c))/qPow((qPow(qAbs((a-x)/c),2*b)+1),2);
}

qreal FuzzyNode::get_d_c(int feature_id, int fuzzy_id)
{
    auto& x = *(_inputs[feature_id]);
    auto& a = _fuzzy_set_params[fuzzy_id][feature_id][0];
    auto& b = _fuzzy_set_params[fuzzy_id][feature_id][1];
    auto& c = _fuzzy_set_params[fuzzy_id][feature_id][2];
    return 2*b*(a-x)*(a-x)*qPow(qAbs((a-x)/c),2*b-2)/(c*c*c*qPow((qPow(qAbs((a-x)/c),2*b)+1),2));
}

qreal FuzzyNode::Process(int fuzzy_id)
{
    qreal result = 1;
    for(int i=0; i<_fuzzy_set_params[fuzzy_id].size(); i++)
    {
        auto& p = _fuzzy_set_params[fuzzy_id][i];
        result = qMin(result,_member_fun(*(_inputs[i]),p[0],p[1],p[2]));
    }
    return result;
}
