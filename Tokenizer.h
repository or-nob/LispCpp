#pragma once

#include <queue>
#include <string>
#include <vector>

#include "Types.h"

class Tokenizer {
    std::queue<std::string> _programList;

    Exp parse();
    Exp readFromTokensRec(std::queue<std::string>& tokens);
    Atom getAtom(const std::string& t);

public:
    Tokenizer(const std::string& p);
    ~Tokenizer() = default;
    Exp readFromTokens();
};
