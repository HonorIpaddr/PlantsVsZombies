/**
 *Copyright (c) 2020 LZ.All Right Reserved
 *Author : LZ
 *Date: 2020.1.26
 *Email: 2117610943@qq.com
 */

#include "BucketZombies.h"

BucketZombies::BucketZombies(Node* node)
{
	_node = node;
	_attackHeadSoundEffectType = 1;
	_attackBodySoundEffectType = 0;
	_animationId = 4;
	_bloodVolume = 200;
	_headShieldVolume = 1200;
	_bodyShieldVolume = 0;
	_currentBloodVolume = 200;
	_currentHeadShieldVolume = 1200;
	_currentBodyShieldVolume = 0;
	_speed = 30;
	_currentSpeed = 30;
	_isHaveShield = true;
	_zombiesType = ZombiesType::ConeZombies;
}

BucketZombies* BucketZombies::create(Node* node)
{
	BucketZombies* bucketZombies = new (std::nothrow) BucketZombies(node);
	if (bucketZombies && bucketZombies->init())
	{
		bucketZombies->autorelease();
		return bucketZombies;
	}
	CC_SAFE_DELETE(bucketZombies);
	return nullptr;
}

void BucketZombies::createZombie()
{
	zombieInit();

	setZombieAnimationInformation(rand() % 2 + 2);

	setZombieScale();

	createZombieShadow();

	_zombiesAnimation->setAttachment("Zombie_bucket", "Zombie_bucket1");
}

void BucketZombies::createPreviewZombie()
{
	zombieInit();

	setZombieAnimationInformation(rand() % 2);

	setZombieScale();

	createZombieShadow();

	_zombiesAnimation->setAttachment("Zombie_bucket", "Zombie_bucket1");
}

void BucketZombies::zombieInjuredEventUpdate()
{
	if (_isHaveShield)/* �����ͷ������ */
	{
		if (_currentHeadShieldVolume <= _headShieldVolume * 2.0f / 3.0f) /* ͷ������һ������ */
		{
			setZombieHeadShieldPrimaryInjure("Zombie_bucket", "Zombie_bucket2");
		}
		if (_currentHeadShieldVolume <= _headShieldVolume / 3.0f)       /* ͷ�����ܶ������� */
		{
			setZombieHeadShieldSecondaryInjure("Zombie_bucket", "Zombie_bucket3");
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

void BucketZombies::setZombieHeadShieldThirdInjure(const string& oldName, const string& newName)
{
	if (_animationId == 6) /* ���������˺� */
	{
		_zombiesAnimation->setAttachment("Zombie_bucket", "tt_innerleg_foot3");
		_animationId = 7;
		_isHaveShield = false;
		_attackHeadSoundEffectType = 0;

		/* ��ʬ������ */
		zombieLoseShieldAnimation("Zombie_bucket");
	}
}
