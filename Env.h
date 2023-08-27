#pragma once

#include <any>
#include <functional>
#include <string>
#include <unordered_map>

struct Exp;

class Env {
    std::unordered_map<std::string, Exp> _env;
    Env* _outer = NULL;

public:
    Env();
    Env(const std::vector<std::string>& keys, const std::vector<Exp>& args, Env& outer);
    void init();
    Exp& operator[](const std::string& k);
    Env& find(const std::string& k);
    void printKeys();
};
