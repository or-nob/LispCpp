#pragma once

#include <any>
#include <cmath>
#include <cstdint>
#include <iostream>
#include <vector>

class Env;

struct Number {
    typedef struct {
        int _i;
        float _f;
    } NumberType;

    NumberType _v;
    std::uint8_t _t;

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
    typedef struct {
        std::string _s;
        Number _n;
    } AtomType;
    AtomType _v;
    std::uint8_t _t;
};

struct Exp {
    typedef struct {
        Atom _a;
        std::vector<std::any> _l;
    } ExpType;
    ExpType _v;
    std::uint8_t _t;

    Exp() = default;
    Exp(const ExpType& v, std::uint8_t t);
    Exp(const std::vector<std::string>& params, const Exp& body, Env& env);

    Exp& operator=(const Exp& o);

    static void print(Exp e);
};

enum class Types { SYMBOL, NUMBER, ATOM, LIST, EXP, ENV };

