#include "../Eval.h"

#include "../Tokenizer.h"
#include "gtest/gtest.h"

TEST(Eval, evalSimpleExpr) {
    Tokenizer t{"(+ 10 10)"};

    Exp e = Eval::eval(t.readFromTokens(), Env{});
    EXPECT_TRUE(std::holds_alternative<Atom>(e._v));

    const Atom& a = std::get<Atom>(e._v);
    EXPECT_TRUE(std::holds_alternative<Number>(a._v));

    const Number& n = std::get<Number>(a._v);
    EXPECT_TRUE(std::holds_alternative<int>(n._v));

    EXPECT_EQ(std::get<int>(n._v), 20);

    Tokenizer t1{"(+ 10.5 10)"};

    Exp e1 = Eval::eval(t1.readFromTokens(), Env{});
    EXPECT_TRUE(std::holds_alternative<Atom>(e1._v));

    const Atom& a1 = std::get<Atom>(e1._v);
    EXPECT_TRUE(std::holds_alternative<Number>(a1._v));

    const Number& n1 = std::get<Number>(a1._v);
    EXPECT_TRUE(std::holds_alternative<float>(n1._v));

    EXPECT_EQ(std::get<float>(n1._v), 20.5);
}

TEST(Eval, evalVarExpr) {
    Tokenizer t{"(begin (define r 50) (/ r 5.1))"};

    Exp e = Eval::eval(t.readFromTokens(), Env{});
    EXPECT_TRUE(std::holds_alternative<Exp::ExpList>(e._v));

    Exp::ExpList l = std::get<Exp::ExpList>(e._v);
    EXPECT_EQ(l.size(), 2);

    EXPECT_NO_THROW(std::any_cast<Exp>(l[1]););
    Exp retExp = std::any_cast<Exp>(l[1]);
    EXPECT_TRUE(std::holds_alternative<Atom>(retExp._v));

    Atom a = std::get<Atom>(retExp._v);
    const Number& n = std::get<Number>(a._v);
    EXPECT_TRUE(std::holds_alternative<float>(n._v));

    EXPECT_NEAR(std::get<float>(n._v), 9.8, 0.01);
}

TEST(Eval, evalIfExpr) {
    Tokenizer t{"(begin (if (> 20 10) (abs -1.1) (- 3 3)))"};

    Exp e = Eval::eval(t.readFromTokens(), Env{});
    EXPECT_TRUE(std::holds_alternative<Exp::ExpList>(e._v));

    Exp::ExpList l = std::get<Exp::ExpList>(e._v);
    EXPECT_EQ(l.size(), 1);

    EXPECT_NO_THROW(std::any_cast<Exp>(l[0]););
    Exp retExp = std::any_cast<Exp>(l[0]);
    EXPECT_TRUE(std::holds_alternative<Atom>(retExp._v));

    Atom a = std::get<Atom>(retExp._v);
    const Number& n = std::get<Number>(a._v);
    EXPECT_TRUE(std::holds_alternative<float>(n._v));

    EXPECT_EQ(std::get<float>(n._v), static_cast<float>(1.1));
}

TEST(Eval, evalComplexExpr) {
    Tokenizer t{"(begin (define r 5) (if (= r 5) (+ r 5.1) (+ r 5)))"};

    Exp e = Eval::eval(t.readFromTokens(), Env{});
    EXPECT_TRUE(std::holds_alternative<Exp::ExpList>(e._v));

    Exp::ExpList l = std::get<Exp::ExpList>(e._v);
    EXPECT_EQ(l.size(), 2);

    EXPECT_NO_THROW(std::any_cast<Exp>(l[1]););
    Exp retExp = std::any_cast<Exp>(l[1]);
    EXPECT_TRUE(std::holds_alternative<Atom>(retExp._v));

    Atom a = std::get<Atom>(retExp._v);
    const Number& n = std::get<Number>(a._v);
    EXPECT_TRUE(std::holds_alternative<float>(n._v));

    EXPECT_EQ(std::get<float>(n._v), static_cast<float>(10.1));
}

TEST(Eval, evalFuncExpr) {
    Env env{};
    EXPECT_THROW(env.find("circle-area"), std::runtime_error);

    Tokenizer t{"(define circle-area (lambda (r) (* 3.14 (* r r))))"};

    Exp e = Eval::eval(t.readFromTokens(), std::forward<Env>(env));
    EXPECT_TRUE(std::holds_alternative<Atom>(e._v));

    EXPECT_NO_THROW(env.find("circle-area")["circle-area"]);

    Tokenizer t1{"(circle-area 5)"};
    Exp ret = Eval::eval(t1.readFromTokens(), std::forward<Env>(env));
    EXPECT_TRUE(std::holds_alternative<Atom>(ret._v));

    Atom a = std::get<Atom>(ret._v);
    const Number& n = std::get<Number>(a._v);
    EXPECT_TRUE(std::holds_alternative<float>(n._v));

    EXPECT_EQ(std::get<float>(n._v), static_cast<float>(78.5));
}

TEST(Eval, evalRecFuncExpr) {
    Env env{};
    EXPECT_THROW(env.find("fact"), std::runtime_error);

    Tokenizer t{"(define fact (lambda (n) (if (<= n 1) 1 (* n (fact (- n 1))))))"};
    Exp e = Eval::eval(t.readFromTokens(), std::forward<Env>(env));
    EXPECT_TRUE(std::holds_alternative<Atom>(e._v));

    EXPECT_NO_THROW(env.find("fact")["fact"]);

    Tokenizer t1{"(fact 5)"};
    Exp ret = Eval::eval(t1.readFromTokens(), std::forward<Env>(env));
    EXPECT_TRUE(std::holds_alternative<Atom>(ret._v));

    Atom a = std::get<Atom>(ret._v);
    const Number& n = std::get<Number>(a._v);
    EXPECT_TRUE(std::holds_alternative<int>(n._v));

    EXPECT_EQ(std::get<int>(n._v), static_cast<int>(120));
}
