#include "Game.h"

std::vector <BulletObj* > bullets;
std::vector<MobObj*> enemys;
MobObj* player;
BarrierObj* barrier;
std::vector<Shop*> shop;
RECT rtMapSize;

void Object::MoveAngleToDir()
{
	_pos.x += (double)cos(_angle) * _speed;
	_pos.y += (double)sin(_angle) * _speed;
}

void Object::Rotate(double angle)
{
	_angle += angle;
}
void Object::SetAngle(double angle)
{
	_angle = angle;
}

bool Object::IsCollide(Object t)
{
	POINT tpos = t.GetPos();
	long long int dis = (_pos.x - tpos.x) * (_pos.x - tpos.x) + (_pos.y - tpos.y) * (_pos.y - tpos.y);
		return dis <= (_r + t.GetR());
}
bool Object::IsCollide(Object* t)
{
	POINT tpos = t->GetPos();
	long long int dis = (_pos.x - tpos.x) * (_pos.x - tpos.x) + (_pos.y - tpos.y) * (_pos.y - tpos.y);
		return dis <= (_r + t->GetR());
}

void Object::Draw(HDC hdc)
{
	Ellipse(hdc, (_pos.x - R) - rtMapSize.left, (_pos.y - R) - rtMapSize.top, (_pos.x + R) - rtMapSize.left, (_pos.y + R) - rtMapSize.top);
}

BulletObj::BulletObj(int damage, int speed, double angle, POINT pos)
{
	_damage = damage;
	_speed = speed;
	_angle = angle;
	_pos = pos;
}
void BulletObj::MoveOneFrame()
{
	MoveAngleToDir();
}


Player::Player(int hp, int damage, POINT pos)
{
	_hp = hp;
	_damage = damage;
	_pos = pos;
}

void Player::MoveOneFrame()
{
	POINT ptPlayer = player->GetPos();

	//Player Move
	if (GetAsyncKeyState(VK_LEFT) & 0x8000) {
		ptPlayer.x -= SPEED;
	}
	if (GetAsyncKeyState(VK_RIGHT) & 0x8000) {
		ptPlayer.x += SPEED;
	}
	if (GetAsyncKeyState(VK_UP) & 0x8000) {
		ptPlayer.y -= SPEED;
	}
	if (GetAsyncKeyState(VK_DOWN) & 0x8000) {
		ptPlayer.y += SPEED;
	}

	//wall collsion check
	int MLeft = -1, MRight = -1, MTop = -1, MBottom = -1; //TODO:변수명

	if (ptPlayer.x - R < rtMapSize.left) {
		MLeft = +rtMapSize.left - (ptPlayer.x - R);
		ptPlayer.x += MLeft - 1;
	}
	if (ptPlayer.x + R > rtMapSize.right) {
		MRight = (ptPlayer.x + R) - rtMapSize.right;
		ptPlayer.x -= MRight - 1;
	}
	if (ptPlayer.y - R < rtMapSize.top) {
		MTop = rtMapSize.top - (ptPlayer.y - R);
		ptPlayer.y += MTop - 1;
	}
	if (ptPlayer.y + R > rtMapSize.bottom) {
		MBottom = (ptPlayer.y + R) - rtMapSize.bottom;
		ptPlayer.y -= MBottom - 1;
	}

	rtMapSize.left -= MLeft;
	rtMapSize.top -= MTop;
	rtMapSize.right += MRight;
	rtMapSize.bottom += MBottom;

	//Shot bullet
	if (GetAsyncKeyState(VK_LBUTTON) & 0x8000) {
		POINT ptMouse, ptPlayer = player->GetPos();
		GetCursorPos(&ptMouse);
		bullets.push_back(new BulletObj(1, 5, atan2((ptMouse.y - ptPlayer.y), (ptMouse.x - ptPlayer.x)), ptPlayer));
	}


	player->SetPos(ptPlayer);
}

void Enemy1::MoveOneFrame()
{

}

void BarrierObj::MovePos(POINT pos)
{
	_pos = pos;
}

