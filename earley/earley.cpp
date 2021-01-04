#include "earley.hpp"

earley::situation earley::situation::with_incremented_dot()
{
    situation result = *this;
    ++result.dot;
    return result;
}

earley::situation::situation(const grammar::rule &rule, size_t _dot, size_t _index) : dot(_dot), index(_index)
{
    right_part = rule.right_part;
    left_part = rule.left_part;
}

bool earley::situation::completed()
{
    return dot >= right_part.size();
}

earley::situation earley::init_first_situation()
{
    grammar::rule first_rule;
    first_rule.left_part = S;
    first_rule.right_part.push_back(G.rules[0].left_part);
    G.rules.push_back(first_rule);

    situation first_situation(first_rule, 0, 0);
    return first_situation;
}

void earley::add_situation(size_t index, situation new_situation)
{
    for (auto &situation : situations[index])
    {
        if (situation == new_situation)
            return;
    }
    situations[index].push_back(new_situation);
}

void earley::scan(size_t index)
{
    if (index == 0)
        return;

    for (auto &situation : situations[index - 1])
    {
        if (!situation.completed() && situation.right_part[situation.dot] == characters[index - 1])
        {
            add_situation(index, situation.with_incremented_dot());
        }
    }
}

void earley::complete(size_t index)
{
    for (size_t i = 0; i < situations[index].size(); ++i)
    {
        situation curr_situation = situations[index][i];
        if (curr_situation.completed())
        {
            update_completed_situation(index, curr_situation);
        }
    }
}

void earley::update_completed_situation(size_t index, situation &completed_situation)
{
    for (auto &curr_situation : situations[completed_situation.index])
    {
        if (!curr_situation.completed() && curr_situation.right_part[curr_situation.dot] == completed_situation.left_part)
        {
            add_situation(index, curr_situation.with_incremented_dot());
        }
    }
}

void earley::predict(size_t index)
{
    for (size_t i = 0; i < situations[index].size(); ++i)
    {
        situation curr_situation = situations[index][i];
        if (!curr_situation.completed() && G.is_terminal(curr_situation.right_part[curr_situation.dot]))
        {
            get_situations_from_rules(index, curr_situation.right_part[curr_situation.dot]);
        }
    }
}

void earley::get_situations_from_rules(size_t index, std::string &character)
{
    for (const grammar::rule &rule : G.rules)
    {
        if (rule.left_part == character)
        {
            situation new_situation(rule, 0, index);
            add_situation(index, new_situation);
        }
    }
}

void earley::init_earley_data(const grammar &gram, const std::string &word)
{
    G = gram;

    std::stringstream character_stream(word);
    std::string character;

    while (character_stream >> character)
    {
        characters.push_back(character);
    }

    situations.resize(characters.size() + 1);
    situations[0].push_back(init_first_situation());
}

bool earley::predict(const grammar &gram, const std::string &word)
{
    init_earley_data(gram, word);

    for (size_t i = 0; i <= characters.size(); ++i)
    {
        scan(i);
        size_t prev_len = -1;
        while (prev_len != situations[i].size())
        {
            prev_len = situations[i].size();
            complete(i);
            predict(i);
        }
    }

    for (auto &situation : situations[characters.size()])
    {
        if (situation.left_part == S && situation.right_part[0] == G.rules[0].left_part && situation.dot == 1 && situation.index == 0)
            return true;
    }

    return false;
}

bool operator==(const earley::situation &first, const earley::situation &second)
{
    if (first.dot == second.dot && first.index == second.index && first.left_part == second.left_part && first.right_part.size() == second.right_part.size())
    {
        for (size_t i = 0; i < first.right_part.size(); ++i)
        {
            if (first.right_part[i] != second.right_part[i])
                return false;
        }
        return true;
    }
    return false;
}