#pragma once
class Vec2
{
	double x;
	double y;
public:
	Vec2(double x = 0, double y = 0);

	bool operator == (const Vec2& v) const;
	bool operator != (const Vec2& v) const;
	Vec2 operator + (const Vec2& v) const;
	Vec2 operator - (const Vec2& v) const;
	Vec2 operator * (const Vec2& v) const;
	Vec2 operator / (const Vec2& v) const;

	void Normalize(const Vec2& v);
	double Length(const Vec2& v) const;
};

