#include "Game.h"
#include <iostream>
#include <cmath>
#include <format>

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
	sScore();
}

void Game::Update()
{
	auto& enemies = this->m_entities.GetEntities("enemy");
	for (auto& e : enemies) {
		if (e->IsActive())
			continue;
		sVertices(e->cTransform, e->cShape->circle.getPointCount());
	}
	auto& player = this->m_entities.GetEntities("player");
	if (!player.empty() && !player[0]->IsActive())
		sVertices(player[0]->cTransform, player[0]->cShape->circle.getPointCount());

	std::string text = "Score: " + std::to_string(this->m_scoreVal);
	this->m_score.setString(text);
	this->m_entities.Update();
}

void Game::Run()
{
	while (this->m_running) 
	{
		Update();
		sUserInput();
		if (!this->m_paused) {
			sEnemySpawner();
			sMovement();
			sCollision();
			this->m_frameCounter++;
		}
		sRotations();
		sRender();
	}
}

void Game::sScore()
{
	auto& font = this->m_font;
	if (!font.loadFromFile("RobotRebels.ttf"))
		std::cout << "failed to load font" << std::endl;
	auto& score = this->m_score;
	score.setFont(font);
	std::string text = "Score: " + std::to_string(this->m_scoreVal);
	score.setString(text);
	score.setCharacterSize(20);
	score.setPosition(10, 10);
	score.setFillColor(sf::Color::Red);
}

void Game::sSpawnPlayer()
{
	this->m_player = this->m_entities.AddEntity("player");
	this->m_player->cShape = std::make_shared<CShape>(10.0f, 3, sf::Color::Red, sf::Color::Blue, 2.0f);
	this->m_player->cTransform = std::make_shared<CTransform>(Vec2(this->m_window.getSize().x / 2, this->m_window.getSize().y / 2), Vec2(10, 10), 0);
	this->m_player->cInput = std::make_shared<CInput>();
	this->m_player->cCollision = std::make_shared<CCollision>(10.0f);
	Vec2 pos = this->m_player->cTransform->pos;
	this->m_player->cShape->circle.setPosition(pos.x, pos.y);

}

void Game::sSpawnBullet(float x, float y)
{
	auto bullet = this->m_entities.AddEntity("bullet");
	int S = 10;
	Vec2 mouse(x, y);
	auto& pTransform = this->m_player->cTransform;
	Vec2 D = mouse - pTransform->pos;
	D.Normalize();
	Vec2 velocity(S * D.x, S * D.y);

	bullet->cTransform = std::make_shared<CTransform>(pTransform->pos, velocity, 0);
	bullet->cShape = std::make_shared<CShape>(10.0f, 30, sf::Color::White, sf::Color::White, 0);
	bullet->cCollision = std::make_shared<CCollision>(10.0f);
	bullet->cLifespan = std::make_shared<CLifespan>(100);

	auto& shape = bullet->cShape->circle;
	auto& transform = bullet->cTransform;
	
	shape.setPosition(transform->pos.x, transform->pos.y);
}

void Game::sMovement()
{
	auto winSize = this->m_window.getSize();

	//player
	auto& pInput = this->m_player->cInput;
	auto& pTransform = this->m_player->cTransform;
	float pRad = this->m_player->cShape->circle.getRadius();
	if (pInput->up && pTransform->pos.y - pRad > 0)
		pTransform->pos.y -= pTransform->velocity.y;
	if (pInput->down && pTransform->pos.y + pRad < winSize.y)
		pTransform->pos.y += pTransform->velocity.y;
	if (pInput->left && pTransform->pos.x - pRad > 0)
		pTransform->pos.x -= pTransform->velocity.x;
	if (pInput->right && pTransform->pos.x + pRad < winSize.x)
		pTransform->pos.x += pTransform->velocity.x;
	this->m_player->cShape->circle.setPosition(sf::Vector2f(pTransform->pos.x, pTransform->pos.y));

	//bullet
	auto& bullets = this->m_entities.GetEntities("bullet");
	if (bullets.size() > 0) 
	{
		for (auto& b : bullets) 
		{
			auto& bT = b->cTransform;
			bT->pos.x += bT->velocity.x;
			bT->pos.y += bT->velocity.y;
			b->cShape->circle.setPosition(bT->pos.x, bT->pos.y);
			auto& color = b->cShape->circle.getFillColor();
			auto& lifeSpan = b->cLifespan;
			float opacity = (static_cast<float>(lifeSpan->remaining) / lifeSpan->total);
			b->cShape->circle.setFillColor(sf::Color(color.r, color.g, color.b, 255 * opacity));
			if (b->cLifespan->remaining <= 0)
				b->Destroy();
			b->cLifespan->remaining--;
		}
	}

	//enemy
	auto& enemies = this->m_entities.GetEntities("enemy");
	if (enemies.size() > 0)
	{
		for (auto& e : enemies)
		{
			auto& eT = e->cTransform;
			float r = e->cShape->circle.getRadius();
			if (eT->pos.x - r <= 0 || eT->pos.x + r >= winSize.x)
				eT->velocity.x *= -1;
			if (eT->pos.y - r <= 0|| eT->pos.y + r >= winSize.y)
				eT->velocity.y *= -1;

			eT->pos.x += eT->velocity.x;
			eT->pos.y += eT->velocity.y;

			e->cShape->circle.setPosition(eT->pos.x, eT->pos.y);			
		}
	}

	//vertices
	auto& vertices = this->m_entities.GetEntities("vertex");
	if (vertices.size() > 0) 
	{
		for (auto& v : vertices)
		{
			auto& vT = v->cTransform;
			vT->pos.x += vT->velocity.x;
			vT->pos.y += vT->velocity.y;
			v->cShape->circle.setPosition(vT->pos.x, vT->pos.y);
			auto& color = v->cShape->circle.getFillColor();
			auto& lifeSpan = v->cLifespan;
			float opacity = (static_cast<float>(lifeSpan->remaining) / lifeSpan->total);
			v->cShape->circle.setFillColor(sf::Color(color.r, color.g, color.b, 255 * opacity));
			if (v->cLifespan->remaining <= 0)
				v->Destroy();
			v->cLifespan->remaining--;
		}
	}
}

