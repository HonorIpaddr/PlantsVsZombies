/**
 *Copyright (c) 2020 LZ.All Right Reserved
 *Author : LZ
 *Date: 2020.1.26
 *Email: 2117610943@qq.com
 */

#include "ConeDoorZombies.h"

ConeDoorZombies::ConeDoorZombies(Node* node)
{
	_node = node;
	_attackHeadSoundEffectType = 0;
	_attackBodySoundEffectType = 1;
	_animationId = 1;
	_bloodVolume = 200;
	_headShieldVolume = 600;
	_bodyShieldVolume = 1200;
	_currentBloodVolume = 200;
	_currentHeadShieldVolume = 600;
	_currentBodyShieldVolume = 1200;
	_speed = 30;
	_currentSpeed = 30;
	_isHaveShield = true;
	_zombiesType = ZombiesType::ConeDoorZombies;
}

ConeDoorZombies* ConeDoorZombies::create(Node* node)
{
	ConeDoorZombies* coneDoorZombies = new (std::nothrow) ConeDoorZombies(node);
	if (coneDoorZombies && coneDoorZombies->init())
	{
		coneDoorZombies->autorelease();
		return coneDoorZombies;
	}
	CC_SAFE_DELETE(coneDoorZombies);
	return nullptr;
}

void ConeDoorZombies::createZombie()
{
	zombieInit();

	setZombieAnimationInformation(rand() % 2 + 2);

	setZombieScale();

	createZombieShadow();

	_zombiesAnimation->setAnimation(0, "Zombies_Door_Walk", true);
	_zombiesAnimation->setAttachment("Zombie_bucket", "Zombie_cone1");
	_zombiesAnimation->setAttachment("Zombie_screendoor", "Zombie_screendoor1");
}

void ConeDoorZombies::createPreviewZombie()
{
	zombieInit();

	setZombieAnimationInformation(rand() % 2);

	setZombieScale();

	createZombieShadow();

	_zombiesAnimation->setAnimation(0, "Zombies_Door_Stand", true);
	_zombiesAnimation->setAttachment("Zombie_bucket", "Zombie_cone1");
	_zombiesAnimation->setAttachment("Zombie_screendoor", "Zombie_screendoor1");
}

void ConeDoorZombies::zombieInjuredEventUpdate()
{
	if (_isHaveShield) /* ����л��� */
	{
		if (_currentBodyShieldVolume <= _bodyShieldVolume * 2.0f / 3) /* ���廤��һ������ */
		{
			setZombieBodyShieldPrimaryInjure("Zombie_screendoor", "Zombie_screendoor2");
		}
		if (_currentBodyShieldVolume <= _bodyShieldVolume * 1.0f / 3) /* ���廤�ܶ������� */
		{
			setZombieBodyShieldSecondaryInjure("Zombie_screendoor", "Zombie_screendoor3");
		}
		if (_currentBodyShieldVolume <= 0)   /* ���廤����ʧ */
		{
			setZombieBodyShieldThirdInjure("Zombie_screendoor", "tt_innerleg_foot3");
		}

		if (_currentHeadShieldVolume <= _headShieldVolume * 2.0f / 3) /* ͷ������һ������ */
		{
			setZombieHeadShieldPrimaryInjure("Zombie_bucket", "Zombie_cone2");
		}
		if (_currentHeadShieldVolume <= _headShieldVolume * 1.0f / 3) /* ͷ�����ܶ������� */
		{
			setZombieHeadShieldSecondaryInjure("Zombie_bucket", "Zombie_cone3");
		}
		if (_currentHeadShieldVolume <= 0)   /* ͷ��������ʧ */
		{
			setZombieHeadShieldThirdInjure("Zombie_bucket", "tt_innerleg_foot3");
		}
	}
	if (_currentBloodVolume <= _bloodVolume / 2.0f)
	{
		setZombiePrimaryInjure();
	}
	if (_currentBloodVolume <= 0)
	{
		setZombieSecondaryInjure();
	}
}

void ConeDoorZombies::setZombieBodyShieldThirdInjure(const string& oldName, const string& newName)
{
	if (_animationId == 3) /* ���廤�������˺� */
	{
		_zombiesAnimation->setAttachment("Zombie_screendoor", "tt_innerleg_foot3");
		_attackBodySoundEffectType = 0;
		_animationId = 4;

		_zombiesAnimation->addAnimation(0, "Zombies_Walk", true);
	}
}

void ConeDoorZombies::setZombieHeadShieldThirdInjure(const string& oldName, const string& newName)
{
	if (_animationId == 6) /* ͷ�����������˺� */
	{
		_zombiesAnimation->setAttachment("Zombie_bucket", "tt_innerleg_foot3");
		_animationId = 7;
		_isHaveShield = false;

		/* ��ʬ��ͷ������ */
		zombieLoseShieldAnimation("Zombie_cone");
	}
}
