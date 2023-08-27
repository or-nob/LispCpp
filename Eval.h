#pragma once

#include "Env.h"

class Eval {
public:
    Eval() = default;
    static Exp eval(Exp e, Env& env);
};