void Game::sRotations()
{
	auto& entities = this->m_entities.GetEntities();
	for (auto& e : entities)
	{
		auto& eT = e->cTransform;
		eT->angle = eT->angle + 2 >= 360 ? (eT->angle + 2) - 360 : eT->angle + 2;
		e->cShape->circle.setRotation(eT->angle);
	}
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
			case sf::Keyboard::A:
				pInput->left = true;
				break;
			case sf::Keyboard::S:
				pInput->down = true;
				break;
			case sf::Keyboard::D:
				pInput->right = true;
				break;
			case sf::Keyboard::Escape:
				this->m_paused = !this->m_paused;
				break;
			default:
				break;
			}
			break;
		case sf::Event::KeyReleased:
			switch (event.key.code)
			{
			case sf::Keyboard::W:
				pInput->up = false;
				break;
			case sf::Keyboard::A:
				pInput->left = false;
				break;
			case sf::Keyboard::S:
				pInput->down = false;
				break;
			case sf::Keyboard::D:
				pInput->right = false;
				break;
			default:
				break;
			}
			break;
		case sf::Event::MouseButtonPressed:
			switch (event.mouseButton.button)
			{
			case sf::Mouse::Left:
				sSpawnBullet(event.mouseButton.x, event.mouseButton.y);
				break;
			default:
				break;
			}
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
	
	this->m_window.draw(this->m_score);
	this->m_window.display();
}

void Game::sEnemySpawner()
{
	if (this->m_frameCounter % 200 == 0) {
		int randX = std::rand() % 1001 + 100;
		int randY = std::rand() % 501 + 100;
		int randVx = 5;
		int randVy = 3;
		float radius = 30;
		Vec2 pos(randX, randY);
		Vec2 vel(randVx, randVy);
		
		auto enemy = this->m_entities.AddEntity("enemy");

		//Set up components
		enemy->cTransform = std::make_shared<CTransform>(pos, vel, 0);
		enemy->cShape = std::make_shared<CShape>(radius, 5, sf::Color::Yellow, sf::Color::Green, 2.0f);
		enemy->cCollision = std::make_shared<CCollision>(radius);
		enemy->cScore = std::make_shared<CScore>(100);

		enemy->cShape->circle.setPosition(pos.x, pos.y);
		this->m_frameCounter = 0;
	}
}

void Game::sCollision()
{
	auto& pT = this->m_player->cTransform;
	auto pCol = this->m_player->cCollision->radius;
	auto& enemies = this->m_entities.GetEntities("enemy");
	auto& bullets = this->m_entities.GetEntities("bullet");
	for (auto& e : enemies) {
		auto& eT = e->cTransform;
		auto eCol = e->cCollision->radius;
		Vec2 D = pT->pos - eT->pos;
		float distSqr = (D.x * D.x) + (D.y * D.y);
		if (distSqr < (pCol + eCol) * (pCol + eCol)) {
			e->Destroy();
			this->m_player->Destroy();
		}
	}

	for (auto& b : bullets) {
		for (auto& e : enemies) {
			auto& bT = b->cTransform;
			auto bCol = b->cCollision->radius;
			auto& eT = e->cTransform;
			auto eCol = e->cCollision->radius;
			Vec2 D = bT->pos - eT->pos;
			float distSqr = (D.x * D.x) + (D.y * D.y);
			if (distSqr < (bCol + eCol) * (bCol + eCol)) {
				e->Destroy();
				b->Destroy();
				this->m_scoreVal += e->cScore->score;
			}
		}
	}
}

void Game::sVertices(const std::shared_ptr<CTransform>& cT, int sides)
{
	int speed = 10;
	float angle = cT->angle;
	float baseAngle = 360.0f / sides;
	Vec2 ePos = cT->pos;
	const float PI = 3.14159265359;
	float radius = 5.0f;

	for (int i = 1; i <= sides; i++) {
		auto ver = this->m_entities.AddEntity("vertex");

		//calculate angle
		float vAngle = baseAngle*i + angle > 360 ? (baseAngle*i + angle) - 360 : (baseAngle*i + angle);
		float rad = vAngle * (PI / 180);
		float x = cos(rad);
		float y = sin(rad);
		Vec2 vel(speed * x, speed * y);

		//set up components
		ver->cTransform = std::make_shared<CTransform>(cT->pos, vel, 0);
		ver->cLifespan = std::make_shared<CLifespan>(50);
		ver->cShape = std::make_shared<CShape>(radius, sides, sf::Color::Cyan, sf::Color::Black, 0);

		ver->cShape->circle.setPosition(ePos.x, ePos.y);
	}
}
