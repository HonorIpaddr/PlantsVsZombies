/**
 *Copyright (c) 2020 LZ.All Right Reserved
 *Author : LZ
 *Date: 2020.2.6
 *Email: 2117610943@qq.com
 */

#include "PeaShooter.h"
#include "Bullet/Bullet.h"
#include "Bullet/Pea.h"

#include "Zombies/Zombies.h"
#include "Scenes/GameScene/GSData.h"


PeaShooter::PeaShooter(Node* node):
	_isCreatePea(false)
{
	_node = node;
	_plantImage = nullptr;

	_healthPoint = 300;
	_combatEffecttiveness = 20;
	_plantsType = PlantsType::PeaShooter;
}

PeaShooter::~PeaShooter()
{
}

PeaShooter* PeaShooter::create(Node* node)
{
	PeaShooter* peaShooter = new (std::nothrow) PeaShooter(node);
	if (peaShooter && peaShooter->init())
	{
		peaShooter->autorelease();
		return peaShooter;
	}
	CC_SAFE_DELETE(peaShooter);
	return nullptr;
}

Sprite* PeaShooter::createPlantImage()
{
	imageInit("PeaShooter", INIT);
	_plantImage->setScale(1.3f);
	return _plantImage;
}

void PeaShooter::createPlantAnimation()
{
	_plantAnimation = plantInit("PeaShooter", "PeaShooter_Normal");
	_plantAnimation->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	_plantAnimation->setScale(1.0f);
	_node->addChild(_plantAnimation);

	// Ӱ��
	setPlantShadow(1.9f);
	_plantAnimation->getChildByName("plantshadow")->setPosition(Vec2(0, 15));

	// �����ɽ�����
	setPlantSoilSplashAnimation(0.8f);
}

void PeaShooter::determineRelativePositionPlantsAndZombies()
{
	for (auto zombie : ZombiesGroup)
	{
		zombieEatPlant(zombie);      /* ��ʬ��ֲ�� */

		zombieRecoveryMove(zombie);  /* ��ʬ�ָ��ƶ� */

		plantAttack(zombie);         /* ֲ�﹥�� */
	}

	plantRecovery("PeaShooter_Normal");
}

void PeaShooter::plantAttack(Zombies* zombie)
{
	if (getPlantIsSurvive() && zombie->getZombieIsSurvive() && zombie->getZombieIsEnterMap() &&     /* ֲ���� && ��ʬû������ && ��ʬ�����ͼ */
		getZombieIsTheFrontOfPlant(zombie) && getZombieIsSameLineWithPlant(zombie))                 /* ��ʬ��ֲ����ͬһ�� && ��ʬ��ֲ���ǰ�� */
	{
		plantEmission("PeaShooter_Shoot");
	}
}

void PeaShooter::plantEmission(const string& plantAnimation)
{
	_isCreatePea = true; /* ��ʾ�н�ʬ��ֲ����ͬһ�� */
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
				createPea();
			}
		});
}

void PeaShooter::plantRecovery(const string& plantAnimation)
{
	if (!_isCreatePea) /* �ж��Ƿ��н�ʬ���㶹������ͬһ�� */
	{
		if (_isChanged)
		{
			_plantAnimation->addAnimation(0, plantAnimation, true);
			_isChanged = false;
		}
	}

	_isCreatePea = false; /* ÿѭ��һ�ξͳ�ʼ�� */
}

void PeaShooter::playSoundEffect(SoundEffectType soundEffect)
{
	switch (soundEffect)
	{
	case SoundEffectType::kernelpult: rand() % 2 == 0 ? Plants::playSoundEffect("kernelpult") : Plants::playSoundEffect("kernelpult2"); break;
	case SoundEffectType::shieldhit:  rand() % 2 == 0 ? Plants::playSoundEffect("shieldhit") : Plants::playSoundEffect("shieldhit2");   break;
	case SoundEffectType::firepea:    Plants::playSoundEffect("firepea"); break;
	}
}

void PeaShooter::createPea()
{
	_bulletAnimation= new Pea(_node);
	_bulletAnimation->setBulletPosition(_position);
	_bulletAnimation->createBullet();

	BulletGroup.push_back(_bulletAnimation);
}