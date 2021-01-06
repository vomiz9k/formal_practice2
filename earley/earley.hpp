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

        situation(grammar::rule rule, size_t _dot, size_t _index);

        bool completed() const; 

        situation() = default;
    };

    friend bool operator==(const situation &first, const situation &second);

    std::vector<std::vector<situation>> situations;
    std::vector<std::string> characters;
    grammar G;

    void init_first_situation();

    void add_situation(size_t index, const situation& new_situation);

    void scan(size_t index);

    void complete(size_t index);

    void update_completed_situation(size_t index, const situation &completed_situation);

    void predict(size_t index);

    void get_situations_from_rules(size_t index, const std::string &character);

    void init_earley_data(const grammar &gram, const std::string &word);

public:
    bool check(const grammar &gram, const std::string &word);

    earley() = default;
#ifdef _TEST
    static void test();
    static void test_add_situation();
    static void test_scan();

    static void test_complete();
    static void test_update_completed_situations();

    static void test_predict();
    static void test_get_situations_from_rules();

    static void test_check();
#endif
};

#ifdef _TEST
#include "test.hpp"
#endif
