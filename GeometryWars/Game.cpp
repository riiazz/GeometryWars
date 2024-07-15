#include "Game.h"
#include <iostream>
#include <cmath>
#include <fstream>
#include <sstream>

Game::Game(const std::string& configPath)
{
	Init(configPath);
}

void Game::Init(const std::string& configPath)
{
	//seed rand
	std::srand(static_cast<unsigned int>(std::time(nullptr)));

	//read config here
	sReadConfig(configPath);

	//setup window
	if (this->m_winCon.fullscreen == 1) {
		this->m_window.create(sf::VideoMode(this->m_winCon.width, this->m_winCon.height), "Geometry Wars", sf::Style::Fullscreen);
	}
	else {
		this->m_window.create(sf::VideoMode(this->m_winCon.width, this->m_winCon.height), "Geometry Wars");
	}
	this->m_window.setFramerateLimit(m_winCon.frameLimit);

	//Spawn Entity
	sSpawnPlayer();
	sScore();
}

void Game::sReadConfig(const std::string& configPath)
{
	std::ifstream fileStream(configPath);
	if (!fileStream.is_open())
		std::cout << "Failed to open the file!!!" << std::endl;
	std::string line;
	while (std::getline(fileStream, line)) {
		std::istringstream s(line);
		std::string type;
		s >> type;
		if (type == "Window") {
			s >> this->m_winCon.width;
			s >> this->m_winCon.height;
			s >> this->m_winCon.frameLimit;
			s >> this->m_winCon.fullscreen;
		}
		else if (type == "Font") {
			s >> this->m_fontCon.path;
			s >> this->m_fontCon.size;
			s >> this->m_fontCon.r >> this->m_fontCon.g >> this->m_fontCon.b;
		}
		else if (type == "Player") {
			s >> this->m_playCon.shapeRadius;
			s >> this->m_playCon.colRadius;
			s >> this->m_playCon.speed;
			s >> this->m_playCon.fR >> this->m_playCon.fG >> this->m_playCon.fB;
			s >> this->m_playCon.oR >> this->m_playCon.oG >> this->m_playCon.oB;
			s >> this->m_playCon.outlineThickness;
			s >> this->m_playCon.shapeVertices;
		}
		else if (type == "Enemy") {
			s >> this->m_enemyCon.shapeRadius;
			s >> this->m_enemyCon.colRadius;
			s >> this->m_enemyCon.sMin >> this->m_enemyCon.sMax;
			s >> this->m_enemyCon.oR >> this->m_enemyCon.oG >> this->m_enemyCon.oB;
			s >> this->m_enemyCon.outlineThickness;
			s >> this->m_enemyCon.vMin >> this->m_enemyCon.vMax;
			s >> this->m_enemyCon.smolLifespan;
			s >> this->m_enemyCon.spawnInterval;
		}
		else if (type == "Bullet") {
			s >> this->m_bulletCon.shapeRadius;
			s >> this->m_bulletCon.colRadius;
			s >> this->m_bulletCon.speed;
			s >> this->m_bulletCon.fR >> this->m_bulletCon.fG >> this->m_bulletCon.fB;
			s >> this->m_bulletCon.oR >> this->m_bulletCon.oG >> this->m_bulletCon.oB;
			s >> this->m_bulletCon.outlineThickness;
			s >> this->m_bulletCon.shapeVertices;
			s >> this->m_bulletCon.lifespan;
		}
	}
	fileStream.close();
}

void Game::Update()
{
	auto& enemies = this->m_entities.GetEntities("enemy");
	for (auto& e : enemies) {
		if (e->IsActive())
			continue;
		sVertices(e);
	}
	auto& player = this->m_entities.GetEntities("player");
	if (!player.empty() && !player[0]->IsActive())
		sVertices(player[0]);

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
	if (!font.loadFromFile(this->m_fontCon.path))
		std::cout << "failed to load font" << std::endl;
	auto& score = this->m_score;
	score.setFont(font);
	std::string text = "Score: " + std::to_string(this->m_scoreVal);
	score.setString(text);
	score.setCharacterSize(this->m_fontCon.size);
	score.setPosition(10, 10);
	sf::Color color(this->m_fontCon.r, this->m_fontCon.g, this->m_fontCon.b);
	score.setFillColor(color);
}

