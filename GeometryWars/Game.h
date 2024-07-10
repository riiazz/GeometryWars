#pragma once
#include <SFML/Graphics.hpp>
#include "EntityManager.h"

class Game
{
	sf::RenderWindow m_window;
	EntityManager m_entities;
	std::shared_ptr<Entity> m_player;
	bool m_paused = false;
	bool m_running = true;
	int m_frameCounter = 0;

public:
	Game(const std::string& configPath);
	void Init(const std::string& configPath);
	void Update();
	void Run();
	//Systems
	void sSpawnPlayer();
	void sMovement();
	void sUserInput();
	void sRender();
	void sEnemySpawner();
	void sCollision();
};

