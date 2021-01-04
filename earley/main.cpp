#include <iostream>
#include "earley.hpp"

int main()
{
    grammar G("in.txt");
    std::cout << earley().predict(G, "1 2 3 4 5 6 7 8 9 + ( 1 )");
}