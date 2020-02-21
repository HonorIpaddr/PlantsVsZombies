/**
 *Copyright (c) 2020 LZ.All Right Reserved
 *Author : LZ
 *Date: 2020.2.11
 *Email: 2117610943@qq.com
 */

#include "Bullet.h"

#include "Zombies/Zombies.h"
#include "Scenes/GameScene/GSData.h"

Bullet::Bullet() :
	_node(nullptr)
,   _attack(0)
,   _isUsed(false)
,   _bulletType(BulletType::None)
,   _position(Vec2::ZERO)
,   _global(Global::getInstance())
{
}

Bullet::~Bullet()
{
}

void Bullet::bulletDeleteUpdate(list<Bullet*>::iterator& bullet)
{
	if (!(*bullet)->getBulletVisible())
	{
		(*bullet)->releaseBullet();
		delete *bullet;
		*bullet = nullptr;
		BulletGroup.erase(bullet++);
	}
	else
	{
		++bullet;
	}
}

SkeletonAnimation* Bullet::getBullet() const
{
	return _bulletAnimation;
}

void Bullet::setBulletPosition(const Vec2& position)
{
	_position = position;
}

void Bullet::setBulletName(const string& name)
{
	_bulletName = name;
}

void Bullet::setBulletIsUsed(const bool isUsed)
{
	_isUsed = isUsed;
}

void Bullet::setBulletAttack(const float attack)
{
	_attack = attack;
}

void Bullet::setBulletOpacity(GLubyte opacity)
{
	_bulletAnimation->setOpacity(opacity);
	_bulletAnimation->getChildByName("shadow")->setOpacity(opacity);
}

void Bullet::setBulletOpacity()
{
	_bulletAnimation->setOpacity(0);
	_bulletAnimation->getChildByName("shadow")->setOpacity(0);
}

bool Bullet::getBulletIsUsed() const
{
	return _isUsed;
}

int Bullet::getBulletAttack() const
{
	return _attack;
}

void Bullet::bulletAttackHurtZombies(Zombies* zombie)
{
	if (zombie->getZombieCurrentBodyShieldVolume() < _attack) /* �����ǰ���廤��Ѫ��С�ڱ�ը�˺� */
	{
		if (zombie->getZombieCurrentBodyShieldVolume() + zombie->getZombieCurrentHeadShieldVolume() < _attack) /* �����ǰ���廤�ܼ�ͷ������Ѫ��С�ڱ�ը�˺� */
		{
			if (zombie->getZombieCurrentBodyShieldVolume() + zombie->getZombieCurrentHeadShieldVolume() + zombie->getZombieCurrentBloodVolume() <= _attack) /* �����ʬ����Ѫ��С�ڱ�ը�˺�����ʬ������ */
			{
				zombie->setZombieCurrentBloodVolume(0);
				zombie->setZombieCurrentHeadShieldVolume(0);
				zombie->setZombieCurrentBodyShieldVolume(0);
			}
			else
			{
				/* ���㽩ʬ����Ѫ�� */
				zombie->setZombieCurrentBloodVolume(zombie->getZombieCurrentBodyShieldVolume() + zombie->getZombieCurrentHeadShieldVolume() + zombie->getZombieCurrentBloodVolume() - _attack);
				zombie->setZombieCurrentHeadShieldVolume(0);
				zombie->setZombieCurrentBodyShieldVolume(0);
			}
		}
		else
		{
			/* ���㽩ʬ����ʣ��Ѫ�� */
			zombie->setZombieCurrentHeadShieldVolume(zombie->getZombieCurrentBodyShieldVolume() + zombie->getZombieCurrentHeadShieldVolume() - _attack);
			zombie->setZombieCurrentBodyShieldVolume(0);
		}
	}
	else
	{
		zombie->setZombieCurrentBodyShieldVolume(zombie->getZombieCurrentBodyShieldVolume() - _attack);
	}
}

void Bullet::bulletExplodeHurtZombies(Zombies* zombie)
{
	if (zombie->getZombieCurrentBodyShieldVolume() > 0)
	{
		zombie->setZombieCurrentBodyShieldVolume(zombie->getZombieCurrentBodyShieldVolume() - _attack);
	}
	else if (zombie->getZombieCurrentHeadShieldVolume() > 0)
	{
		zombie->setZombieCurrentHeadShieldVolume(zombie->getZombieCurrentHeadShieldVolume() - _attack);
	}
	else
	{
		zombie->setZombieCurrentBloodVolume(zombie->getZombieCurrentBloodVolume() - _attack);
	}
}

string& Bullet::getBulletName()
{
	return _bulletName;
}

float Bullet::getZOrder(const int& positionY) const
{
	const int pos[5] = { 675,537,399,261,123 };
	for (int i = 0; i < 5; i++)
	{
		if (pos[i] == positionY)
		{
			return i * 20 + 12;
		}
	}
	return 0;
}

void Bullet::releaseBullet() const
{
	_bulletAnimation->removeFromParent();
}

Vec2 Bullet::getBulletPosition() const
{
	return _bulletAnimation->getPosition();
}

float Bullet::getBulletPositionX() const
{
	return _bulletAnimation->getPositionX();
}

float Bullet::getBulletPositionY() const
{
	return _bulletAnimation->getPositionY();
}

BulletType Bullet::getBulletType() const
{
	return _bulletType;
}

bool Bullet::getBulletVisible() const
{
	return _bulletAnimation->isVisible();
}