bool BarrierObj::IsCollide(Object t)
{
	POINT tpos = t.GetPos(), vertex;
	int x = 0, y = 0,check;
	if(_pos.x - _r/2 > tpos.x)x = 1;
	else if(_pos.x + _r/2 < tpos.x)x = 3;
	else x = 2;
	if(_pos.y - _r/2 > tpos.y)y = 1;
	else if(_pos.y + _r/2 < tpos.y)y = 3;
	else y = 2;
	check = (y<<2) | x;
	switch(check)
	{
		case 6:
			return _pos.y - _r/2 <= (t.GetR());
		case 9:
			return _pos.x - _r/2 <= (t.GetR());
		case 10:
			return true;
		case 12:
			return _pos.x + _r/2 <= (t.GetR());
		case 14:
			return _pos.y + _r/2 <= (t.GetR());
		case 5:
			vertex = {_pos.x - _r/2, _pos.y - _r/2};
			break;
		case 7:
			vertex = {_pos.x + _r/2, _pos.y - _r/2};
			break;
		case 13:
			vertex = {_pos.x - _r/2, _pos.y + _r/2};
			break;
		case 15:
			vertex = {_pos.x - _r/2, _pos.y + _r/2};
			break;
	}
	//TODO:vertex가 초기화 안된경우 처리
	long long int dis = (vertex.x - tpos.x) * (vertex.x - tpos.x) + (vertex.y - tpos.y) * (vertex.y - tpos.y);
	return dis <= (t.GetR());
}

bool BarrierObj::IsCollide(Object* t)
{
	POINT tpos = t->GetPos(), vertex;
	int x = 0, y = 0,check;
	if(_pos.x - _r/2 > tpos.x)x = 1;
	else if(_pos.x + _r/2 < tpos.x)x = 3;
	else x = 2;
	if(_pos.y - _r/2 > tpos.y)y = 1;
	else if(_pos.y + _r/2 < tpos.y)y = 3;
	else y = 2;
	check = (y<<2) | x;
	switch(check)
	{
		case 6:
			return _pos.y - _r/2 <= (t->GetR());
		case 9:
			return _pos.x - _r/2 <= (t->GetR());
		case 10:
			return true;
		case 12:
			return _pos.x + _r/2 <= (t->GetR());
		case 14:
			return _pos.y + _r/2 <= (t->GetR());
		case 5:
			vertex = {_pos.x - _r/2, _pos.y - _r/2};
			break;
		case 7:
			vertex = {_pos.x + _r/2, _pos.y - _r/2};
			break;
		case 13:
			vertex = {_pos.x - _r/2, _pos.y + _r/2};
			break;
		case 15:
			vertex = {_pos.x - _r/2, _pos.y + _r/2};
			break;
	}
	long long int dis = (vertex.x - tpos.x) * (vertex.x - tpos.x) + (vertex.y - tpos.y) * (vertex.y - tpos.y);
	return dis <= (t->GetR());
}

Shop::Shop(int maxcnt,void(*func)(Shop*, int))//TODO:_name 초기화
{
	_maxcnt = maxcnt;
	_cnt = 0;
	_parent = NULL;
	_func = func;
}

Shop::Shop(int maxcnt,void(*func)(Shop*, int), Shop* parent)//TODO:_name 초기화
{
	_maxcnt = maxcnt;
	_cnt = 0;
	_parent = parent;
	_func = func;
}

void Init()
{
	Shop* parent;
	player = new Player(10, 10, {100,100});
	shop.push_back(new Shop(5, AddShot));
	shop.push_back(new Shop(1, AddBarrier));
	parent = shop.back();
}

bool MoveFrame()
{
	for (auto i : bullets) {
		i->MoveOneFrame();
	}
	for (auto i : enemys) {
		i->MoveOneFrame();
	}
	player->MoveOneFrame();

	/*
	for (auto i : bullets) {
		for (auto j : enemys) {
			if (i->IsCollide(j)) {
				int hp = j->GetHp();
				j->SetHp(hp - (i->GetDamage()));
			}
		} 
	}
	for (auto i : enemys) {
		if (i->IsCollide(player)) {
			int hp = player->GetHp();
			player->SetHp(hp - (i->GetDamage()));
			if (player->GetHp() <= 0)return false;
		}
	}*/

	return true;
}

void UpdateShop()
{
	for(Shop* i : shop)i->Update();
}

void AddShot(Shop* subject,int mode)
{
	if(mode == 0){ //message
		char text[MAXLENGTH] = "Fire ";
		strcat(text, INTTOCHAR(subject->_cnt));
		if(subject->_cnt != subject->_maxcnt){
			strcat(text, "(");
			strcat(text, INTTOCHAR(subject->_cnt+1));
			strcat(text, ")");
		}
		strcat(text, ((subject->_cnt)==1?" bullet":" bullets"));
		strcpy(subject->_name, text);
	}
	else if(mode == 1){ //upgrage
	}
}

void AddBarrier(Shop* subject,int mode)
{
	if(mode == 0){ //message
		strcpy(subject->_name, ((subject->_cnt) == 1 ? "(Add Barrier)" : "Add Barrier"));
	}
	else if(mode == 1){ //upgrage
	}
}
