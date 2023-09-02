#include <iostream>
#include <unordered_map>
#include <vector>

#include "Eval.h"
#include "Tokenizer.h"
#include "Types.h"

int main(void) {
    Env e{};
    // Tokenizer t{"(begin (define r 10) (* pi (* r r)))"};
    // Tokenizer t{"(begin (+ 10.5 10))"};
    // Tokenizer t{"(begin (define r 50) (/ r 5.1))"};
    // Tokenizer t{"(define circle-area (lambda (r) (* 3.14 (* r r))))"};
    // Tokenizer t{"(begin (if (> 20 10) (+ 1 1) (- 3 3)))"};
    // Tokenizer t{"(begin (if (>= 20 20) (+ 1 1) (abs -3.1)))"};
    // Tokenizer t{"(begin (define r 5) (if (= r 5) (+ r 5.1) (+ r 5)))"};
    // Tokenizer t{"(begin (define r 5) (if (= r 5) (+ 1 1) (abs -3.1)))"};
    // Tokenizer t{"(+ 10 10)"};

    // Tokenizer t{"(define circle-area (lambda (r) (* 3.14 (* r r))))"};

    // Tokenizer t1{"(circle-area 5)"};
    // Eval::eval(t.readFromTokens(), e);

    // const auto& l = t.readFromTokens()._v._l;
    // for (size_t i = 1; i < l.size(); ++i) {
    //     std::cout << std::holds_alternative<Number>(std::any_cast<Exp>(l[i])._v._a._v);
    // }
    // for ()
    // Exp::print(Eval::eval(t.readFromTokens(), e));
    // Exp::print(Eval::eval(t1.readFromTokens(), e));

    for (std::string line; std::getline(std::cin, line);) {
        Tokenizer t{line};
        Exp::print(Eval::eval(t.readFromTokens(), e));
        // e.printKeys();
    }

    return 0;
}
