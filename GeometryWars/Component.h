#pragma once
#include "Vec2.h"
#include <SFML/Graphics.hpp>

class CTransform
{
public:
	Vec2 pos = { 0, 0 };
	Vec2 velocity = { 0, 0 };
	Vec2 scale = { 0, 0 };
	float angle = 0;
	CTransform(const Vec2 & p, const Vec2& v, float a)
		: pos(p), velocity(v), angle(a)
	{}
};

class CShape {
public:
	sf::CircleShape circle;
	CShape(float radius, int point, const sf::Color& fill, const sf::Color& outlineColor, float thickness)
		: circle(radius, point)
	{
		circle.setFillColor(fill);
		circle.setOutlineColor(outlineColor);
		circle.setOutlineThickness(thickness);
		circle.setOrigin(radius, radius);
	};
};

class CCollision {
public:
	float radius = 0;
	CCollision(float radius)
		: radius(radius){}
};

class CScore {
public:
	int score = 0;
	CScore(int score)
		: score(score){}
};

class CLifespan {
public:
	int remaining = 0;
	int total = 0;
	CLifespan(int total)
		: total(total), remaining(total){}
};

class CInput {
public:
	bool up = false;
	bool down = false;
	bool left = false;
	bool right = false;
	bool shoot = false;
	CInput(){}
};