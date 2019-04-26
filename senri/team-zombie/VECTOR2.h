#pragma once

class VECTOR2
{
public:
	VECTOR2();
	VECTOR2(int x, int y);
	~VECTOR2();
	int x;
	int y;
	float fx;
	float fy;

	//������Z�q
	VECTOR2& operator=(const VECTOR2& vec);

	//��r���Z�q	�^�U��₤�Ƃ���bool
	bool operator==(const VECTOR2& vec) const;
	bool operator!=(const VECTOR2& vec) const;
	bool operator<=(const VECTOR2& vec) const;
	bool operator<(const VECTOR2& vec) const;

	//�Y�������Z�q
	int& operator[](int i);

	//�P�����Z�q
	VECTOR2& operator += (const VECTOR2& vec);
	VECTOR2& operator -= (const VECTOR2& vec);
	VECTOR2& operator *= (int k);
	VECTOR2& operator /= (int k);
	VECTOR2& operator /= (float k);
	VECTOR2 operator +() const;
	VECTOR2 operator -() const;
	float Magnitude()const;
	void Normalize();
};

//�x�N�g���̉��Z
//VECTOR2 + VECTOR2				�����2�܂Ł@�P�̏ꍇ������
//�Ԃ�l�̌^ operator+���Z�q(�����P,����2....);
//VECTOR2 + VECTOR2
VECTOR2 operator+(const VECTOR2& u, const VECTOR2& v);

//VECTOR2 - VECTOR2
VECTOR2 operator-(const VECTOR2& u, const VECTOR2& v);
//int��VECTOR2�̌v�Z
//int * VECTOR2	
VECTOR2 operator*(int k, const VECTOR2& v);

VECTOR2 operator+(int k, const VECTOR2& v);

VECTOR2 operator-(int k, const VECTOR2& v);


//Vector3D * int
VECTOR2 operator*(const VECTOR2& v, int k);//���t��p�ӂ��Ă��ق����֗�
										   //Vector3D / int
VECTOR2 operator/(const VECTOR2& v, int k);//VECTOR2�Ŋ��邱�Ƃ͂ł��Ȃ�����1����

VECTOR2 operator%(const VECTOR2& v, int k);

VECTOR2 operator+(const VECTOR2& v, int k);

VECTOR2 operator-(const VECTOR2& v, int k);
