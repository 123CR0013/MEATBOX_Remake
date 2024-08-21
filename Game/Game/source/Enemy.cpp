#include "Enemy.h"

Enemy::Enemy(ModeBase* mode) : GameObject(mode)
{
	_objectType = TYPE::ENEMY;
}

Enemy::~Enemy()
{

}
