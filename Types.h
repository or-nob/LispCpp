#pragma once

#include <any>
#include <cmath>
#include <cstdint>
#include <iostream>
#include <variant>
#include <vector>

class Env;

struct Number {
    using NumberType = std::variant<int, float>;
    NumberType _v;

    Number& operator+=(const Number& o);
    Number& operator-=(const Number& o);
    Number& operator*=(const Number& o);
    Number& operator/=(const Number& o);

    Number operator+(const Number& o);
    Number operator-(const Number& o);
    Number operator*(const Number& o);
    Number operator/(const Number& o);

    Number operator>(const Number& o);
    Number operator>=(const Number& o);
    Number operator<(const Number& o);
    Number operator<=(const Number& o);
    Number operator==(const Number& o);

    static Number abs(const Number& o);
};

struct Atom {
    using AtomType = std::variant<std::string, Number>;
    AtomType _v;
};

struct Exp {
    using ExpList = std::vector<std::any>;
    using ExpType = std::variant<Atom, ExpList>;

    ExpType _v;
    Exp() = default;
    Exp(const ExpType& v);
    Exp(const std::vector<std::string>& params, const Exp& body);

    Exp& operator=(const Exp& o);

    static void print(Exp e);
};

enum class Types { SYMBOL, NUMBER, ATOM, LIST, EXP, ENV };

using Procedure = std::function<Exp(std::vector<Exp>, Env)>;
