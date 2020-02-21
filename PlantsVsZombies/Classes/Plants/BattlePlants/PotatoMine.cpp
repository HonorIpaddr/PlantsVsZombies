/**
 *Copyright (c) 2020 LZ.All Right Reserved
 *Author : LZ
 *Date: 2020.2.6
 *Email: 2117610943@qq.com
 */

#include "PotatoMine.h"

#include "Zombies/Zombies.h"
#include "Scenes/GameScene/GSData.h"

PotatoMine::PotatoMine(Node* node):
	_isReady(false)
,   _isBeginExplode(false)
,   _isExplodeFinished(false)
,   _isCanKillZombies(false)
,   _isKillZombiesFinished(false)
,   _breakGround(15.f)
{
	_node = node;
	_plantImage = nullptr;

	_healthPoint = 300;
	_isLoop = false;
	_combatEffecttiveness = 1800;
	_plantsType = PlantsType::PotatoMine;
}

PotatoMine::~PotatoMine()
{
}

PotatoMine* PotatoMine::create(Node* node)
{
	PotatoMine* potatoMine = new (std::nothrow) PotatoMine(node);
	if (potatoMine && potatoMine->init())
	{
		potatoMine->autorelease();
		return potatoMine;
	}
	CC_SAFE_DELETE(potatoMine);
	return nullptr;
}
Sprite* PotatoMine::createPlantImage()
{
	imageInit("PotatoMine", INIT);
	_plantImage->setScale(1.3f);
	return _plantImage;
}

void PotatoMine::createPlantAnimation()
{
	_plantAnimation = plantInit("PotatoMine", "PotatoMine_Ready");
	_plantAnimation->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	_plantAnimation->setScale(1.0f);
	_node->addChild(_plantAnimation);

	// Ӱ��
	setPlantShadow(1.8f);

	// �����ɽ�����
	setPlantSoilSplashAnimation(0.8f);

	// ��������
	createListener();
}

void PotatoMine::createListener()
{
	_plantAnimation->runAction(Sequence::create(DelayTime::create(_breakGround), CallFunc::create([this]()
		{
			AudioEngine::setVolume(AudioEngine::play2d(_global->userInformation->getMusicPath().find("dirt_rise")->second), _global->userInformation->getSoundEffectVolume());
			_isReady = true;
			_plantAnimation->setAnimation(0, "PotatoMine_Out", false);
			_plantAnimation->addAnimation(0, "PotatoMine_Normal", true);
		}), nullptr));
}

void PotatoMine::determineRelativePositionPlantsAndZombies()
{
	for (auto zombie : ZombiesGroup)
	{
		zombieEatPlant(zombie);      /* ��ʬ��ֲ�� */

		plantExplode();              /* ֲ�﹥�� */

		zombieRecoveryMove(zombie);  /* ��ʬ�ָ��ƶ� */
	}
}

void PotatoMine::plantExplode()
{
	if (_isBeginExplode && !_isExplodeFinished)/* �����ױ�ը */
	{
		_isExplodeFinished = true;
		_plantAnimation->setScale(1.2f);
		_plantAnimation->setAnimation(0, "PotatoMine_Blast", false);
		_plantAnimation->setEventListener([&](spTrackEntry* entry, spEvent* event)
			{
				if (strcmp(event->data->name, "BlastBegin") == 0)
				{
					_isCanKillZombies = true;
				}
			});
		_plantAnimation->runAction(Sequence::create(DelayTime::create(2.f),
			CallFunc::create([&]()
				{
					_plantAnimation->setVisible(false);
				}), nullptr));
	}

	if (_isCanKillZombies&& !_isKillZombiesFinished)
	{
		explodeHurtZombies();
		_isKillZombiesFinished = true;
	}
}

void PotatoMine::zombieEatPlant(Zombies* zombie)
{
	if (getPlantIsSurvive() && getZombieIsSameLineWithPlant(zombie) && getzombieIsEncounterPlant(zombie)) /* ֲ���� && ֲ���뽩ʬ��ͬһ�� && ��ʬ����ֲ�� */
	{
		if (_isReady)  /* ������׼���� */
		{
			_isBeginExplode = true;
		}

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

void PotatoMine::explodeHurtZombies()
{
	AudioEngine::setVolume(AudioEngine::play2d(_global->userInformation->getMusicPath().find("potato_mine")->second), _global->userInformation->getSoundEffectVolume());

	for (auto zombie : ZombiesGroup)
	{
		if (zombie->getZombieIsSurvive() && zombie->getZombieIsEnterMap() && getZombieIsInExplodeRange(zombie)) /* ��ʬ��� && ��ʬ�����ͼ && ��ʬ�ڱ�ը��Χ�� */
		{
			hurtZombies(zombie);

			if (!zombie->getZombieIsSurvive())
			{
				zombie->getZombieAnimation()->setVisible(false);
			}
		}
	}
}

bool PotatoMine::getZombieIsInExplodeRange(Zombies* zombie)
{
	return (getZombieIsSameLineWithPlant(zombie) && fabs(zombie->getZombieAnimation()->getPositionX() - _plantAnimation->getPositionX()) < 190) ? true : false;
}
