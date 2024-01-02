#pragma once

#include <windows.h>
#include <math.h>
#include <vector>

#define UP 0
#define DOWN 1
#define RIGHT 2
#define LEFT 3

#define R 50
#define SPEED 10

#define MAXLENGTH 80
/*
#define PI 3.1415926
#define RADIAN(X) (X) * PI / 180
*/

class Object //¸ðµç ¹°Ã¼´Â ¿øÀ¸·Î ÇÔ 
{
protected:
	POINT _pos;
	int _r;
	double _angle;
	double _speed;

public:
	void MoveAngleToDir();

	void Rotate(double);
	void SetAngle(double);

	POINT GetPos() { return _pos; }
	void SetPos(POINT pos) { _pos = pos; }
	int GetR() { return _r; }

	bool IsCollide(Object);
	bool IsCollide(Object*);

	void Draw(HDC hdc, RECT rtWindow);
};

class BulletObj : public Object
{
	int _damage;
public:
	BulletObj(int, int, double, POINT);

	int GetDamage() { return _damage; }

	void MoveOneFrame();
};

class MobObj : public Object
{
	int _hp;
	int _damage;
public:
	MobObj(int, int, POINT);

	int GetDamage() { return _damage; }
	int GetHp() { return _hp; }
	void SetHp(int hp) { _hp = hp; }

	void MoveOneFrame();
};

class BarrierObj : public Object
{
	int _damage;
public:
	void MovePos(POINT);
};

class Shop
{
	int _maxcnt, _cnt;
	char _text[MAXLENGTH]; //not THCAR?
	void(*)(*Shop, int) _func; //upgrade or inform
public:
	Shop(int, void(*)(*Shop, int));
	void Upgrade() {_func(this, 1);}
}
/*
==list==
addshot
shotdamage
reload
barrier
gravity wave
*/

void Init();
bool MoveFrame();
void reset(); // shop reset(only once work)
