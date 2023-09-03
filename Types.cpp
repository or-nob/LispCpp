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

Number arithmaticOperation(const Number& self, const Number& o,
                           const std::function<float(float, float)>& op) {
    bool isInt = std::holds_alternative<int>(self._v);
    bool isOtherInt = std::holds_alternative<int>(o._v);

    float f = isInt ? std::get<int>(self._v) : std::get<float>(self._v);
    float s = isOtherInt ? std::get<int>(o._v) : std::get<float>(o._v);

    if (isInt && isOtherInt) {
        return {._v = static_cast<int>(op(f, s))};
    } else {
        return {._v = static_cast<float>(op(f, s))};
    }
}

Number Number::operator+(const Number& o) {
    return arithmaticOperation(*this, o, [](float f, float s) { return f + s; });
}

Number Number::operator-(const Number& o) {
    return arithmaticOperation(*this, o, [](float f, float s) { return f - s; });
}

Number Number::operator*(const Number& o) {
    return arithmaticOperation(*this, o, [](float f, float s) { return f * s; });
}

Number Number::operator/(const Number& o) {
    return arithmaticOperation(*this, o, [](float f, float s) { return f / s; });
}

Number cmpOperation(const Number& self, const Number& o,
                    const std::function<int(float, float)>& op) {
    float f =
        std::holds_alternative<int>(self._v) ? std::get<int>(self._v) : std::get<float>(self._v);
    float s = std::holds_alternative<int>(o._v) ? std::get<int>(o._v) : std::get<float>(o._v);
    return {._v = static_cast<int>(op(f, s))};
}

Number Number::operator>(const Number& o) const {
    return cmpOperation(*this, o, [](float f, float s) { return f > s; });
}

Number Number::operator>=(const Number& o) const {
    return cmpOperation(*this, o, [](float f, float s) { return f >= s; });
}

Number Number::operator<(const Number& o) const {
    return cmpOperation(*this, o, [](float f, float s) { return f < s; });
}

Number Number::operator<=(const Number& o) const {
    return cmpOperation(*this, o, [](float f, float s) { return f <= s; });
}

Number Number::operator==(const Number& o) const {
    return cmpOperation(*this, o, [](float f, float s) { return f == s; });
}

Number Number::abs(const Number& o) {
    float s = std::holds_alternative<int>(o._v) ? std::get<int>(o._v) : std::get<float>(o._v);
    if (std::holds_alternative<int>(o._v)) return {._v = static_cast<int>(fabs(s))};
    return {._v = static_cast<float>(fabs(s))};
}

Exp::Exp(const Exp::ExpType& v) : _v(v) {}

Exp::Exp(const std::vector<std::string>& params, const Exp& body) {
    _v = Procedure{[params, body](std::vector<Exp> expList, Env env) -> Exp {
        Env e{params, expList, env};
        return Eval::eval(body, e);
    }};
}

Exp& Exp::operator=(const Exp& o) {
    _v = o._v;
    return *this;
}

void Exp::print(Exp e) {
    if (std::holds_alternative<Atom>(e._v)) {
        Atom a = std::get<Atom>(e._v);
        if (std::holds_alternative<std::string>(a._v)) {
            std::cout << std::get<std::string>(a._v) << '\n';
        } else {
            Number n = std::get<Number>(a._v);
            std::cout << (std::holds_alternative<int>(n._v) ? std::get<int>(n._v)
                                                            : std::get<float>(n._v))
                      << '\n';
        }
    } else {
        const ExpList& expL = std::get<ExpList>(e._v);
        for (auto& exp : expL) {
            try {
                print(std::any_cast<Exp>(exp));
            } catch (std::bad_any_cast exp) {
                std::cerr << exp.what();
            }
        }
    }
}
