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
	sSpawnBullet();
}

void Game::Run()
{
	while (this->m_running) 
	{
		this->m_entities.Update();
		sUserInput();
		sEnemySpawner();
		sRender();
		this->m_frameCounter++;
	}
}

void Game::sSpawnPlayer()
{
	this->m_player = this->m_entities.AddEntity("player");
	this->m_player->cShape = std::make_shared<CShape>(10.0f, 3, sf::Color::Red, sf::Color::Blue, 2.0f);
	this->m_player->cTransform = std::make_shared<CTransform>(Vec2(this->m_window.getSize().x / 2, this->m_window.getSize().y / 2), Vec2(0, 0), 0);
	this->m_player->cInput = std::make_shared<CInput>();
	this->m_player->cCollision = std::make_shared<CCollision>(10.0f);
	Vec2 pos = this->m_player->cTransform->pos;
	this->m_player->cShape->circle.setPosition(pos.x, pos.y);

}

void Game::sSpawnBullet()
{
	auto bullet = this->m_entities.AddEntity("bullet");
	auto& pTransform = this->m_player->cTransform;
	bullet->cTransform = std::make_shared<CTransform>(pTransform->pos, Vec2(1, 1), 0);
	bullet->cShape = std::make_shared<CShape>(10.0f, 30, sf::Color::White, sf::Color::White, 0);
	bullet->cCollision = std::make_shared<CCollision>(10.0f);
	bullet->cLifespan = std::make_shared<CLifespan>(100);

	auto& shape = bullet->cShape->circle;
	auto& transform = bullet->cTransform;
	
	shape.setPosition(transform->pos.x, transform->pos.y);
}

void Game::sUserInput()
{
	sf::Event event;
	auto& pInput = this->m_player->cInput;
	while (this->m_window.pollEvent(event)) 
	{
		switch (event.type)
		{
		case sf::Event::Closed:
			this->m_window.close();
			break;
		case sf::Event::KeyPressed:
			switch (event.key.code)
			{
			case sf::Keyboard::W:
				pInput->up = true;
				break;
			default:
				break;
			}
			break;
		case sf::Event::KeyReleased:
			break;
		default:
			break;
		}
	}
}

void Game::sRender()
{
	this->m_window.clear();
	auto& entities = this->m_entities.GetEntities();
	for (auto& e : entities) 
	{
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
