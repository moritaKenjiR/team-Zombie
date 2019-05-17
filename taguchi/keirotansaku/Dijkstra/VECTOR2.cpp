#include "VECTOR2.h"
#include "Dxlib.h"
#include <cmath>
VECTOR2::VECTOR2()
{
	x = 0;
	y = 0;
}

VECTOR2::VECTOR2(int x, int y)
{
	this->x = x;
	this->y = y;
}

VECTOR2::~VECTOR2()
{
}

//‘ã“ü‰‰ŽZŽq
VECTOR2 & VECTOR2::operator=(const VECTOR2 & vec)	//&‚ÍŽQÆ“n‚µ
{
	this->x = vec.x;
	this->y = vec.y;
	return *this;		//ƒ|ƒCƒ“ƒ^‚Ì’†g‚ðŒ©‚½‚¢‚Æ‚«‚Í@*
}


//”äŠr‰‰ŽZŽq
bool VECTOR2::operator==(const VECTOR2 & vec) const
{
	return (this->x == vec.x) && (this->y == vec.y);
}

bool VECTOR2::operator!=(const VECTOR2 & vec) const
{
	return  !((this->x == vec.x) && (this->y == vec.y));
}

bool VECTOR2::operator<=(const VECTOR2 & vec) const
{
	return ((this->x <= vec.x) && (this->y <= vec.y));
}

bool VECTOR2::operator<(const VECTOR2 & vec) const
{
	return ((this->x < vec.x) && (this->y < vec.y));
}

//’P€‰‰ŽZŽq
VECTOR2 & VECTOR2::operator+=(const VECTOR2 & vec)
{
	this->x += vec.x;
	this->y += vec.y;
	return *this;
}

VECTOR2 & VECTOR2::operator-=(const VECTOR2 & vec)
{
	this->x -= vec.x;
	this->y -= vec.y;
	return *this;
}

VECTOR2 & VECTOR2::operator*=(float k)
{
	this->x *= k;
	this->y *= k;
	return *this;
}

VECTOR2 & VECTOR2::operator/=(float k)
{
	this->x /= k;
	this->y /= k;
	return *this;
}

VECTOR2 VECTOR2::operator+() const
{
	return *this;
}

VECTOR2 VECTOR2::operator-() const
{
	return VECTOR2(-this->x, -this->y);
}

float VECTOR2::Magnitude() const
{
	return std::sqrt(x * x + y * y);
}

void VECTOR2::Normalize()
{
	float mag = Magnitude();
	fx = x;
	fy = y;
	if (mag < (1e-6)) return;
	fx /= mag;
	fy /= mag;
}

float VECTOR2::Clamp(const float & i, float MIN = 0.0f, float MAX = 1.0f)
{
	return max(min(i,MAX),MIN);
}

VECTOR2 VECTOR2::Clamp(const VECTOR2 &i, float MIN = 0.0f, float MAX = 1.0f)
{
	return VECTOR2(Clamp(i.x,MIN,MAX),Clamp(i.y,MIN,MAX));
}

VECTOR2 VECTOR2::Clamp(const VECTOR2 &i, VECTOR2 MIN = VECTOR2(0.0f,0.0f), VECTOR2 MAX = VECTOR2(1.0f, 1.0f))
{
	return VECTOR2(Clamp(i.x,MIN.x,MAX.x), Clamp(i.y, MIN.y, MAX.y));
}

//ÍÞ¸ÄÙ‚Ì‰‰ŽZ
//VECTOR2 + VECTOR2
VECTOR2 operator+(const VECTOR2 & u, const VECTOR2 & v)
{
	VECTOR2 vec;
	vec.x = u.x + v.x;
	vec.y = u.y + v.y;
	return vec;

	//ã‹L‚Æ“¯‚¶
	//return VECTOR2(u.x + v.x, u.y + v.y);

}

//VECTOR2 - VECTOR2
VECTOR2 operator-(const VECTOR2 & u, const VECTOR2 & v)
{
	VECTOR2 vec;
	vec.x = u.x - v.x;
	vec.y = u.y - v.y;
	return vec;
}

//int * VECTOR2
VECTOR2 operator*(int k, const VECTOR2 & v)
{
	VECTOR2 vec;
	vec.x = k * v.x;
	vec.y = k * v.y;
	return vec;
}

VECTOR2 operator+(int k, const VECTOR2 & v)
{
	VECTOR2 vec;
	vec.x = k + v.x;
	vec.y = k + v.y;
	return vec;
}

VECTOR2 operator-(int k, const VECTOR2 & v)
{
	VECTOR2 vec;
	vec.x = k - v.x;
	vec.y = k - v.y;
	return vec;
}

//Vector3D * int
VECTOR2 operator*(const VECTOR2 & v, int k)
{
	VECTOR2 vec;
	vec.x = v.x * k;
	vec.y = v.y * k;
	return vec;
}

//Vector3D / int
VECTOR2 operator/(const VECTOR2 & v, float k)
{
	VECTOR2 vec;
	vec.x = v.x / k;
	vec.y = v.y / k;
	return vec;
}

VECTOR2 operator+(const VECTOR2 & v, int k)
{
	VECTOR2 vec;
	vec.x = v.x + k;
	vec.y = v.y + k;
	return vec;
}

VECTOR2 operator-(const VECTOR2 & v, int k)
{
	VECTOR2 vec;
	vec.x = v.x - k;
	vec.y = v.y - k;
	return vec;
}
