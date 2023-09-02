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
        ret._v._a._t = 1;

        Number sum = expList[0]._v._a._v._n;

        std::for_each(expList.begin() + 1, expList.end(), [&sum](const auto& e) {
            if (e._t != 0) return;
            Atom a = e._v._a;
            if (a._t == 0) return;
            sum += a._v._n;
        });

        ret._v._a._v._n = sum;

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
             ret._v._a._t = 1;

             Number sub = expList[0]._v._a._v._n;

             std::for_each(expList.begin() + 1, expList.end(), [&sub](const auto& e) {
                 if (e._t != 0) return;
                 Atom a = e._v._a;
                 if (a._t == 0) return;
                 sub -= a._v._n;
             });

             ret._v._a._v._n = sub;
             return ret;
         }}}},
        1,
    };

    _env["*"] = Exp{
        {._l = {Procedure{[](std::vector<Exp> expList, Env) -> Exp {
             Exp ret;
             ret._t = 0;
             ret._v._a._t = 1;

             Number mul = expList[0]._v._a._v._n;

             std::for_each(expList.begin() + 1, expList.end(), [&mul](const auto& e) {
                 if (e._t != 0) return;
                 Atom a = e._v._a;
                 if (a._t == 0) return;
                 mul *= a._v._n;
             });

             ret._v._a._v._n = mul;
             return ret;
         }}}},
        1,
    };

    _env["/"] = Exp{
        {._l = {Procedure{[](std::vector<Exp> expList, Env) -> Exp {
             Exp ret;
             ret._t = 0;
             ret._v._a._t = 1;

             Number div = expList[0]._v._a._v._n;

             std::for_each(expList.begin() + 1, expList.end(), [&div](const auto& e) {
                 if (e._t != 0) return;
                 Atom a = e._v._a;
                 if (a._t == 0) return;
                 div /= a._v._n;
             });

             ret._v._a._v._n = div;
             return ret;
         }}}},
        1,
    };

    _env[">"] = Exp{
        {._l = {Procedure{[](std::vector<Exp> expList, Env) -> Exp {
             if (expList.size() != 2) return {};
             Exp ret;
             ret._t = 0;
             ret._v._a._t = 1;
             Number cond = expList[0]._v._a._v._n > expList[1]._v._a._v._n;
             ret._v._a._v._n = cond;
             return ret;
         }}}},
        1,
    };

    _env[">="] = Exp{
        {._l = {Procedure{[](std::vector<Exp> expList, Env) -> Exp {
             if (expList.size() != 2) return {};
             Exp ret;
             ret._t = 0;
             ret._v._a._t = 1;
             Number cond = expList[0]._v._a._v._n >= expList[1]._v._a._v._n;
             ret._v._a._v._n = cond;
             return ret;
         }}}},
        1,
    };

    _env["<"] = Exp{
        {._l = {Procedure{[](std::vector<Exp> expList, Env) -> Exp {
             if (expList.size() != 2) return {};
             Exp ret;
             ret._t = 0;
             ret._v._a._t = 1;
             Number cond = expList[0]._v._a._v._n < expList[1]._v._a._v._n;
             ret._v._a._v._n = cond;
             return ret;
         }}}},
        1,
    };

    _env["<="] = Exp{
        {._l = {Procedure{[](std::vector<Exp> expList, Env) -> Exp {
             if (expList.size() != 2) return {};
             Exp ret;
             ret._t = 0;
             ret._v._a._t = 1;
             Number cond = expList[0]._v._a._v._n <= expList[1]._v._a._v._n;
             ret._v._a._v._n = cond;
             return ret;
         }}}},
        1,
    };

    _env["="] = Exp{
        {._l = {Procedure{[this](std::vector<Exp> expList, Env) -> Exp {
             if (expList.size() != 2) return {};
             Exp ret;
             ret._t = 0;
             ret._v._a._t = 0;
             Number cond = expList[0]._v._a._v._n == expList[1]._v._a._v._n;
             ret._v._a._v._n = cond;
             return ret;
         }}}},
        1,
    };

    _env["abs"] = Exp{
        {._l = {Procedure{[](std::vector<Exp> expList, Env) -> Exp {
             if (expList.size() != 1) return {};
             Exp ret;
             ret._t = 0;
             ret._v._a._t = 1;
             ret._v._a._v._n = Number::abs(expList[0]._v._a._v._n);
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
