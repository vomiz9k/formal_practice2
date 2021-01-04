#include "grammar.hpp"

bool grammar::is_terminal(const std::string &character) const
{
    return terminals.count(character);
}

void grammar::add_rule(const std::string &rule_str)
{
    std::stringstream rule_stream(rule_str);
    std::string character;

    rule curr_rule;
    rule_stream >> character;
    terminals.insert(character);

    curr_rule.left_part = character;

    rule_stream >> character;
    if (character != "->")
        throw std::runtime_error("Bad grammar syntax");

    while (rule_stream >> character)
        curr_rule.right_part.push_back(character);

    rules.emplace_back(curr_rule);
}

//public:
grammar::grammar(const std::string &path_to_file)
{
    std::ifstream in(path_to_file);
    std::string curr_line;
    while (std::getline(in, curr_line))
        add_rule(curr_line);
}