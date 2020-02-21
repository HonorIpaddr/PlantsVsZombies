/**
 *Copyright (c) 2020 LZ.All Right Reserved
 *Author : LZ
 *Date: 2020.1.26
 *Email: 2117610943@qq.com
 */

#include "ConeZombies.h"

ConeZombies::ConeZombies(Node* node)
{
	_node = node;
	_attackHeadSoundEffectType = 0;
	_attackBodySoundEffectType = 0;
	_animationId = 4;
	_bloodVolume = 200;
	_headShieldVolume = 600;
	_bodyShieldVolume = 0;
	_currentBloodVolume = 200;
	_currentHeadShieldVolume = 600;
	_currentBodyShieldVolume = 0;
	_speed = 30;
	_currentSpeed = 30;
	_isHaveShield = true;
	_zombiesType = ZombiesType::ConeZombies;
}

ConeZombies* ConeZombies::create(Node* node)
{
	ConeZombies* coneZombies = new (std::nothrow) ConeZombies(node);
	if (coneZombies && coneZombies->init())
	{
		coneZombies->autorelease();
		return coneZombies;
	}
	CC_SAFE_DELETE(coneZombies);
	return nullptr;
}

void ConeZombies::createZombie()
{
	zombieInit();

	setZombieAnimationInformation(rand() % 2 + 2);

	setZombieScale();

	createZombieShadow();

	_zombiesAnimation->setAttachment("Zombie_bucket", "Zombie_cone1");
}

void ConeZombies::createPreviewZombie()
{
	zombieInit();

	setZombieAnimationInformation(rand() % 2);

	setZombieScale();

	createZombieShadow();

	_zombiesAnimation->setAttachment("Zombie_bucket", "Zombie_cone1");
}

void ConeZombies::zombieInjuredEventUpdate()
{
	if (_isHaveShield)/* ���ͷ���л��� */
	{
		if (_currentHeadShieldVolume <= _headShieldVolume * 2.0f / 3.0f) /* ͷ������һ������ */
		{
			setZombieHeadShieldPrimaryInjure("Zombie_bucket", "Zombie_cone2");
		}
		if (_currentHeadShieldVolume <= _headShieldVolume / 3.0f)       /* ͷ�����ܶ������� */
		{
			setZombieHeadShieldSecondaryInjure("Zombie_bucket", "Zombie_cone3");
		}
		if (_currentHeadShieldVolume <= 0)                              /* ͷ��������ʧ */
		{
			setZombieHeadShieldThirdInjure("Zombie_bucket", "tt_innerleg_foot3");
		}
	}
	if (_currentBloodVolume <= _bloodVolume / 2.0f)   /* ��ʬѪ��С��һ��ֵ����ʬ���첲 */
	{
		setZombiePrimaryInjure();
	}
	if (_currentBloodVolume <= 0)                     /* ��ʬѪ��С���㣬��ʬ���� */
	{
		setZombieSecondaryInjure();
	}
}

void ConeZombies::setZombieHeadShieldThirdInjure(const string& oldName, const string& newName)
{
	if (_animationId == 6) /* ���������˺� */
	{
		_zombiesAnimation->setAttachment("Zombie_bucket", "tt_innerleg_foot3");
		_animationId = 7;
		_isHaveShield = false;

		/* ��ʬ������ */
		zombieLoseShieldAnimation("Zombie_cone");
	}
}
