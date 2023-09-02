#include "Types.h"

#include "Env.h"
#include "Eval.h"

Number& Number::operator+=(const Number& o) {
    *this = operator+(o);
    return *this;
}

Number& Number::operator-=(const Number& o) {
    *this = operator-(o);
    return *this;
}

Number& Number::operator*=(const Number& o) {
    *this = operator*(o);
    return *this;
}

Number& Number::operator/=(const Number& o) {
    *this = operator/(o);
    return *this;
}

Number Number::operator+(const Number& o) {
    Number n;
    n._t = (_t == 0 && o._t == 0) ? 0 : 1;

    float f = (_t == 0) ? _v._i : _v._f;
    float s = (o._t == 0) ? o._v._i : o._v._f;

    if (n._t == 0)
        n._v._i = f + s;
    else
        n._v._f = f + s;

    return n;
}

Number Number::operator-(const Number& o) {
    Number n;
    n._t = (_t == 0 && o._t == 0) ? 0 : 1;
    float f = (_t == 0) ? _v._i : _v._f;
    float s = (o._t == 0) ? o._v._i : o._v._f;

    if (n._t == 0)
        n._v._i = f - s;
    else
        n._v._f = f - s;

    return n;
}

Number Number::operator*(const Number& o) {
    Number n;
    n._t = (_t == 0 && o._t == 0) ? 0 : 1;
    float f = (_t == 0) ? _v._i : _v._f;
    float s = (o._t == 0) ? o._v._i : o._v._f;

    if (n._t == 0)
        n._v._i = f * s;
    else
        n._v._f = f * s;

    return n;
}

Number Number::operator/(const Number& o) {
    Number n;
    n._t = (_t == 0 && o._t == 0) ? 0 : 1;

    float f = (_t == 0) ? _v._i : _v._f;
    float s = (o._t == 0) ? o._v._i : o._v._f;

    if (n._t == 0)
        n._v._i = f / s;
    else
        n._v._f = f / s;

    return n;
}

Number Number::operator>(const Number& o) {
    Number n;
    n._t = 0;

    float f = (_t == 0) ? _v._i : _v._f;
    float s = (o._t == 0) ? o._v._i : o._v._f;

    n._v._i = f > s;
    return n;
}

Number Number::operator>=(const Number& o) {
    Number n;
    n._t = 0;

    float f = (_t == 0) ? _v._i : _v._f;
    float s = (o._t == 0) ? o._v._i : o._v._f;

    n._v._i = f >= s;
    return n;
}

Number Number::operator<(const Number& o) {
    Number n;
    n._t = 0;

    float f = (_t == 0) ? _v._i : _v._f;
    float s = (o._t == 0) ? o._v._i : o._v._f;

    n._v._i = f < s;
    return n;
}

Number Number::operator<=(const Number& o) {
    Number n;
    n._t = 0;

    float f = (_t == 0) ? _v._i : _v._f;
    float s = (o._t == 0) ? o._v._i : o._v._f;

    n._v._i = f <= s;
    return n;
}

Number Number::operator==(const Number& o) {
    Number n;
    n._t = 0;
    n._v._i = _t == o._t && _v._i == o._v._i && _v._f == o._v._f;
    return n;
}

Number Number::abs(const Number& o) {
    Number n;
    n._t = o._t;
    if (o._t == 0)
        n._v._i = fabs(o._v._i);
    else
        n._v._f = fabs(o._v._f);
    return n;
}

Exp::Exp(const Exp::ExpType& v, std::uint8_t t) : _v(v), _t(t) {}

Exp::Exp(const std::vector<std::string>& params, const Exp& body) {
    _v = {._l = {std::function<Exp(std::vector<Exp>, Env)>{
              [this, params, body](std::vector<Exp> expList, Env env) -> Exp {
                  Env e{params, expList, env};
                  return Eval::eval(body, e);
              }}}};
    _t = 1;
}

Exp& Exp::operator=(const Exp& o) {
    _v = o._v;
    _t = o._t;
    return *this;
}

void Exp::print(Exp e) {
    if (e._t == 0) {
        Atom a = e._v._a;
        if (a._t == 0) {
            std::cout << a._v._s << '\n';
        } else {
            Number n = a._v._n;
            std::cout << (n._t == 0 ? n._v._i : n._v._f) << '\n';
        }
    } else {
        for (auto& exp : e._v._l) {
            try {
                print(std::any_cast<Exp>(exp));
            } catch (std::bad_any_cast exp) {
                std::cerr << exp.what();
            }
        }
    }
}
