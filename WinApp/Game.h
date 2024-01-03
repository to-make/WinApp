#pragma once

#include <windows.h>
#include <tchar.h>
#include <math.h>
#include <vector>
#include <string>
#include <time.h>

#define UP 0
#define DOWN 1
#define RIGHT 2
#define LEFT 3

#define R 50
#define SPEED 10
#define MAXLENGTH 80

#define INTTOCHAR(N) [](int n) -> TCHAR* {TCHAR tchar[MAXLENGTH];  wsprintf(tchar, _T("%d)"), n); return tchar;}(N)
#define FLOATTOCHAR(N) [](float f) -> TCHAR* {TCHAR tchar[MAXLENGTH];  wsprintf(tchar, _T("%f"), f); return tchar;}(N)

/*
#define PI 3.1415926
#define RADIAN(X) (X) * PI / 180
*/

class Object
{
protected:
	POINT _pos;
	int _r;
	int _size;
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
	virtual void MoveOneFrame(int);

	void Draw(HDC hdc);
};

class BulletObj : public Object
{
	int _damage;
public:
	BulletObj(int, int, double, POINT);

	int GetDamage() { return _damage; }

	virtual void MoveOneFrame(int) override;
};

class MobObj : public Object
{
protected:
	int _hp;
	int _damage;
public:
	int GetDamage() { return _damage; }
	void SetDamage(int damage) { _damage = damage; }
	int GetHp() { return _hp; }
	void SetHp(int hp) { _hp = hp; }
};

class Player : public MobObj
{
	int _bulletcooltime, _bulletlasttime;//총알 발사 쿨
	//TODO:게임 관련 변수들 추가
public:
	//int hp, int damage, int speed, int cooltime, POINT pos
	Player(int, int, int, int, POINT);

	int GetCooltime() { return _bulletcooltime; }
	int GetLasttime() { return _bulletlasttime; }
	void SetCooltime(double cooltime) { _bulletcooltime = cooltime; }
	void SetLasttime(int lasttime) { _bulletlasttime = lasttime; }
	
	virtual void MoveOneFrame(int) override;
};

class Enemy1 : public MobObj
{
public:
	//int hp, int damage, int speed
	Enemy1(int, int, int, POINT);
	virtual void MoveOneFrame(int)  override;
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
	int _maxcnt, _cnt;
	Shop* _parent;
	TCHAR _name[MAXLENGTH]; //not THCAR?
	void(*_message)(Shop*, int);
	void(*_upgrade)(Shop*, int);
public:

	Shop(int, void(*)(Shop*, int));
	Shop(int, void(*)(Shop*, int), Shop*);
	int GetMaxcnt() { return _maxcnt; }
	int GetCnt() { return _cnt; }
	TCHAR* GetName() { return _name; }
	void SetCnt(int cnt) { _cnt = cnt; }
	void UpdateMessage() { _message(this); }
	void Upgrade() { _upgrade(this); _message(this); }
	bool IsVisiable() {return _parent->_cnt!=0;}
};
/*
==list==
addshot
shotdamage
reload
reduceplayersize
barrier
-damage
-size
gravity wave
charge defense
enemy move down
*/

void Init();
bool MoveFrame();
void UpdateShop();
void AddShotMessage(Shop*);
void AddShotUpgrade(Shop*);
void ShotDamageMessage(Shop*);
void ShotDamageUpgrade(Shop*);
void ShotReloadMessage(Shop*);
void ShotReloadUpgrade(Shop*);
void AddBarrierMessage(Shop*);
void AddBarrierUpgrade(Shop*);
