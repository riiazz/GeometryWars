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
    Game g("config.txt");
    g.Run();
    return 0;
}
