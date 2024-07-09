#include "Vec2.h"

Vec2::Vec2(double x, double y) 
	: x(x), y(y)
{}

bool Vec2::operator==(const Vec2 & v) const
{
	return (this->x == v.x && this->y == v.y);
}

bool Vec2::operator!=(const Vec2& v) const
{
	return (this->x != v.x || this->y != v.y);;
}

Vec2 Vec2::operator+(const Vec2& v) const
{
	return Vec2(this->x + v.x, this->y + v.y);
}

Vec2 Vec2::operator-(const Vec2& v) const
{
	return Vec2(this->x - v.x, this->y - v.y);
}

Vec2 Vec2::operator*(const Vec2& v) const
{
	return Vec2(this->x * v.x, this->y * v.y);
}

Vec2 Vec2::operator/(const Vec2& v) const
{
	return Vec2(this->x / v.x, this->y / v.y);
}
