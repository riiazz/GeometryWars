#include "Vec2.h"
#include <math.h>

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

void Vec2::Normalize()
{
	int l = Length();
	this->x = this->x / l;
	this->y = this->y / l;
}

double Vec2::Length() const
{
	return sqrtf((this->x* this->x) + (this->y* this->y));
}
