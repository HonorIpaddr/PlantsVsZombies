/**
 *Copyright (c) 2020 LZ.All Right Reserved
 *Author : LZ
 *Date: 2020.2.25
 *Email: 2117610943@qq.com
 */

#include "AcidLemon.h"
#include "Bullet/AcidLemonBullet.h"

#include "Zombies/Zombies.h"
#include "Scenes/GameScene/GSData.h"

AcidLemonShooter::AcidLemonShooter(Node* node) :
	_isCreateAcidLemon(false)
{
	_node = node;
	_plantImage = nullptr;

	_healthPoint = 300;
	_combatEffecttiveness = 0;
	_plantsType = PlantsType::AcidLemonShooter;
}

AcidLemonShooter::~AcidLemonShooter()
{
}

AcidLemonShooter* AcidLemonShooter::create(Node* node)
{
	AcidLemonShooter* acidLemonShooter = new (std::nothrow) AcidLemonShooter(node);
	if (acidLemonShooter && acidLemonShooter->init())
	{
		acidLemonShooter->autorelease();
		return acidLemonShooter;
	}
	CC_SAFE_DELETE(acidLemonShooter);
	return nullptr;
}

Sprite* AcidLemonShooter::createPlantImage()
{
	imageInit("LemonShooter", INIT);
	_plantImage->setScale(1.3f);
	return _plantImage;
}

void AcidLemonShooter::createPlantAnimation()
{
	_plantAnimation = plantInit("LemonShooter", "LemonNormal");
	_plantAnimation->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	_plantAnimation->setScale(1.2f);
	_plantAnimation->setTimeScale(0.8f);
	_node->addChild(_plantAnimation);

	// Ӱ��
	setPlantShadow(1.5f);
	_plantAnimation->getChildByName("plantshadow")->setPosition(Vec2(0, 10));

	// �����ɽ�����
	setPlantSoilSplashAnimation(0.8f);
}

void AcidLemonShooter::determineRelativePositionPlantsAndZombies()
{
	for (auto zombie : ZombiesGroup)
	{
		zombieEatPlant(zombie);      /* ��ʬ��ֲ�� */

		plantAttack(zombie);         /* ֲ�﹥�� */

		zombieRecoveryMove(zombie);  /* ��ʬ�ָ��ƶ� */
	}

	plantRecovery("LemonNormal");
}

void AcidLemonShooter::plantAttack(Zombies* zombie)
{
	if (getPlantIsSurvive() && zombie->getZombieIsSurvive() && zombie->getZombieIsEnterMap() &&     /* ֲ���� && ��ʬû������ && ��ʬ�����ͼ */
		getZombieIsTheFrontOfPlant(zombie) && getZombieIsSameLineWithPlant(zombie))                 /* ��ʬ��ֲ����ͬһ�� && ��ʬ��ֲ���ǰ�� */
	{
		plantEmission("LemonShooter");
	}
}

void AcidLemonShooter::plantEmission(const string& plantAnimation)
{
	_isCreateAcidLemon = true; /* ��ʾ�н�ʬ��ֲ����ͬһ�� */
	if (!_isChanged)     /* �ж϶����Ƿ��Ѿ��ı� */
	{
		_plantAnimation->addAnimation(0, plantAnimation, true);
		_isChanged = true;
	}

	_plantAnimation->setEventListener([&](spTrackEntry* entry, spEvent* event)
		{
			if (strcmp(event->data->name, "shoot") == 0)
			{
				rand() % 2 == 0 ? Plants::playSoundEffect("throw") : Plants::playSoundEffect("throw2");
				createAcidLemon();
			}
		});
}

void AcidLemonShooter::plantRecovery(const string& plantAnimation)
{
	if (!_isCreateAcidLemon)  /* �ж��Ƿ��н�ʬ��ǿ������������ͬһ�� */
	{
		if (_isChanged)
		{
			_plantAnimation->addAnimation(0, plantAnimation, true);
			_isChanged = false;
		}
	}

	_isCreateAcidLemon = false; /* ÿѭ��һ�ξͳ�ʼ�� */
}

void AcidLemonShooter::createAcidLemon()
{
	_bulletAnimation = new AcidLemonBullet(_node);
	_bulletAnimation->setBulletPosition(_position);
	_bulletAnimation->createBullet();

	BulletGroup.push_back(_bulletAnimation);
}