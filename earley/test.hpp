#ifdef _TEST
#include "earley.hpp"
void grammar::test()
{
    try
    {
        grammar G;
        G.add_rule("S -> A");
        if (G.rules.size() != 1 || G.rules[0].left_part != "S" 
            || G.rules[0].right_part.size() != 1 || G.rules[0].right_part[0] != "A" 
            || G.terminals.size() != 1 || G.terminals.count("S") != 1)
        {
            throw std::runtime_error("Bad rule adding");
        }

        G.add_rule("A -> B C");
        if (G.rules.size() != 2 || G.rules[1].left_part != "A" 
            || G.rules[1].right_part.size() != 2 || G.rules[1].right_part[0] != "B" 
            || G.rules[1].right_part[1] != "C" || G.terminals.size() != 2 
            || G.terminals.count("A") != 1)
        {
            throw std::runtime_error("Bad rule adding");
        }

        if (G.is_terminal("S") != true || G.is_terminal("A") != true 
            || G.is_terminal("B") != false || G.is_terminal("C") != false)
        {
            throw std::runtime_error("Bad is_terminal function");
        }

        bool incorrect_rule_catched = false;
        try
        {
            G.add_rule("Hello, world!");
        }
        catch (std::runtime_error &err)
        {
            if (std::string(err.what()) == "Bad grammar syntax")
            {
                incorrect_rule_catched = true;
            }
        }

        if (!incorrect_rule_catched)
        {
            throw std::runtime_error("Bad grammar syntax was not catched");
        }
    }
    catch (std::runtime_error &err)
    {
        std::cout << "ERROR! " << err.what() << '\n';
    }
}

void earley::test_add_situation()
{
    grammar G;
    G.add_rule("S -> A B");
    G.add_rule("A -> some");
    G.add_rule("B -> word");
    std::string word = "some word";

    earley Algo;
    Algo.init_earley_data(G, word);

    situation sit1(G.rules[0], 0, 0);

    Algo.add_situation(0, sit1);
    if (Algo.situations[0].size() != 2 || !(Algo.situations[0][1] == sit1))
    {
        throw std::runtime_error("Bad add_situation func");
    }

    Algo.add_situation(0, sit1);
    if (Algo.situations[0].size() != 2 || !(Algo.situations[0][1] == sit1))
    {
        throw std::runtime_error("Bad add_situation func");
    }
}

void earley::test_scan()
{
    grammar G;
    G.add_rule("S -> A B");
    G.add_rule("A -> some");
    G.add_rule("B -> word");
    std::string word = "some word";

    earley Algo;
    Algo.init_earley_data(G, word);

    Algo.add_situation(0, situation(G.rules[1], 0, 0));
    Algo.scan(1);

    if (Algo.situations[1].size() != 1 || !(Algo.situations[1][0] == situation(G.rules[1], 1, 0)))
    {
        throw std::runtime_error("Bad scan func");
    }
}

void earley::test_update_completed_situations()
{
    grammar G;
    G.add_rule("S -> A B");
    G.add_rule("A -> some");
    G.add_rule("B -> word");
    std::string word = "some word";

    earley Algo;
    Algo.init_earley_data(G, word);

    Algo.add_situation(0, situation(G.rules[1], 0, 0));
    Algo.add_situation(0, situation(G.rules[0], 0, 0));

    Algo.scan(1);
    Algo.update_completed_situation(1, Algo.situations[1][0]);

    if (Algo.situations[1][1].left_part != "S" || Algo.situations[1][1].right_part.size() != 2 
        || Algo.situations[1][1].right_part[0] != "A" || Algo.situations[1][1].right_part[1] != "B" 
        || Algo.situations[1][1].dot != 1 || Algo.situations[1][1].index != 0)
    {
        throw std::runtime_error("Bad update_completed_situations func");
    }
}

void earley::test_complete()
{
    grammar G;
    G.add_rule("S -> A B");
    G.add_rule("A -> some");
    G.add_rule("B -> word");
    std::string word = "some word";

    earley Algo;
    Algo.init_earley_data(G, word);

    Algo.add_situation(0, situation(G.rules[0], 0, 0));
    Algo.add_situation(1, situation(G.rules[1], 1, 0));
    Algo.complete(1);

    if (Algo.situations[1][1].left_part != "S" || Algo.situations[1][1].right_part.size() != 2 
        || Algo.situations[1][1].right_part[0] != "A" || Algo.situations[1][1].right_part[1] != "B"
        || Algo.situations[1][1].dot != 1 || Algo.situations[1][1].index != 0)
    {
        throw std::runtime_error("Bad complete func");
    }
}

void earley::test_get_situations_from_rules()
{
    grammar G;
    G.add_rule("S -> A B");
    G.add_rule("A -> some");
    G.add_rule("B -> word");
    std::string word = "some word";

    earley Algo;
    Algo.init_earley_data(G, word);

    Algo.get_situations_from_rules(1, "A");
    if (!(Algo.situations[1][0] == situation(G.rules[1], 0, 1)))
    {
        throw std::runtime_error("Bad get_situations_from_rules func");
    }
}

void earley::test_predict()
{
    grammar G;
    G.add_rule("S -> A B");
    G.add_rule("A -> some");
    G.add_rule("B -> word");
    std::string word = "some word";

    earley Algo;
    Algo.init_earley_data(G, word);

    Algo.add_situation(1, situation(G.rules[0], 1, 0));
    Algo.predict(1);

    if (!(Algo.situations[1][1] == situation(G.rules[2], 0, 1)))
    {
        throw std::runtime_error("Bad get_situations_from_rules func");
    }
}

void earley::test_check()
{
    grammar G("test/test1");
    if (!earley().check(G, "some word") || earley().check(G, "some") 
        || earley().check(G, "word") || earley().check(G, "A") 
        || earley().check(G, "B") || earley().check(G, "A B") 
        || earley().check(G, "S") || earley().check(G, "abrakadabra"))
    {
        throw std::runtime_error("Bad checking func");
    }

    grammar MATH("test/test2");
    std::string word;
    std::string result;
    std::ifstream in("test/test2_input");
    while (std::getline(in, word))
    {
        std::getline(in, result);
        if (earley().check(MATH, word) != result[0] - '0')
        {
            throw std::runtime_error("Bad checking func");
        }
    }
}

void earley::test()
{
    try
    {
        test_add_situation();
        test_scan();
        test_update_completed_situations();
        test_complete();
        test_get_situations_from_rules();
        test_predict();
        test_check();
    }
    catch (std::runtime_error &err)
    {
        std::cout << "ERROR! " << err.what() << '\n';
    }
}

#endif