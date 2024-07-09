#pragma once
class Vec2
{
public:
	double x;
	double y;

	Vec2(double x = 0, double y = 0);

	bool operator == (const Vec2& v) const;
	bool operator != (const Vec2& v) const;
	Vec2 operator + (const Vec2& v) const;
	Vec2 operator - (const Vec2& v) const;
	Vec2 operator * (const Vec2& v) const;
	Vec2 operator / (const Vec2& v) const;

	void Normalize();
	double Length() const;
};

