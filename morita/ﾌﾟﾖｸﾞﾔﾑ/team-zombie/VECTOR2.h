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

	//代入演算子
	VECTOR2& operator=(const VECTOR2& vec);

	//比較演算子	真偽を問うときはbool
	bool operator==(const VECTOR2& vec) const;
	bool operator!=(const VECTOR2& vec) const;
	bool operator<=(const VECTOR2& vec) const;
	bool operator<(const VECTOR2& vec) const;
	bool operator>(const VECTOR2 & vec) const;

	//添え字演算子

	//単項演算子
	VECTOR2& operator += (const VECTOR2& vec);
	VECTOR2& operator -= (const VECTOR2& vec);
	VECTOR2& operator *= (float k);
	VECTOR2& operator /= (float k);
	VECTOR2 operator +() const;
	VECTOR2 operator -() const;
	float Magnitude()const;
	void Normalize();
};

//ベクトルの演算
//VECTOR2 + VECTOR2				↓大体2個まで　１個の場合もある
//返り値の型 operator+演算子(引数１,引数2....);
//VECTOR2 + VECTOR2
VECTOR2 operator+(const VECTOR2& u, const VECTOR2& v);

//VECTOR2 - VECTOR2
VECTOR2 operator-(const VECTOR2& u, const VECTOR2& v);
//intとVECTOR2の計算
//int * VECTOR2	
VECTOR2 operator*(float k, const VECTOR2& v);

VECTOR2 operator+(float k, const VECTOR2& v);

VECTOR2 operator-(float k, const VECTOR2& v);


//Vector3D * int
VECTOR2 operator*(const VECTOR2& v, float k);//←逆を用意してたほうが便利
										   //Vector3D / int
VECTOR2 operator/(const VECTOR2& v, float k);//VECTOR2で割ることはできないから1個だけ

VECTOR2 operator%(const VECTOR2& v, float k);

VECTOR2 operator+(const VECTOR2& v, float k);

VECTOR2 operator-(const VECTOR2& v, float k);
