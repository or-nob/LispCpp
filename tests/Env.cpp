#include "../Env.h"

#include "../Tokenizer.h"
#include "gtest/gtest.h"

TEST(Env, envFindCheck) {
    Env e{};
    EXPECT_THROW(e.find("dummy"), std::runtime_error);
    EXPECT_NO_THROW(e.find("begin"));
}

TEST(Env, envFindCheckWithParams) {
    Env o{};
    Env e1{{"c"}, {{Atom{._v = Number{._v = 1}}}}, o};
    EXPECT_NO_THROW(e1.find("c"));
}
