#include "Env.h"

#include <cassert>
#include <cmath>

#include "Types.h"

Env::Env() { init(); }

Env::Env(const std::vector<std::string>& keys, const std::vector<Exp>& args, Env& outer) {
    init();
    for (size_t i = 0; i < keys.size(); ++i) {
        _env[keys[i]] = args[i];
    }
    _outer = &outer;
}

Env& Env::find(const std::string& k) {
    const auto& ret =
        std::find_if(_env.begin(), _env.end(), [&k](const auto& e) { return e.first == k; });
    if (ret != _env.end()) return *this;
    if (!_outer) std::runtime_error("Cannot find");
    return _outer->find(k);
}

void Env::init() {
    _env["begin"] = Exp{Exp::ExpType{Procedure{[](std::vector<Exp> expList, Env) {
        Exp ret;
        ret._v = Exp::ExpList{};
        std::for_each(expList.begin(), expList.end(),
                      [&ret](const auto& e) { std::get<Exp::ExpList>(ret._v).emplace_back(e); });
        return ret;
    }}}};

    auto arithmaticOperation = [](const std::vector<Exp>& expList,
                                  std::function<void(Number&, Number&)> op) {
        Exp ret;

        if (!std::holds_alternative<Number>(std::get<Atom>(expList[0]._v)._v)) return Exp{};

        Number acc = std::get<Number>(std::get<Atom>(expList[0]._v)._v);

        std::for_each(expList.begin() + 1, expList.end(), [&acc, &op](const Exp& e) {
            if (!std::holds_alternative<Atom>(e._v)) return;
            Atom a = std::get<Atom>(e._v);
            if (!std::holds_alternative<Number>(a._v)) return;
            op(acc, std::get<Number>(a._v));
        });

        ret._v = Atom{._v = Number{acc}};
        return ret;
    };

    _env["+"] =
        Exp{Exp::ExpType{Procedure{[&arithmaticOperation](std::vector<Exp> expList, Env) -> Exp {
            return arithmaticOperation(expList, [](Number& acc, Number& elem) { acc += elem; });
        }}}};

    _env["-"] =
        Exp{Exp::ExpType{Procedure{[&arithmaticOperation](std::vector<Exp> expList, Env) -> Exp {
            return arithmaticOperation(expList, [](Number& acc, Number& elem) { acc -= elem; });
        }}}};

    _env["*"] =
        Exp{Exp::ExpType{Procedure{[&arithmaticOperation](std::vector<Exp> expList, Env) -> Exp {
            return arithmaticOperation(expList, [](Number& acc, Number& elem) { acc *= elem; });
        }}}};

    _env["/"] =
        Exp{Exp::ExpType{Procedure{[&arithmaticOperation](std::vector<Exp> expList, Env) -> Exp {
            return arithmaticOperation(expList, [](Number& acc, Number& elem) { acc /= elem; });
        }}}};

    auto cmpOperation = [](const std::vector<Exp>& expList,
                           std::function<Number(const Number&, const Number&)> op) -> Exp {
        if (expList.size() != 2) return Exp{};

        Exp ret;

        if (!std::holds_alternative<Number>(std::get<Atom>(expList[0]._v)._v) ||
            !std::holds_alternative<Number>(std::get<Atom>(expList[1]._v)._v))
            return Exp{};

        ret._v = Atom{._v = op(std::get<Number>(std::get<Atom>(expList[0]._v)._v),
                               std::get<Number>(std::get<Atom>(expList[1]._v)._v))};
        return ret;
    };

    _env[">"] = Exp{Exp::ExpType{Procedure{[&cmpOperation](std::vector<Exp> expList, Env) -> Exp {
        return cmpOperation(expList, [](const Number& f, const Number& s) { return f > s; });
    }}}};

    _env[">="] = Exp{Exp::ExpType{Procedure{[&cmpOperation](std::vector<Exp> expList, Env) -> Exp {
        return cmpOperation(expList, [](const Number& f, const Number& s) { return f >= s; });
    }}}};

    _env["<"] = Exp{Exp::ExpType{Procedure{[&cmpOperation](std::vector<Exp> expList, Env) -> Exp {
        return cmpOperation(expList, [](const Number& f, const Number& s) { return f < s; });
    }}}};

    _env["<="] = Exp{Exp::ExpType{Procedure{[&cmpOperation](std::vector<Exp> expList, Env) -> Exp {
        return cmpOperation(expList, [](const Number& f, const Number& s) { return f <= s; });
    }}}};

    _env["="] = Exp{Exp::ExpType{Procedure{[&cmpOperation](std::vector<Exp> expList, Env) -> Exp {
        return cmpOperation(expList, [](const Number& f, const Number& s) { return f == s; });
    }}}};

    _env["abs"] = Exp{Exp::ExpType{Procedure{[](std::vector<Exp> expList, Env) -> Exp {
        if (expList.size() != 1) return {};
        Exp ret;
        if (!std::holds_alternative<Number>(std::get<Atom>(expList[0]._v)._v)) return Exp{};
        ret._v = Atom{._v = Number::abs(std::get<Number>(std::get<Atom>(expList[0]._v)._v))};
        return ret;
    }}}};
}

Exp& Env::operator[](const std::string& k) { return _env[k]; }

void Env::printKeys() {
    std::for_each(_env.begin(), _env.end(), [](const auto& e) { std::cout << e.first << " "; });
    std::cout << '\n';
    if (_outer) _outer->printKeys();
}
