#include <iostream>
#include <string>

#define _TEST

#include "earley.hpp"

int main()
{
#ifdef _TEST
    grammar::test();
    earley::test();
#endif

    //example:
    try
    {
        grammar G("example.txt");
        std::cout << earley().check(G, "x + x / x") << ' ' << earley().check(G, "x / + x");
    }
    catch (std::runtime_error &err)
    {
        std::cout << "ERROR! " << err.what() << '\n';
        return 1;
    }
}