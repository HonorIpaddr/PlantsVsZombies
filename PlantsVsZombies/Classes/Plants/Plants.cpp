/**
 *Copyright (c) 2020 LZ.All Right Reserved
 *Author : LZ
 *Date: 2020.2.5
 *Email: 2117610943@qq.com
 */

#include "Plants.h"

#include "Zombies/Zombies.h"
#include "Scenes/GameScene/GSData.h"

int Plants::plantNumber = -1;

Plants::Plants(Node* node, const Vec2& position) :
	_node(node)
,	_position(position)
,	_plantShadow(nullptr)
,	_plantImage(nullptr)
,	_plantAnimation(nullptr)
,	_soilSplashAnimation(nullptr)
,	_sunNeed(NOINITIALIZATION)
,	_coolDownTime(NOINITIALIZATION)
,	_costGold(NOINITIALIZATION)
,	_costMasonry(NOINITIALIZATION)
,	_healthPoint(NOINITIALIZATION)
,	_plantTag(NOINITIALIZATION)
,	_isLoop(true)
,   _isCanDelete{ false,false }
,   _zOrder(0)
,   _plantNumber(++plantNumber)
,   _combatEffecttiveness(0)
,   _plantsType(PlantsType::None)
,	_global(Global::getInstance())
{
}

Plants::~Plants()
{
}

SkeletonAnimation* Plants::plantInit(const std::string& plantname, const std::string& animaionname)
{
	auto iter = _global->userInformation->getAnimationData().find(plantname);
	if (iter != _global->userInformation->getAnimationData().end())/* ��������ҵ� */
	{
		_plantAnimation = SkeletonAnimation::createWithData(iter->second);
		_plantAnimation->setPosition(_position);
		_plantAnimation->setAnimation(0, animaionname, _isLoop);
		_plantAnimation->setLocalZOrder(_zOrder);
		_plantAnimation->setTag(_plantTag);
		return _plantAnimation;
	}
	return nullptr;
}

void Plants::imageInit(const std::string& name, const Vec2& position)
{
	auto iter = _global->userInformation->getImagePath().find(name);
	if (iter != _global->userInformation->getImagePath().end())
	{
		_plantImage = Sprite::create(iter->second);
		_plantImage->setPosition(position);
		_plantImage->setName("Preview");
		_plantImage->setOpacity(150);
		_node->addChild(_plantImage, 99);
	}
}

void Plants::determineRelativePositionPlantsAndZombies()
{
	for (auto zombie : ZombiesGroup)
	{
		zombieEatPlant(zombie);      /* ��ʬ��ֲ�� */

		zombieRecoveryMove(zombie);  /* ��ʬ�ָ��ƶ� */
	}
}

void Plants::playSoundEffect(const std::string& MusicName)
{
	AudioEngine::setVolume(AudioEngine::play2d(Global::getInstance()->userInformation->getMusicPath().find(MusicName)->second), Global::getInstance()->userInformation->getSoundEffectVolume());
}

void Plants::stopPlantsAllAction()
{
	for (auto& sunFlower : PlantsGroup)
	{
		sunFlower.second->getPlantAnimation()->stopAllActions();
	}
}

void Plants::setPlantSoilSplashAnimation(const float& scale)
{
	/* ������ֲ�����ɽ����� */
	auto iter = _global->userInformation->getAnimationData().find("SplashOfSoil");
	if (iter != _global->userInformation->getAnimationData().end())
	{
		_soilSplashAnimation = SkeletonAnimation::createWithData(iter->second);
		_soilSplashAnimation->setPosition(Vec2(0, -50));
		_soilSplashAnimation->setAnimation(0, "SplashOfSoil", false);
		_soilSplashAnimation->setScale(scale);
		_soilSplashAnimation->setGlobalZOrder(_plantAnimation->getGlobalZOrder());
		_plantAnimation->addChild(_soilSplashAnimation);
	}
}

void Plants::setPlantShadow(const float& scale)
{
	/* ����ֲ��Ӱ�� */
	auto iter = _global->userInformation->getImagePath().find("plantshadow");
	if (iter != _global->userInformation->getImagePath().end())
	{
		_plantShadow = Sprite::create(iter->second);
		_plantShadow->setScale(scale);
		_plantShadow->setPosition(Vec2(-5, 15));
		_plantShadow->setName("plantshadow");
		_plantAnimation->addChild(_plantShadow, -1);
	}
}

void Plants::setPlantVisible(const bool visible)
{
	_plantAnimation->setVisible(visible);
}

bool Plants::getPlantVisible()
{
	return _plantAnimation->isVisible();
}

bool* Plants::getPlantIsCanDelete()
{
	return _isCanDelete;
}

bool Plants::getZombieIsTheFrontOfPlant(Zombies* zombie) const
{
	return zombie->getZombiePositionX() >= _plantAnimation->getPositionX() ? true : false;
}

bool Plants::getZombieIsSameLineWithPlant(Zombies* zombie) const
{
	return fabs(zombie->getZombiePositionY() - 10 - _plantAnimation->getPositionY()) <= 10 ? true : false;
}

bool Plants::getzombieIsEncounterPlant(Zombies* zombie) const
{
	return fabs(zombie->getZombiePositionX() - _plantAnimation->getPositionX()) <= 80 ? true : false;
}

void Plants::zombieEatPlant(Zombies* zombie)
{
	if (getPlantIsSurvive() && getZombieIsSameLineWithPlant(zombie) && getzombieIsEncounterPlant(zombie)) /* ֲ���� && ֲ���뽩ʬ��ͬһ�� && ��ʬ����ֲ�� */
	{
		if (zombie->getZombieIsSurvive() && !zombie->getZombieIsEat())
		{
			const string eateffect[3] = { "chomp","chomp2","chompsoft" };
			zombie->setZombieEatPlantNumber(_plantNumber);
			zombie->setZombieStop();
			zombie->setZombieIsEat(true);
			zombie->getZombieAnimation()->setAnimation(1, "Zombies_Eat", true);
			zombie->getZombieAnimation()->setEventListener([this, eateffect](spTrackEntry* entry, spEvent* event)
				{
					if (!strcmp(event->data->name, "eat"))
					{
						if (event->intValue == 1)
						{
							reducePlantHealthPoint(100);
							playSoundEffect(eateffect[rand() % 3]);
							_plantAnimation->runAction(Sequence::create(TintTo::create(0.2f, Color3B(100, 100, 100)), TintTo::create(0.2f, Color3B::WHITE), nullptr));
						}
					}
				});
		}
	}
}

void Plants::zombieRecoveryMove(Zombies* zombie)
{
	if (!getPlantIsSurvive() && zombie->getZombieEatPlantNumber() == _plantNumber &&  /* ֲ������ && ��ʬ�ǳԵĸ�ֲ�� */
		zombie->getZombieIsEat() && zombie->getZombieIsStop()) /* ��ʬ���ڳ�ֲ�� && ��ʬ����ֹͣ�ƶ� */
	{
		setPlantVisible(false);
		zombie->setZombieIsEat(false);
		if (!zombie->getZombieIsPlayDieAnimation()) /* ��ʬû�в����������� */
		{
			zombie->getZombieAnimation()->setMix("Zombies_Eat", Zombies::getZombieAniamtionName(zombie->getZombieType()), 1.5f);
			zombie->getZombieAnimation()->addAnimation(1, Zombies::getZombieAniamtionName(zombie->getZombieType()), true);
			zombie->setZombieCurrentSpeed(zombie->getZombieSpeed());
		}
	}
}
