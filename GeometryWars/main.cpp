#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>
#include <sstream>
#include "main.h"
#include "Vec2.h"
#include "EntityManager.h"
#include "Entity.h"

int main()
{
    EntityManager test;
    auto a = test.AddEntity("test");
    auto b = test.AddEntity("test");
    std::cout << a->Id() << " " << a->Tag() << " " << a->IsActive() << std::endl;
    std::cout << b->Id() << " " << b->Tag() << " " << b->IsActive() << std::endl;
    auto c = test.getEntities();
    std::cout << c.size() << std::endl;
    test.Update();
    c = test.getEntities();
    std::cout << c.size() << std::endl;
    return 0;
}
