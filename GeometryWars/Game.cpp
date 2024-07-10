#include "Game.h"
#include <iostream>

Game::Game(const std::string& configPath)
{
	Init(configPath);
}

void Game::Init(const std::string& configPath)
{
	std::srand(static_cast<unsigned int>(std::time(nullptr)));
	//read config here
	this->m_window.create(sf::VideoMode(1280, 720), "Geometry Wars");
	this->m_window.setFramerateLimit(60);
	sSpawnPlayer();
}

void Game::Run()
{
	while (this->m_running) {
		this->m_entities.Update();

		sEnemySpawner();
		sRender();
		this->m_frameCounter++;
	}
}

void Game::sSpawnPlayer()
{
	this->m_player = this->m_entities.AddEntity("player");
	this->m_player->cShape = std::make_shared<CShape>(10.0f, 3, sf::Color::Red, sf::Color::Blue, 2.0f);
	this->m_player->cShape->circle.setPosition(this->m_window.getSize().x / 2, this->m_window.getSize().y / 2);
}

void Game::sRender()
{
	this->m_window.clear();
	auto entities = this->m_entities.GetEntities();
	for (auto& e : entities) {
		this->m_window.draw(e->cShape->circle);
	}
	this->m_window.display();
}

void Game::sEnemySpawner()
{
	if (this->m_frameCounter % 200 == 0) {
		int randX = std::rand() % 1001 + 100;
		int randY = std::rand() % 501 + 100;
		auto enemy = this->m_entities.AddEntity("enemy");
		enemy->cShape = std::make_shared<CShape>(10.0f, 3, sf::Color::Yellow, sf::Color::Green, 2.0f);
		enemy->cShape->circle.setPosition(randX, randY);
		this->m_frameCounter = 0;
	}
}
