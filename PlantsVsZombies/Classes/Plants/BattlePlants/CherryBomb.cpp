/**
 *Copyright (c) 2020 LZ.All Right Reserved
 *Author : LZ
 *Date: 2020.2.6
 *Email: 2117610943@qq.com
 */

#include "CherryBomb.h"

#include "Scenes/GameScene/GSData.h"
#include "Zombies/Zombies.h"

CherryBomb::CherryBomb(Node* node):
	_isReadyExplode(false)
,   _isExplode(false)
{
	_node = node;
	_plantImage = nullptr;

	_healthPoint = 300;
	_isLoop = false;
	_combatEffecttiveness = 1800;
	_plantsType = PlantsType::CherryBomb;
}

CherryBomb::~CherryBomb()
{
}

CherryBomb* CherryBomb::create(Node* node)
{
	CherryBomb* cherryBomb = new (std::nothrow) CherryBomb(node);
	if (cherryBomb && cherryBomb->init())
	{
		cherryBomb->autorelease();
		return cherryBomb;
	}
	CC_SAFE_DELETE(cherryBomb);
	return nullptr;
}
Sprite* CherryBomb::createPlantImage()
{
	imageInit("CherryBomb", INIT);
	_plantImage->setScale(1.3f);
	return _plantImage;
}

void CherryBomb::createPlantAnimation()
{
	_plantAnimation = plantInit("CherryBomb", "CherryBomb_Explode");
	_plantAnimation->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	_plantAnimation->setTimeScale(1.5f);
	_node->addChild(_plantAnimation);

	// Ӱ��
	setPlantShadow(1.8f);

	// �����ɽ�����
	setPlantSoilSplashAnimation(0.8f);

	// �¼�����
	setEventListener();
}

void CherryBomb::setEventListener()
{
	_plantAnimation->setEventListener([this](spTrackEntry* entry, spEvent* event)
		{
			if (!strcmp(event->data->name, "Finished"))
			{
				_isReadyExplode = true;
				_healthPoint = 0;
				_plantAnimation->setVisible(false);
			}
		});
	playSoundEffect("wakeup");
}

bool CherryBomb::getPlantIsReadyExplode() const
{
	return _isReadyExplode;
}

bool CherryBomb::getZombieIsInExplodeRange(Zombies* zombie) const
{
	/* ��ʬ�Ƿ��ڱ�ը��Χ�ж� */
	return sqrt(pow(zombie->getZombieAnimation()->getPositionX() - _plantAnimation->getPositionX(), 2) +
		pow(zombie->getZombieAnimation()->getPositionY() - _plantAnimation->getPositionY(), 2)) <= 195 ? true : false;
}

void CherryBomb::determineRelativePositionPlantsAndZombies()
{
	for (auto zombie : ZombiesGroup)
	{
		zombieEatPlant(zombie);      /* ��ʬ��ֲ�� */

		plantExplode();              /* ֲ�﹥�� */

		zombieRecoveryMove(zombie);  /* ��ʬ�ָ��ƶ� */
	}
}

void CherryBomb::plantExplode()
{
	if (!getPlantIsSurvive() || getPlantIsReadyExplode()) /* ֲ������ || ֲ��׼����ը */
	{
		if (!_isExplode) /* ���û�б�ը */
		{
			_isExplode = true;
			explodeHurtZombies();
			showExplodeAnimation();
		}
	}
}

void CherryBomb::explodeHurtZombies()
{
	for (auto zombie : ZombiesGroup)
	{
		if (zombie->getZombieIsSurvive() && zombie->getZombieIsEnterMap() && getZombieIsInExplodeRange(zombie)) /* ��ʬ��� && ��ʬ�����ͼ && ��ʬ�ڱ�ը��Χ�� */
		{
			hurtZombies(zombie);
			zombie->setZombieIsShowLoseLimbsAnimation(false);

			if (!zombie->getZombieIsSurvive())
			{
				zombie->getZombieAnimation()->setVisible(false);
				if (zombie->getZombieType() == ZombiesType::LmpZombies)
				{
					zombie->playZombiesDieAnimation("LmpZombies_Charre");
				}
				else
				{
					zombie->playZombiesDieAnimation("Zombies_Ash");
				}
			}
		}
	}
}

void CherryBomb::showExplodeAnimation()
{
	playSoundEffect("cherrybomb");

	/* ��ը���� */
	auto cherryBomb_Explode = SkeletonAnimation::createWithData(_global->userInformation->getAnimationData().find("CherryBomb_Explode")->second);
	cherryBomb_Explode->setPosition(_plantAnimation->getPosition());
	cherryBomb_Explode->setAnimation(0, "CherryBomb_Explode", false);
	cherryBomb_Explode->setScale(1.5f);
	cherryBomb_Explode->setLocalZOrder(_plantAnimation->getLocalZOrder() + 10); // ֲ�����˳���10���õ��ڽ�ʬ����˳�� ����ը�Ϳ��Ը��ǵ���ʬ����
	_node->addChild(cherryBomb_Explode);

	cherryBomb_Explode->setEventListener([cherryBomb_Explode](spTrackEntry* entry, spEvent* event)
		{
			if (!strcmp(event->data->name, "Finished"))
			{
				cherryBomb_Explode->runAction(FadeOut::create(0.5f));
			}
		});
	_node->runAction(Sequence::create(DelayTime::create(2.0f), CallFunc::create([cherryBomb_Explode]() {cherryBomb_Explode->removeFromParent(); }), nullptr));
}
