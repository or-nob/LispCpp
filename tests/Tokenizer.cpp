#include "../Tokenizer.h"

#include "gtest/gtest.h"

TEST(Tokenizer, simpleExpr) {
    Tokenizer t{"(+ 10 10)"};
    Exp e = t.readFromTokens();

    EXPECT_TRUE(std::holds_alternative<Exp::ExpList>(e._v));
    const auto& eL = std::get<Exp::ExpList>(e._v);
    for (size_t i = 0; i < eL.size(); ++i) {
        const auto& e = eL[i];
        EXPECT_NO_THROW({ std::any_cast<Exp>(e); });
        auto exp = std::any_cast<Exp>(e);

        EXPECT_TRUE(std::holds_alternative<Atom>(exp._v));
        Atom a = std::get<Atom>(exp._v);

        if (i == 0) {
            EXPECT_TRUE(std::holds_alternative<std::string>(a._v));
            EXPECT_EQ(std::get<std::string>(a._v), "+");
        }

        if (i != 0) {
            EXPECT_TRUE(std::holds_alternative<Number>(a._v));
            Number n = std::get<Number>(a._v);
            EXPECT_TRUE(std::holds_alternative<int>(n._v));
        }
    }
}

TEST(Tokenizer, largeExpr) {
    Tokenizer t{"(begin (define r 50) (if (> r 10) (+ 1 1) (- 3 3)))"};
    Exp e = t.readFromTokens();

    EXPECT_TRUE(std::holds_alternative<Exp::ExpList>(e._v));
    const auto& l = std::get<Exp::ExpList>(e._v);
    EXPECT_EQ(l.size(), 3);

    EXPECT_NO_THROW(std::any_cast<Exp>(l[0]));
    const auto& f = std::any_cast<Exp>(l[0]);
    EXPECT_TRUE(std::holds_alternative<Atom>(f._v));
    Atom a = std::get<Atom>(f._v);
    EXPECT_TRUE(std::holds_alternative<std::string>(a._v));
    EXPECT_EQ(std::get<std::string>(a._v), "begin");

    EXPECT_NO_THROW(std::any_cast<Exp>(l[1]));
    const auto& s = std::any_cast<Exp>(l[1]);
    EXPECT_TRUE(std::holds_alternative<Exp::ExpList>(s._v));
    const auto& sl = std::get<Exp::ExpList>(s._v);
    EXPECT_EQ(sl.size(), 3);

    EXPECT_NO_THROW(std::any_cast<Exp>(l[2]));
    const auto& tr = std::any_cast<Exp>(l[2]);
    EXPECT_TRUE(std::holds_alternative<Exp::ExpList>(tr._v));
    const auto& trl = std::get<Exp::ExpList>(tr._v);
    EXPECT_EQ(trl.size(), 4);
}