void Game::sSpawnPlayer()
{
	auto& con = this->m_playCon;
	this->m_player = this->m_entities.AddEntity("player");
	sf::Color fillColor(con.fR, con.fG, con.fB);
	sf::Color outColor(con.oR, con.oG, con.oB);
	this->m_player->cShape = std::make_shared<CShape>(con.shapeRadius, con.shapeVertices, fillColor, outColor, con.outlineThickness);
	Vec2 vel(con.speed, con.speed);
	this->m_player->cTransform = std::make_shared<CTransform>(Vec2(this->m_window.getSize().x / 2, this->m_window.getSize().y / 2), vel, 0);
	this->m_player->cInput = std::make_shared<CInput>();
	this->m_player->cCollision = std::make_shared<CCollision>(con.colRadius);
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
			auto& outColor = v->cShape->circle.getOutlineColor();
			auto& lifeSpan = v->cLifespan;
			float opacity = (static_cast<float>(lifeSpan->remaining) / lifeSpan->total);
			v->cShape->circle.setFillColor(sf::Color(color.r, color.g, color.b, 255 * opacity));
			v->cShape->circle.setOutlineColor(sf::Color(outColor.r, outColor.g, outColor.b, 255 * opacity));
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
			this->m_running = false;
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
			case sf::Keyboard::E:
				this->m_running = false;
				this->m_window.close();
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
	if (this->m_frameCounter % this->m_enemyCon.spawnInterval == 0) {
		auto& con = this->m_enemyCon;
		int randX = std::rand() % 1001 + 100;
		int randY = std::rand() % 501 + 100;
		int diff = 1 + con.sMax - con.sMin;
		int randV = std::rand() % diff + con.sMin;
		float radius = con.shapeRadius;
		float colRadius = con.colRadius;
		Vec2 pos(randX, randY);
		Vec2 vel(randV, randV);
		int rR = std::rand() % (1 + 255 - 0) + 0;
		int rG = std::rand() % (1 + 255 - 0) + 0;
		int rB = std::rand() % (1 + 255 - 0) + 0;
		sf::Color fillColor(rR, rG, rB);
		sf::Color outlineColor(con.oR, con.oG, con.oB);
		int thickness = con.outlineThickness;
		int diffVer = 1 + con.vMax - con.vMin;
		int randVertices = std::rand() % diffVer + con.vMin;
		int lifeSpan = con.smolLifespan;
		
		auto enemy = this->m_entities.AddEntity("enemy");

		//Set up components
		enemy->cTransform = std::make_shared<CTransform>(pos, vel, 0);
		enemy->cShape = std::make_shared<CShape>(radius, randVertices, fillColor, outlineColor, thickness);
		enemy->cCollision = std::make_shared<CCollision>(colRadius);
		enemy->cScore = std::make_shared<CScore>(100);
		enemy->cLifespan = std::make_shared<CLifespan>(lifeSpan);

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

void Game::sVertices(const std::shared_ptr<Entity>& entity)
{
	auto& eT = entity->cTransform;
	int sides = entity->cShape->circle.getPointCount();
	int speed = eT->velocity.x;
	float angle = eT->angle;
	float baseAngle = 360.0f / sides;
	Vec2 ePos = eT->pos;
	const float PI = 3.14159265359;
	float radius = entity->cShape->circle.getRadius() / 2;
	sf::Color fillColor = entity->cShape->circle.getFillColor();
	sf::Color outlineColor = entity->cShape->circle.getOutlineColor();
	float thickness = entity->cShape->circle.getOutlineThickness();

	for (int i = 1; i <= sides; i++) {
		auto ver = this->m_entities.AddEntity("vertex");

		//calculate angle
		float vAngle = baseAngle*i + angle > 360 ? (baseAngle*i + angle) - 360 : (baseAngle*i + angle);
		float rad = vAngle * (PI / 180);
		float x = cos(rad);
		float y = sin(rad);
		Vec2 vel(speed * x, speed * y);

		//set up components
		ver->cTransform = std::make_shared<CTransform>(eT->pos, vel, 0);
		ver->cLifespan = std::make_shared<CLifespan>(50);
		ver->cShape = std::make_shared<CShape>(radius, sides, fillColor, outlineColor, thickness);

		ver->cShape->circle.setPosition(ePos.x, ePos.y);
	}
}
