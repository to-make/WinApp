#pragma once

#include <windows.h>
#include <math.h>
#include <vector>
#include <string>

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

class Object
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

	virtual bool IsCollide(Object);
	virtual bool IsCollide(Object*);

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
protected:
	int _hp;
	int _damage;
public:
	int GetDamage() { return _damage; }
	int GetHp() { return _hp; }
	void SetHp(int hp) { _hp = hp; }

	virtual void MoveOneFrame() = 0;
};

class Player : public MobObj
{
	int _cooltime; //게임 관련 변수들
public:
	Player(int, int, POINT);
	void MoveOneFrame()  override;
};

class Enemy1 : public MobObj
{
	void MoveOneFrame()  override;
};

class BarrierObj : public Object
{
	int _damage;
public:
	void MovePos(POINT);
	bool IsCollide(Object) override;
	bool IsCollide(Object*) override;
};

class Shop
{
public:
	int _maxcnt, _cnt;
	Shop* _parent;
	char _name[MAXLENGTH]; //not THCAR?
	void(*_func)(Shop*, int); //upgrade or inform

	Shop(int, void(*)(Shop*, int));
	Shop(int, void(*)(Shop*, int), Shop*);
	void Update() { _func(this, 0); }
	void Upgrade() { _func(this, 1); }
	bool IsVisiable() {return _parent->_cnt!=0;}
};
/*
==list==
addshot
shotdamage
reload
barrier
gravity wave
charge defense
enemy move down
*/

void Init();
bool MoveFrame();
void SetShop(); // shop reset(only once work)
void UpdateShop();
void AddShot(Shop*, int);
void AddBarrier(Shop*, int);
