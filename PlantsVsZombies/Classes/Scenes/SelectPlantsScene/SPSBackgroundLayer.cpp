/**
 *Copyright (c) 2019 LZ.All Right Reserved
 *Author : LZ
 *Date: 2019.8.12
 *Modify: 2020.2.3
 *Email: 2117610943@qq.com
 */

#include "SPSBackgroundLayer.h"
#include "Zombies/CommonZombies.h"
#include "Zombies/ConeZombies.h"
#include "Zombies/BucketZombies.h"
#include "Zombies/CommonDoorZombies.h"
#include "Zombies/ConeDoorZombies.h"
#include "Zombies/BucketDoorZombies.h"
#include "Zombies/LmpZombies.h"
#include "Based/LevelData.h"

SPSBackgroundLayer::SPSBackgroundLayer() :
	_global(Global::getInstance()),
	_openLevelData(OpenLevelData::getInstance()),
	_previewZombiesTag(0)
{
}

SPSBackgroundLayer::~SPSBackgroundLayer()
{
}

bool SPSBackgroundLayer::init()
{
	if (!Layer::init()) return false;

	/* �������� */
	createBackGroundEffect();

	setBackgroundImagePosition();

	/* ����Ԥ����ʬ */
	createPreviewZombie();

	return true;
}

void SPSBackgroundLayer::createBackGroundEffect()
{
	switch (_global->userInformation->getSelectWorldName())
	{
	case WorldName::Mordern:
		createMordernBackgroundImage();
		break;
	case WorldName::DarkAges:
		createFireEffect(_global->userInformation->getBackground());
		break;
	case WorldName::NeonMixtapeTour:
		break;
	case WorldName::FrostibiteCaves:
		break;
	default:
		break;
	}
}

void SPSBackgroundLayer::createMordernBackgroundImage()
{
	switch (_global->userInformation->getCurrentPlayLevels())
	{
	case 35:
		_global->userInformation->setBackground(Sprite::create(_global->userInformation->getImagePath().find("background6")->second));
		_global->userInformation->getBackground()->addChild(ParticleSnow::create());
		break;
	case 36:
		_global->userInformation->setBackground(Sprite::create(_global->userInformation->getImagePath().find("background1")->second));
		break;
	case 37:
		_global->userInformation->setBackground(Sprite::create(_global->userInformation->getImagePath().find("background4")->second));
		break;
	case 50:
		_global->userInformation->setBackground(Sprite::create(_global->userInformation->getImagePath().find("background5")->second));
		break;
	case 51:
		_global->userInformation->setBackground(Sprite::create(_global->userInformation->getImagePath().find("background3")->second));
		break;
	case 52:
		_global->userInformation->setBackground(Sprite::create(_global->userInformation->getImagePath().find("background2")->second));
		break;
	default:
		_global->userInformation->setBackground(Sprite::create(_global->userInformation->getImagePath().find("background")->second));
		break;
	}
}

void SPSBackgroundLayer::createFireEffect(Sprite* name)
{
	auto fire = ParticleFire::create();
	fire->setPosition(Vec2(1900, 30));
	name->addChild(fire);

	auto fire1 = ParticleFire::create();
	fire1->setPosition(Vec2(1820, 1000));
	name->addChild(fire1);
}

void SPSBackgroundLayer::createPreviewZombie()
{
	srand(time(nullptr));
	switch (_global->userInformation->getSelectWorldName())
	{
	case WorldName::Mordern:
		createMordernPreviewZombies();
		break;
	case WorldName::DarkAges:
		break;
	case WorldName::NeonMixtapeTour:
		break;
	case WorldName::FrostibiteCaves:
		break;
	default:
		break;
	}

	sortZombiesPosition();
}

void SPSBackgroundLayer::createMordernPreviewZombies()
{
	int number = _openLevelData->readLevelData(_openLevelData->getLevelNumber())->getZombiesType().size();
	for (int i = 0; i < number; i++)
	{
		auto zombies = createDifferentZombies(i);
		zombies->setZombiePosition(Vec2(2050 + rand() % 450, rand() % 600 + 80));
		zombies->setZombieTag(_previewZombiesTag++);
		zombies->createPreviewZombie();
		zombies->setZombieAttributeForGameType();

		PreviewZombies previewzombies;
		previewzombies.animation = zombies->getZombieAnimation();
		previewzombies.position_y = previewzombies.animation->getPositionY();
		_previewZombies.push_back(previewzombies);
	}
}

void SPSBackgroundLayer::setBackgroundImagePosition()
{
	if (_global->userInformation->getBackground() != nullptr)
	{
		_global->userInformation->getBackground()->setAnchorPoint(Point(0, 0));
		_global->userInformation->getBackground()->setContentSize(Size(2930, 1081));
		this->addChild(_global->userInformation->getBackground(), 0, "previewBackgroundImage");
	}
}

void SPSBackgroundLayer::sortZombiesPosition()
{
	std::sort(_previewZombies.begin(), _previewZombies.end(), cmp);

	int i = 0;
	for (auto& pre : _previewZombies)
	{
		pre.animation->setLocalZOrder(110 + i++);
		pre.animation->setOpacity(255);
		pre.animation->getChildByName("shadow")->setOpacity(255);
	}
}

bool SPSBackgroundLayer::cmp(PreviewZombies& a, PreviewZombies& b)
{
	return a.position_y > b.position_y;
}

Zombies* SPSBackgroundLayer::createDifferentZombies(const int& id)
{
	Zombies* zombies;
	switch (static_cast<ZombiesType>(_openLevelData->readLevelData(_openLevelData->getLevelNumber())->getZombiesType().at(id)))
	{
	case ZombiesType::CommonZombies:      zombies = CommonZombies::create(this);      break;
	case ZombiesType::ConeZombies:        zombies = ConeZombies::create(this);        break;
	case ZombiesType::BucketZombies:      zombies = BucketZombies::create(this);      break;
	case ZombiesType::CommonDoorZombies:  zombies = CommonDoorZombies::create(this);  break;
	case ZombiesType::ConeDoorZombies:    zombies = ConeDoorZombies::create(this);    break;
	case ZombiesType::BucketDoorZombies:  zombies = BucketDoorZombies::create(this);  break;
	case ZombiesType::LmpZombies:         zombies = LmpZombies::create(this);         break;
	default: break;
	}
	return zombies;
}
