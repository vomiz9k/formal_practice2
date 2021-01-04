#pragma once

#include <vector>
#include <unordered_set>
#include <string>
#include <sstream>
#include <fstream>

class grammar
{
    friend class earley;

    struct rule
    {
        std::string left_part = "";
        std::vector<std::string> right_part;
    };

    std::unordered_set<std::string> terminals;
    std::vector<rule> rules;

    bool is_terminal(const std::string &character) const;
    void add_rule(const std::string &rule_str);

public:
    grammar(const std::string &path_to_file);
    grammar() = default;
};