/**
 *Copyright (c) 2020 LZ.All Right Reserved
 *Author : LZ
 *Date: 2020.2.6
 *Email: 2117610943@qq.com
 */

#include "Spikeweed.h"

#include "../EmissionPlants/PeaShooter.h"
#include "Zombies/Zombies.h"
#include "Scenes/GameScene/GSData.h"

Spikeweed::Spikeweed(Node* node):
	_isAttack(false)
,   _isHaveZombies(false)
,   _isChanged(false)
{
	_node = node;
	_plantImage = nullptr;

	_healthPoint = 300;
	_combatEffecttiveness = 20;
	_plantsType = PlantsType::Spikeweed;
}

Spikeweed::~Spikeweed()
{
}

Spikeweed* Spikeweed::create(Node* node)
{
	Spikeweed* spikeweed = new (std::nothrow) Spikeweed(node);
	if (spikeweed && spikeweed->init())
	{
		spikeweed->autorelease();
		return spikeweed;
	}
	CC_SAFE_DELETE(spikeweed);
	return nullptr;
}
Sprite* Spikeweed::createPlantImage()
{
	imageInit("Spikeweed", INIT);
	_plantImage->setScale(1.0f);
	return _plantImage;
}

void Spikeweed::createPlantAnimation()
{
	_plantAnimation = plantInit("Spikeweed", "Spikeweed_Normal");
	_plantAnimation->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	_plantAnimation->setScale(0.8f);
	_node->addChild(_plantAnimation);

	// Ӱ��
	setPlantShadow(1.0f);

	// �����ɽ�����
	setPlantSoilSplashAnimation(0.8f);
}

void Spikeweed::determineRelativePositionPlantsAndZombies()
{
	plantAttack();         /* ֲ�﹥�� */

	// ��ʬ��ֲ������˺������� �ڴ����
}

void Spikeweed::plantAttack()
{
	for (auto zombie : ZombiesGroup)
	{
		if (zombie->getZombieIsSurvive() && getZombieIsSameLineWithPlant(zombie) && getzombieIsEncounterPlant(zombie))       /* ��ʬû������ && ��ʬ��ֲ����ͬһ�� && ��ʬ����ֲ�� */
		{
			plantAnimationChange(zombie);
		}
	}

	plantAnimationRecovery();
}

void Spikeweed::plantAnimationChange(Zombies* zombie)
{
	_isHaveZombies = true; /* �н�ʬ��� */
	if (!_isChanged)       /* �ж϶����Ƿ��Ѿ��ı� */
	{
		_plantAnimation->addAnimation(0, "Spikeweed_Attack", true);
		_isChanged = true;
	}

	_plantAnimation->setEventListener([this](spTrackEntry* entry, spEvent* event)
		{
			if (strcmp(event->data->name, "Attack") == 0)
			{
				_isAttack = false;
			}
		});

	if (!_isAttack)
	{
		auto sound = zombie->getZombieHeadAttackSoundEffect() + zombie->getZombieBodyAttackSoundEffect();
		PeaShooter::playSoundEffect(static_cast<SoundEffectType>(sound > 1 ? 1 : sound));

		zombie->getZombieAnimation()->runAction(Sequence::create(TintTo::create(0.2f, Color3B(100, 100, 100)), TintTo::create(0.2f, Color3B::WHITE), nullptr));

		hurtZombies(zombie);
	}
}

void Spikeweed::plantAnimationRecovery()
{
	if (!_isHaveZombies && _isChanged) /* û�н�ʬ */
	{
		_plantAnimation->addAnimation(0, "Spikeweed_Normal", true);
		_isChanged = false;
	}
	_isHaveZombies = false;
	_isAttack = true;
}
