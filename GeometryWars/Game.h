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
	sf::Text m_score;
	sf::Font m_font;
	int m_scoreVal = 0;

public:
	Game(const std::string& configPath);
	void Init(const std::string& configPath);
	void Update();
	void Run();
	//Systems
	void sScore();
	void sSpawnPlayer();
	void sSpawnBullet(float x, float y);
	void sMovement();
	void sRotations();
	void sUserInput();
	void sRender();
	void sEnemySpawner();
	void sCollision();
	void sVertices(const std::shared_ptr<CTransform>& cT, int sides);
};

