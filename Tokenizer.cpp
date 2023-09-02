#include "Tokenizer.h"

#include <iostream>
#include <string>

Tokenizer::Tokenizer(const std::string& p) {
    std::string program = p;
    auto replace = [&program](const std::string& f, const std::string& s) {
        size_t pos = 0;
        while ((pos = program.find(f, pos)) != std::string::npos) {
            program.replace(pos, f.length(), s);
            pos += s.length();
        }
    };

    replace("(", "( ");
    replace(")", " )");

    program = " " + program + " ";

    size_t fPos = 0, sPos = 0;
    fPos = program.find(' ', 0);
    while ((sPos = program.find(' ', fPos + 1)) != std::string::npos) {
        auto s = program.substr(fPos + 1, sPos - fPos - 1);
        _programList.push(s);
        fPos = sPos;
    }
}

Exp Tokenizer::readFromTokens() { return readFromTokensRec(_programList); }

Exp Tokenizer::readFromTokensRec(std::queue<std::string>& tokens) {
    if (tokens.empty()) return {};
    std::string t = tokens.front();
    tokens.pop();

    if (t == "(") {
        Exp e;
        e._t = 1;
        while (tokens.front() != ")") e._v._l.emplace_back(readFromTokensRec(tokens));
        tokens.pop();
        return e;
    } else if (t == ")") {
        return {};
    } else {
        return Exp{Exp::ExpType{._a = getAtom(t)}, 0};
    }
}

Atom Tokenizer::getAtom(const std::string& t) {
    Atom ret;
    try {
        int i = std::stoi(t);
        float f = std::stof(t);
        ret._v = Number{._v = (i == f ? i : f)};
    } catch (...) {
        std::get<std::string>(ret._v) = t;
    }

    return ret;
}
