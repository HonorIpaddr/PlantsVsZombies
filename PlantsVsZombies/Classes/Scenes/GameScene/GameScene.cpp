/**
 *Copyright (c) 2020 LZ.All Right Reserved
 *Author : LZ
 *Date: 2020.1.28
 *Email: 2117610943@qq.com
 */

#include "GameScene.h"
#include "GSBackgroundLayer.h"
#include "GSButtonLayer.h"
#include "GSInformationLayer.h"
#include "GSControlLayer.h"
#include "GSAnimationLayer.h"
#include "GSDefine.h"
#include "GSData.h"

#include "Plants/DefensePlants/SunFlower.h"
#include "Plants/EmissionPlants/Bullet/Bullet.h"
#include "Based/Car.h"

GameScene::GameScene() :
  _global(Global::getInstance())
, _director(Director::getInstance())
{
}

GameScene::~GameScene()
{
	DELETE_MAP_GROUP(PlantsGroup);
	DELETE_LIST_GROUP(ZombiesGroup);
	DELETE_LIST_GROUP(SunsGroup);
	DELETE_LIST_GROUP(BulletGroup);
	DELETE_LIST_GROUP(CarsGroup);

	Zombies::setZombiesNumbers(0);
	Zombies::zombiesWinOrLoseInit();
}

Scene* GameScene::createScene()
{
	return GameScene::create();
}

bool GameScene::init()
{
	if(!Scene::init())return false;

	controlPlayMusic();
	backgroundLayer();   // ������
	informationLayer();  // ��Ϣ��
	buttonLayer();       // ��ť��
	controlLayer();      // ���Ʋ�
	animationLayer();    // ������

	return true;
}

void GameScene::controlPlayMusic()
{
	switch (_global->userInformation->getCurrentPlayLevels())
	{
	case 35: _global->changeBgMusic("mainmusic.mo3.2", true);  break;
	case 36: _global->changeBgMusic("mainmusic.mo3.3", true);  break;
	case 37: _global->changeBgMusic("mainmusic.mo3.7", true);  break;
	case 50: _global->changeBgMusic("mainmusic.mo3.10", true); break;
	case 51: _global->changeBgMusic("mainmusic.mo3.11", true); break;
	case 52: _global->changeBgMusic("mainmusic.mo3.12", true); break;
	default: _global->changeBgMusic("KitanaiSekai", true);     break;
	}
}

void GameScene::backgroundLayer()
{
	backgroundLayerInformation = GSBackgroundLayer::create();
	backgroundLayerInformation->addLayer(this, 0, "backgroundLayer");
}

void GameScene::controlLayer()
{
	controlLayerInformation = GSControlLayer::create();
	controlLayerInformation->addLayer(this, 1, "controlLayer");
}

void GameScene::animationLayer()
{
	animationLayerInformation = GSAnimationLayer::create(this);
	animationLayerInformation->addLayer(this, 2, "animationLayer");
}

void GameScene::informationLayer()
{
	informationLayerInformation = GSInformationLayer::create();
	informationLayerInformation->addLayer(this, 3, "informationLayer");
}

void GameScene::buttonLayer()
{
	buttonLayerInformation = GSButtonLayer::create();
	buttonLayerInformation->addLayer(this, 4, "buttonLayer");
}