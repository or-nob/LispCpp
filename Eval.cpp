#include "Eval.h"

#include <cassert>

#include "Types.h"

Exp Eval::eval(Exp e, Env& env) {
    if (e._t == 0 && e._v._a._t == 0) {
        return env.find(e._v._a._v._s)[e._v._a._v._s];
    } else if (e._t == 0 && e._v._a._t == 1) {
        return e;
    } else if (e._t == 1) {
        try {
            Exp fExp = std::any_cast<Exp>(e._v._l[0]);

            std::string fSym = fExp._v._a._v._s;
            if (fSym == "define") {
                std::string symbol = std::any_cast<Exp>(e._v._l[1])._v._a._v._s;
                env[symbol] = eval(std::any_cast<Exp>(e._v._l[2]), env);
                return Exp{};
            } else if (fSym == "if") {
                Exp test = std::any_cast<Exp>(e._v._l[1]);
                Exp conseq = std::any_cast<Exp>(e._v._l[2]);
                Exp alter = std::any_cast<Exp>(e._v._l[3]);

                return eval(test, env)._v._a._v._n._v._i ? eval(conseq, env) : eval(alter, env);
            } else if (fSym == "set!") {
                std::string sym = std::any_cast<Exp>(e._v._l[1])._v._a._v._s;
                env.find(sym)[sym] = eval(std::any_cast<Exp>(e._v._l[2]), env);
                return Exp{};
            } else if (fSym == "lambda") {
                Exp listExpParams = std::any_cast<Exp>(e._v._l[1]);
                std::vector<std::string> params;
                std::for_each(listExpParams._v._l.begin(), listExpParams._v._l.end(),
                              [&params](const auto& e) {
                                  const auto& p = std::any_cast<Exp>(e);
                                  params.emplace_back(p._v._a._v._s);
                              });
                Exp body = std::any_cast<Exp>(e._v._l[2]);
                return Exp{params, body, env};
            }

            auto procExp = eval(fExp, env);
            auto procExpFunc =
                std::any_cast<std::function<Exp(std::vector<Exp>)>>(procExp._v._l[0]);
            std::vector<Exp> eList;
            for (size_t i = 1; i < e._v._l.size(); ++i) {
                eList.emplace_back(eval(std::any_cast<Exp>(e._v._l[i]), env));
            }

            return procExpFunc(eList);
        } catch (...) {
            return Exp{};
        }
    }

    return Exp{};
}
