#pragma once

class VECTOR2
{
public:
	VECTOR2();
	VECTOR2(float x, float y);
	~VECTOR2();
	float x;
	float y;
	float fx;
	float fy;

	//‘ã“ü‰‰Zq
	VECTOR2& operator=(const VECTOR2& vec);

	//”äŠr‰‰Zq	^‹U‚ğ–â‚¤‚Æ‚«‚Íbool
	bool operator==(const VECTOR2& vec) const;
	bool operator!=(const VECTOR2& vec) const;
	bool operator<=(const VECTOR2& vec) const;
	bool operator<(const VECTOR2& vec) const;
	bool operator>(const VECTOR2 & vec) const;

	//“Y‚¦š‰‰Zq

	//’P€‰‰Zq
	VECTOR2& operator += (const VECTOR2& vec);
	VECTOR2& operator -= (const VECTOR2& vec);
	VECTOR2& operator *= (float k);
	VECTOR2& operator /= (float k);
	VECTOR2 operator +() const;
	VECTOR2 operator -() const;
	float Magnitude()const;
	void Normalize();
};

//ƒxƒNƒgƒ‹‚Ì‰‰Z
//VECTOR2 + VECTOR2				«‘å‘Ì2ŒÂ‚Ü‚Å@‚PŒÂ‚Ìê‡‚à‚ ‚é
//•Ô‚è’l‚ÌŒ^ operator+‰‰Zq(ˆø”‚P,ˆø”2....);
//VECTOR2 + VECTOR2
VECTOR2 operator+(const VECTOR2& u, const VECTOR2& v);

//VECTOR2 - VECTOR2
VECTOR2 operator-(const VECTOR2& u, const VECTOR2& v);
//int‚ÆVECTOR2‚ÌŒvZ
//int * VECTOR2	
VECTOR2 operator*(float k, const VECTOR2& v);

VECTOR2 operator+(float k, const VECTOR2& v);

VECTOR2 operator-(float k, const VECTOR2& v);


//Vector3D * int
VECTOR2 operator*(const VECTOR2& v, float k);//©‹t‚ğ—pˆÓ‚µ‚Ä‚½‚Ù‚¤‚ª•Ö—˜
										   //Vector3D / int
VECTOR2 operator/(const VECTOR2& v, float k);//VECTOR2‚ÅŠ„‚é‚±‚Æ‚Í‚Å‚«‚È‚¢‚©‚ç1ŒÂ‚¾‚¯

VECTOR2 operator%(const VECTOR2& v, float k);

VECTOR2 operator+(const VECTOR2& v, float k);

VECTOR2 operator-(const VECTOR2& v, float k);

bool Range(float&, float, float);