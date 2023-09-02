#include "Eval.h"

#include <cassert>

#include "Types.h"

Exp Eval::eval(Exp e, Env& env) {
    if (e._t == 0 && std::holds_alternative<std::string>(e._v._a._v)) {
        return env.find(std::get<std::string>(e._v._a._v))[std::get<std::string>(e._v._a._v)];
    } else if (e._t == 0 && std::holds_alternative<Number>(e._v._a._v)) {
        return e;
    } else if (e._t == 1) {
        try {
            Exp fExp = std::any_cast<Exp>(e._v._l[0]);

            if (!std::holds_alternative<std::string>(fExp._v._a._v)) return Exp{};

            std::string fSym = std::get<std::string>(fExp._v._a._v);
            if (fSym == "define") {
                if (!std::holds_alternative<std::string>(std::any_cast<Exp>(e._v._l[1])._v._a._v))
                    return Exp{};
                std::string symbol = std::get<std::string>(std::any_cast<Exp>(e._v._l[1])._v._a._v);
                env[symbol] = eval(std::any_cast<Exp>(e._v._l[2]), env);
                return Exp{};
            } else if (fSym == "if") {
                Exp test = std::any_cast<Exp>(e._v._l[1]);
                Exp conseq = std::any_cast<Exp>(e._v._l[2]);
                Exp alter = std::any_cast<Exp>(e._v._l[3]);

                Exp cond = eval(test, env);
                if (!std::holds_alternative<Number>(cond._v._a._v) ||
                    !std::holds_alternative<int>(std::get<Number>(eval(test, env)._v._a._v)._v))
                    return Exp{};

                return std::get<int>(std::get<Number>(eval(test, env)._v._a._v)._v)
                           ? eval(conseq, env)
                           : eval(alter, env);
            } else if (fSym == "set!") {
                if (!std::holds_alternative<std::string>(std::any_cast<Exp>(e._v._l[1])._v._a._v))
                    return Exp{};
                std::string sym = std::get<std::string>(std::any_cast<Exp>(e._v._l[1])._v._a._v);
                env.find(sym)[sym] = eval(std::any_cast<Exp>(e._v._l[2]), env);
                return Exp{};
            } else if (fSym == "lambda") {
                Exp listExpParams = std::any_cast<Exp>(e._v._l[1]);
                std::vector<std::string> params;
                std::for_each(listExpParams._v._l.begin(), listExpParams._v._l.end(),
                              [&params](const auto& e) {
                                  const auto& p = std::any_cast<Exp>(e);
                                  if (!std::holds_alternative<std::string>(p._v._a._v)) return;
                                  params.emplace_back(std::get<std::string>(p._v._a._v));
                              });
                Exp body = std::any_cast<Exp>(e._v._l[2]);
                return Exp{params, body};
            } else {
                auto procExp = eval(fExp, env);

                auto procExpFunc = std::any_cast<Procedure>(procExp._v._l[0]);
                std::vector<Exp> eList;
                for (size_t i = 1; i < e._v._l.size(); ++i) {
                    eList.emplace_back(eval(std::any_cast<Exp>(e._v._l[i]), env));
                }

                return procExpFunc(eList, env);
            }
        } catch (...) {
            return Exp{};
        }
    }

    return Exp{};
}
