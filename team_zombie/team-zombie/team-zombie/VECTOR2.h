#pragma once
class VECTOR2
{
public:
	VECTOR2();
	VECTOR2(int x, int y);
	~VECTOR2();
	int x;
	int y;

	//代入演算子
	VECTOR2& operator=(const VECTOR2& vec);

	//比較演算子	真偽を問うときはbool
	bool operator==(const VECTOR2& vec) const;
	bool operator!=(const VECTOR2& vec) const;
	bool operator<=(const VECTOR2& vec) const;
	bool operator<(const VECTOR2& vec) const;

	//添え字演算子
	int& operator[](int i);

	//単項演算子
	VECTOR2& operator += (const VECTOR2& vec);
	VECTOR2& operator -= (const VECTOR2& vec);
	VECTOR2& operator *= (int k);
	VECTOR2& operator /= (int k);
	VECTOR2 operator +() const;
	VECTOR2 operator -() const;
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
VECTOR2 operator*(int k, const VECTOR2& v);

VECTOR2 operator+(int k, const VECTOR2& v);

VECTOR2 operator-(int k, const VECTOR2& v);


//Vector3D * int
VECTOR2 operator*(const VECTOR2& v, int k);//←逆を用意してたほうが便利
										   //Vector3D / int
VECTOR2 operator/(const VECTOR2& v, int k);//VECTOR2で割ることはできないから1個だけ

VECTOR2 operator%(const VECTOR2& v, int k);

VECTOR2 operator+(const VECTOR2& v, int k);

VECTOR2 operator-(const VECTOR2& v, int k);
