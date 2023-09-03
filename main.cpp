#include <iostream>
#include <unordered_map>
#include <vector>

#include "Eval.h"
#include "Tokenizer.h"
#include "Types.h"

int main(void) {
    Env e{};
    for (std::string line; std::getline(std::cin, line);) {
        Tokenizer t{line};
        Exp::print(Eval::eval(t.readFromTokens(), std::forward<Env>(e)));
    }

    return 0;
}
