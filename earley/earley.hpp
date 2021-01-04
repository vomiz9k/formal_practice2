#pragma once

#include "grammar.hpp"

class earley
{
    const std::string S = "_ ";

    struct situation : public grammar::rule
    {
        size_t dot;
        size_t index;

        situation with_incremented_dot();

        situation(const grammar::rule &rule, size_t _dot, size_t _index);

        bool completed();

        situation() = default;
    };

    friend bool operator==(const situation &first, const situation &second);

    std::vector<std::vector<situation>> situations;
    std::vector<std::string> characters;
    grammar G;

    situation init_first_situation();

    void add_situation(size_t index, situation new_situation);

    void scan(size_t index);

    void complete(size_t index);

    void update_completed_situation(size_t index, situation &completed_situation);

    void predict(size_t index);

    void get_situations_from_rules(size_t index, std::string &character);

    void init_earley_data(const grammar &gram, const std::string &word);

public:
    bool predict(const grammar &gram, const std::string &word);

    earley() = default;
};
