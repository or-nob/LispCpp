#include <iostream>
#include <unordered_map>
#include <vector>

#include "Eval.h"
#include "Tokenizer.h"
#include "Types.h"

int main(void) {
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

    // Exp::print(Eval::eval(t.readFromTokens(), e));
    // Exp::print(Eval::eval(t1.readFromTokens(), e));

    Env e{};
    for (std::string line; std::getline(std::cin, line);) {
        Tokenizer t{line};
        Exp::print(Eval::eval(t.readFromTokens(), e));
        // e.printKeys();
    }

    return 0;
}
