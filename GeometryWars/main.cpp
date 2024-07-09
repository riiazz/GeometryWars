#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>
#include <sstream>
#include "main.h"
#include "Vec2.h"

int main()
{
    Vec2 a(4, 3);
    Vec2 b(4, 3);
    int f;
    std::cout << (a == b) << std::endl;
    std::cin >> f;
    return 0;
}
