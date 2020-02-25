/**
 *Copyright (c) 2020 LZ.All Right Reserved
 *Author : LZ
 *Date: 2020.2.25
 *Email: 2117610943@qq.com
 */

#include "AcidLemonBullet.h"

#include "../PeaShooter.h"
#include "Zombies/Zombies.h"
#include "Scenes/GameScene/GSData.h"

AcidLemonBullet::AcidLemonBullet(Node* node)
{
    _node = node;
    _attack = 20;
    _bulletType = BulletType::AcidLemonBullet;
}

AcidLemonBullet::~AcidLemonBullet()
{
}

void AcidLemonBullet::createBullet()
{
    bulletInit();
    createShadow();
}

void AcidLemonBullet::bulletAndZombiesCollision()
{
	for (auto zombie : ZombiesGroup)
	{
		if (!_isUsed && getBulletIsSameLineWithZombie(zombie) &&  /* û�б�ʹ�� && �������뽩ʬ��ͬһ�� */
			getBulletIsEncounterWithZombie(zombie) &&             /* �������뽩ʬ��ײ */
			zombie->getZombieIsSurvive() && zombie->getZombieIsEnterMap()) /* ��ʬû������ && ��ʬ�����ͼ�� */
		{
			auto sound = zombie->getZombieHeadAttackSoundEffect() + zombie->getZombieBodyAttackSoundEffect();
			PeaShooter::playSoundEffect(static_cast<SoundEffectType>(sound > 1 ? 1 : sound));  /* ����ָ������ */

			setBulletOpacity();                /* �ӵ���ʧ */
			setAttackForShield(zombie);        /* �Խ�ʬ���ʻ�����ɶ����˺� */
			bulletAttackHurtZombies(zombie);   /* ��ʬ��������ֵ */

			zombie->getZombieAnimation()->runAction(Sequence::create(TintTo::create(0.2f, Color3B(100, 100, 100)), TintTo::create(0.2f, Color3B::WHITE), nullptr));

			createAcidLemonBulletExplode();   /* ������ը���� */
			setBulletAttack(0);
			setBulletIsUsed(true);

			break; /* һ���㶹ֻ�ܻ���һ����ʬ */
		}
	}
}

void AcidLemonBullet::bulletInit()
{
	_bulletAnimation = SkeletonAnimation::createWithData(_global->userInformation->getAnimationData().find("LemonJuice")->second);
	_bulletAnimation->setAnimation(0, rand() % 2 ? "shoot" : "shoot1", true);
	_bulletAnimation->setPosition(_position + Vec2(70, 90));
	_bulletAnimation->setScale(0.9f);
	_bulletAnimation->setName(_bulletName);
	_bulletAnimation->setLocalZOrder(getZOrder(_position.y));
	_node->addChild(_bulletAnimation);
	_bulletAnimation->runAction(Sequence::create(MoveBy::create(2.0f, Vec2(2000, 0)),
		CallFunc::create([this]()
			{
				_bulletAnimation->setVisible(false);
			}), nullptr));
}

void AcidLemonBullet::createShadow()
{
	/* ����Ӱ�� */
	auto shadow = Sprite::create(_global->userInformation->getImagePath().find("plantshadow")->second);
	shadow->setScaleX(1.7f);
	shadow->setName("shadow");
	shadow->setPosition(Vec2(0, -92));
	shadow->setLocalZOrder(_bulletAnimation->getLocalZOrder());
	_bulletAnimation->addChild(shadow, -1);
}

void AcidLemonBullet::createAcidLemonBulletExplode()
{
	auto explode = SkeletonAnimation::createWithData(_global->userInformation->getAnimationData().find("PeaExplode")->second);
	explode->setPosition(getBulletPosition());
	explode->setAnimation(0, "Explode4", false);
	explode->setLocalZOrder(_bulletAnimation->getLocalZOrder());
	_node->addChild(explode);

	explode->runAction(Sequence::create(DelayTime::create(0.8f), CallFunc::create([explode]()
		{
			explode->removeFromParent();
		}), nullptr));
}

void AcidLemonBullet::setAttackForShield(Zombies* zombie)
{
	if (zombie->getZombieIsHaveShield()) /* ��ʬ�л��� */
	{
		if (zombie->getZombieBodyAttackSoundEffect() || zombie->getZombieHeadAttackSoundEffect()) /* ��ʬ�����ʻ��� */
		{
			_attack = 100;
		}
	}
	else
	{
		_attack = 20;
	}
}
