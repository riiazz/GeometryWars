#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>
#include <sstream>
#include "main.h"
#include "Vec2.h"
#include "EntityManager.h"
#include "Entity.h"
#include "Game.h"

int main()
{
    /*EntityManager test;
    auto a = test.AddEntity("test");
    auto b = test.AddEntity("test2");
    std::cout << a->Id() << " " << a->Tag() << " " << a->IsActive() << std::endl;
    std::cout << b->Id() << " " << b->Tag() << " " << b->IsActive() << std::endl;
    auto c = test.GetEntities();
    std::cout << c.size() << std::endl;
    test.Update();
    c = test.GetEntities();
    std::cout << c.size() << std::endl;
    c = test.GetEntities("test3");
    std::cout << c.size() << std::endl;*/
    Game g("test");
    g.Run();
    return 0;
}
