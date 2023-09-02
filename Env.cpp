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
    _env["begin"] = Exp{
        {._l = {Procedure{[](std::vector<Exp> expList, Env) {
             Exp ret;
             ret._t = 1;
             std::for_each(expList.begin(), expList.end(),
                           [&ret](const auto& e) { ret._v._l.emplace_back(e); });
             return ret;
         }}}},
        1,
    };

    Procedure add = [](std::vector<Exp> expList, Env) -> Exp {
        Exp ret;
        ret._t = 0;

        if (!std::holds_alternative<Number>(expList[0]._v._a._v)) return Exp{};

        Number sum = std::get<Number>(expList[0]._v._a._v);

        std::for_each(expList.begin() + 1, expList.end(), [&sum](const auto& e) {
            if (e._t != 0) return;
            Atom a = e._v._a;
            if (!std::holds_alternative<Number>(a._v)) return;
            sum += std::get<Number>(a._v);
        });

        ret._v._a._v = sum;

        return ret;
    };

    _env["+"] = Exp{
        {._l = {add}},
        1,
    };

    _env["-"] = Exp{
        {._l = {Procedure{[](std::vector<Exp> expList, Env) -> Exp {
             Exp ret;
             ret._t = 0;

             if (!std::holds_alternative<Number>(expList[0]._v._a._v)) return Exp{};

             Number sub = std::get<Number>(expList[0]._v._a._v);

             std::for_each(expList.begin() + 1, expList.end(), [&sub](const auto& e) {
                 if (e._t != 0) return;
                 Atom a = e._v._a;
                 if (!std::holds_alternative<Number>(a._v)) return;
                 sub -= std::get<Number>(a._v);
             });

             ret._v._a._v = sub;
             return ret;
         }}}},
        1,
    };

    _env["*"] = Exp{
        {._l = {Procedure{[](std::vector<Exp> expList, Env) -> Exp {
             Exp ret;
             ret._t = 0;

             if (!std::holds_alternative<Number>(expList[0]._v._a._v)) return Exp{};

             Number mul = std::get<Number>(expList[0]._v._a._v);

             std::for_each(expList.begin() + 1, expList.end(), [&mul](const auto& e) {
                 if (e._t != 0) return;
                 Atom a = e._v._a;
                 if (!std::holds_alternative<Number>(a._v)) return;
                 mul *= std::get<Number>(a._v);
             });

             ret._v._a._v = mul;
             return ret;
         }}}},
        1,
    };

    _env["/"] = Exp{
        {._l = {Procedure{[](std::vector<Exp> expList, Env) -> Exp {
             Exp ret;
             ret._t = 0;

             if (!std::holds_alternative<Number>(expList[0]._v._a._v)) return Exp{};

             Number div = std::get<Number>(expList[0]._v._a._v);

             std::for_each(expList.begin() + 1, expList.end(), [&div](const auto& e) {
                 if (e._t != 0) return;
                 Atom a = e._v._a;
                 if (!std::holds_alternative<Number>(a._v)) return;
                 div /= std::get<Number>(a._v);
             });

             ret._v._a._v = div;
             return ret;
         }}}},
        1,
    };

    _env[">"] = Exp{
        {._l = {Procedure{[](std::vector<Exp> expList, Env) -> Exp {
             if (expList.size() != 2) return {};
             Exp ret;
             ret._t = 0;

             if (!std::holds_alternative<Number>(expList[0]._v._a._v) ||
                 !std::holds_alternative<Number>(expList[1]._v._a._v))
                 return Exp{};

             Number cond =
                 std::get<Number>(expList[0]._v._a._v) > std::get<Number>(expList[1]._v._a._v);
             ret._v._a._v = cond;
             return ret;
         }}}},
        1,
    };

    _env[">="] = Exp{
        {._l = {Procedure{[](std::vector<Exp> expList, Env) -> Exp {
             if (expList.size() != 2) return {};
             Exp ret;
             ret._t = 0;
             if (!std::holds_alternative<Number>(expList[0]._v._a._v) ||
                 !std::holds_alternative<Number>(expList[1]._v._a._v))
                 return Exp{};

             Number cond =
                 std::get<Number>(expList[0]._v._a._v) >= std::get<Number>(expList[1]._v._a._v);
             ret._v._a._v = cond;
             return ret;
         }}}},
        1,
    };

    _env["<"] = Exp{
        {._l = {Procedure{[](std::vector<Exp> expList, Env) -> Exp {
             if (expList.size() != 2) return {};
             Exp ret;
             ret._t = 0;
             if (!std::holds_alternative<Number>(expList[0]._v._a._v) ||
                 !std::holds_alternative<Number>(expList[1]._v._a._v))
                 return Exp{};

             Number cond =
                 std::get<Number>(expList[0]._v._a._v) < std::get<Number>(expList[1]._v._a._v);
             ret._v._a._v = cond;
             return ret;
         }}}},
        1,
    };

    _env["<="] = Exp{
        {._l = {Procedure{[](std::vector<Exp> expList, Env) -> Exp {
             if (expList.size() != 2) return {};
             Exp ret;
             ret._t = 0;
             if (!std::holds_alternative<Number>(expList[0]._v._a._v) ||
                 !std::holds_alternative<Number>(expList[1]._v._a._v))
                 return Exp{};

             Number cond =
                 std::get<Number>(expList[0]._v._a._v) <= std::get<Number>(expList[1]._v._a._v);
             ret._v._a._v = cond;
             return ret;
         }}}},
        1,
    };

    _env["="] = Exp{
        {._l = {Procedure{[this](std::vector<Exp> expList, Env) -> Exp {
             if (expList.size() != 2) return {};
             Exp ret;
             ret._t = 0;
             if (!std::holds_alternative<Number>(expList[0]._v._a._v) ||
                 !std::holds_alternative<Number>(expList[1]._v._a._v))
                 return Exp{};

             Number cond =
                 std::get<Number>(expList[0]._v._a._v) == std::get<Number>(expList[1]._v._a._v);
             ret._v._a._v = cond;
             return ret;
         }}}},
        1,
    };

    _env["abs"] = Exp{
        {._l = {Procedure{[](std::vector<Exp> expList, Env) -> Exp {
             if (expList.size() != 1) return {};
             Exp ret;
             ret._t = 0;
             if (!std::holds_alternative<Number>(expList[0]._v._a._v)) return Exp{};
             ret._v._a._v = Number::abs(std::get<Number>(expList[0]._v._a._v));
             return ret;
         }}}},
        1,
    };
}

Exp& Env::operator[](const std::string& k) { return _env[k]; }

void Env::printKeys() {
    std::for_each(_env.begin(), _env.end(), [](const auto& e) { std::cout << e.first << " "; });
    std::cout << '\n';
    if (_outer) _outer->printKeys();
}
