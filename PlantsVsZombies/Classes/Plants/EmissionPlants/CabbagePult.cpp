/**
 *Copyright (c) 2020 LZ.All Right Reserved
 *Author : LZ
 *Date: 2020.2.6
 *Emal: 2117610943@qq.com
 */

#include "CabbagePult.h"
#include "PeaShooter.h"
#include "Bullet/Cabbage.h"
#include "Bullet/Bullet.h"

#include "Zombies/Zombies.h"
#include "Scenes/GameScene/GSData.h"

#define _MAX_ 0xffffff

CabbagePult::CabbagePult(Node* node):
    _isCreateCabbage(false)
,   _zombiePostion(Vec2::ZERO)
,   _zombie(nullptr)
{
	_node = node;
	_plantImage = nullptr;

	_healthPoint = 300;
	_combatEffecttiveness = 60; // !!!��ʱû��ʹ�� ʹ�õ���cabbage�����_attack
	_plantsType = PlantsType::CabbagePult;
}

CabbagePult::~CabbagePult()
{
}

CabbagePult* CabbagePult::create(Node* node)
{
	CabbagePult* cabbagePult = new (std::nothrow) CabbagePult(node);
	if (cabbagePult && cabbagePult->init())
	{
		cabbagePult->autorelease();
		return cabbagePult;
	}
	CC_SAFE_DELETE(cabbagePult);
	return nullptr;
}
Sprite* CabbagePult::createPlantImage()
{
	imageInit("Cabbage", INIT);
	_plantImage->setScale(1.5f);
	return _plantImage;
}

void CabbagePult::createPlantAnimation()
{
	_plantAnimation = plantInit("Cabbage", "Cabbage_Normal");
	_plantAnimation->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	_plantAnimation->setScale(0.8f);
	_node->addChild(_plantAnimation);

	// Ӱ��
	setPlantShadow(2.6f);

	// �����ɽ�����
	setPlantSoilSplashAnimation(1.0f);
}

void CabbagePult::determineRelativePositionPlantsAndZombies()
{
	_distance = _MAX_; /* ��ʼ��������� */

	for (auto zombie : ZombiesGroup)
	{
		zombieEatPlant(zombie);      /* ��ʬ��ֲ�� */

		plantAttack(zombie);         /* ֲ�﹥�� */

		zombieRecoveryMove(zombie);  /* ��ʬ�ָ��ƶ� */
	}

	if (_isCreateCabbage)  /* ����н�ʬ����Ĳ�Ͷ����ͬһ�� */
	{
		_zombiePostion = _zombie->getZombieAnimation()->getPosition();
		_plantAnimation->setEventListener([&](spTrackEntry* entry, spEvent* event)
			{
				if (strcmp(event->data->name, "Shoot") == 0)
				{
					rand() % 2 == 0 ? playSoundEffect("throw") : playSoundEffect("throw2");
					createCabbage();
				}
			});
	}

	if (!_isCreateCabbage) /* ���û�н�ʬ����Ĳ�Ͷ����ͬһ�� */
	{
		if (_isChanged)
		{
			_plantAnimation->addAnimation(0, "Cabbage_Normal", true);
			_isChanged = false;
		}
	}

	_isCreateCabbage = false; /* ÿѭ��һ�ξͳ�ʼ�� */
}

void CabbagePult::plantAttack(Zombies* zombie)
{
	if (zombie->getZombieIsSurvive() && getPlantIsSurvive() && zombie->getZombieIsEnterMap() && /* ��ʬ��� && ֲ���� && ��ʬ�����ͼ */
		getZombieIsSameLineWithPlant(zombie) && getZombieIsTheFrontOfPlant(zombie))             /* ��ֲ����ͬһ�� && ��ʬ��ֲ���ǰ�� */
	{
		_isCreateCabbage = true; /* ��ʾ�н�ʬ��ֲ����ͬһ�� */
		if (!_isChanged)
		{
			_plantAnimation->addAnimation(0, "Cabbage_Shoot", true);
			_isChanged = true;
		}

		float distance = zombie->getZombieAnimation()->getPositionX() - _plantAnimation->getPositionX();
		if (_distance > distance) /* ��ʬ��ֲ��ľ���С����һ����ʬ��ֲ��ľ��� */
		{
			_distance = distance;
			_zombie = zombie;     /* ��¼�ý�ʬ */
		}
	}
}

void CabbagePult::createCabbage()
{
	_bulletAnimation = new Cabbage(_node);
	_bulletAnimation->setBulletPosition(_position);
	dynamic_cast<Cabbage*>(_bulletAnimation)->setZombiePosition(_zombiePostion);
	_bulletAnimation->createBullet();

	BulletGroup.push_back(_bulletAnimation);
}


