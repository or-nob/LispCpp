#pragma once

// #include "Env.h"
// #include "Eval.h"
// #include "Types.h"
//
// struct Procedure : Exp {
//     Procedure(const std::vector<std::string>& params, const Exp& body, Env& env) : Exp() {
//         _v = {._l = {std::function<Exp(std::vector<Exp>)>{
//                   [this, params, body, &env](std::vector<Exp> expList) -> Exp {
//                       Env e{params, expList, env};
//                       return Eval::eval(body, e);
//                   }}}};
//         _t = 1;
//     }
// };
