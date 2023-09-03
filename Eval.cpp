#include "Eval.h"

#include <cassert>

#include "Types.h"

Exp Eval::eval(Exp e, Env& env) {
    bool isAtom = std::holds_alternative<Atom>(e._v);
    if (isAtom && std::holds_alternative<std::string>(std::get<Atom>(e._v)._v)) {
        return env.find(std::get<std::string>(
            std::get<Atom>(e._v)._v))[std::get<std::string>(std::get<Atom>(e._v)._v)];
    } else if ((isAtom && std::holds_alternative<Number>(std::get<Atom>(e._v)._v)) ||
               std::holds_alternative<Procedure>(e._v)) {
        return e;
    } else if (std::holds_alternative<Exp::ExpList>(e._v)) {
        try {
            auto& expList = std::get<Exp::ExpList>(e._v);
            Exp fExp = std::any_cast<Exp>(expList[0]);

            if (!std::holds_alternative<Atom>(fExp._v) ||
                !std::holds_alternative<std::string>(std::get<Atom>(fExp._v)._v))
                return Exp{};

            std::string fSym = std::get<std::string>(std::get<Atom>(fExp._v)._v);
            if (fSym == "define") {
                if (!std::holds_alternative<std::string>(
                        std::get<Atom>(std::any_cast<Exp>(expList[1])._v)._v))
                    return Exp{};
                std::string symbol =
                    std::get<std::string>(std::get<Atom>(std::any_cast<Exp>(expList[1])._v)._v);
                env[symbol] = eval(std::any_cast<Exp>(expList[2]), env);
                return Exp{};
            } else if (fSym == "if") {
                Exp test = std::any_cast<Exp>(expList[1]);
                Exp conseq = std::any_cast<Exp>(expList[2]);
                Exp alter = std::any_cast<Exp>(expList[3]);

                Exp cond = eval(test, env);
                if (!std::holds_alternative<Number>(std::get<Atom>(cond._v)._v) ||
                    !std::holds_alternative<int>(
                        std::get<Number>(std::get<Atom>(eval(test, env)._v)._v)._v))
                    return Exp{};

                return std::get<int>(std::get<Number>(std::get<Atom>(eval(test, env)._v)._v)._v)
                           ? eval(conseq, env)
                           : eval(alter, env);
            } else if (fSym == "set!") {
                if (!std::holds_alternative<std::string>(
                        std::get<Atom>(std::any_cast<Exp>(expList[1])._v)._v))
                    return Exp{};
                std::string sym =
                    std::get<std::string>(std::get<Atom>(std::any_cast<Exp>(expList[1])._v)._v);
                env.find(sym)[sym] = eval(std::any_cast<Exp>(expList[2]), env);
                return Exp{};
            } else if (fSym == "lambda") {
                Exp listExpParams = std::any_cast<Exp>(expList[1]);
                std::vector<std::string> params;
                std::for_each(
                    std::get<Exp::ExpList>(listExpParams._v).begin(),
                    std::get<Exp::ExpList>(listExpParams._v).end(), [&params](const auto& e) {
                        const auto& p = std::any_cast<Exp>(e);
                        if (!std::holds_alternative<std::string>(std::get<Atom>(p._v)._v)) return;
                        params.emplace_back(std::get<std::string>(std::get<Atom>(p._v)._v));
                    });
                Exp body = std::any_cast<Exp>(expList[2]);
                return Exp{params, body};
            } else {
                auto procExp = eval(fExp, env);
                if (!std::holds_alternative<Procedure>(procExp._v)) return Exp{};

                auto procExpFunc = std::get<Procedure>(procExp._v);
                std::vector<Exp> eList;
                for (size_t i = 1; i < expList.size(); ++i) {
                    eList.emplace_back(eval(std::any_cast<Exp>(expList[i]), env));
                }

                return procExpFunc(eList, env);
            }
        } catch (...) {
            return Exp{};
        }
    }

    return Exp{};
}
