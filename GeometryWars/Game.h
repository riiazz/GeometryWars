#pragma once
#include <SFML/Graphics.hpp>
#include "EntityManager.h"

struct WindowCon {
	int width = 0;
	int height = 0;
	int frameLimit = 0;
	int fullscreen = 0;
};

struct FontCon {
	std::string path;
	int size = 0;
	int r, g, b;
};

struct PlayerCon {
	int shapeRadius = 0;
	int colRadius = 0;
	float speed = 0.0f;
	int fR, fG, fB;
	int oR, oG, oB;
	int outlineThickness = 0;
	int shapeVertices = 0;
};

struct EnemyCon {
	int shapeRadius = 0;
	int colRadius = 0;
	float sMin, sMax;
	int oR, oG, oB;
	int outlineThickness = 0;
	int vMin, vMax;
	int smolLifespan = 0;
	int spawnInterval = 0;
};

struct BulletCon {
	int shapeRadius = 0;
	int colRadius = 0;
	float speed = 0.0f;
	int fR, fG, fB;
	int oR, oG, oB;
	int outlineThickness = 0;
	int shapeVertices = 0;
	int lifespan = 0;
};

class Game
{
	//Config
	WindowCon m_winCon;
	FontCon m_fontCon;
	PlayerCon m_playCon;
	EnemyCon m_enemyCon;
	BulletCon m_bulletCon;
	
	//Game Property
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

