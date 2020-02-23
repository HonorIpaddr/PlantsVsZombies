/**
 *Copyright (c) 2020 LZ.All Right Reserved
 *Author : LZ
 *Date: 2020.1.28
 *Email: 2117610943@qq.com
 */

#include "GSControlLayer.h"
#include "GSDefine.h"
#include "GSData.h"
#include "GSGameResultJudgement.h"
#include "GSGameEndLayer.h"
#include "GSButtonLayer.h"
#include "GSInformationLayer.h"
#include "GSBackgroundLayer.h"
#include "GSAnimationLayer.h"

#include "Plants/Plants.h"
#include "Plants/Plants-Files.h"
#include "Zombies/Zombies.h"
#include "Based/LevelData.h"
#include "Based/GameType.h"
#include "Scenes/EasterEggsScene/GameEasterEggs.h"

GameMapInformation::GameMapInformation():
   rowNumbers(5)
,  columnNumbers(9)
{
	MAP_INIT(plantsMap);
	MAP_CAN_NOT_PLANT(plantsMap);
}

GSControlLayer::GSControlLayer():
	_global(Global::getInstance())
,   _openLevelData(OpenLevelData::getInstance())
,	_gameMapInformation(new GameMapInformation)
,   _selectPlantsTag(PlantsType::None)
,   _plantPreviewImage(nullptr)
,   _animationLayer(nullptr)
,   _gameEndShieldLayer(nullptr)
,   _zombiesAppearControl(new ZombiesAppearControl)
{
	srand(time(nullptr));
	_levelData = _openLevelData->readLevelData(_openLevelData->getLevelNumber())->getMunchZombiesFrequency();
}

GSControlLayer::~GSControlLayer()
{
	delete _gameMapInformation;
	delete _zombiesAppearControl;
}

bool GSControlLayer::init()
{
	if(!Layer::init())return false;

	createSchedule();
	createZombiesHowlSoundEffect();
	createPlantsCardListener();
	createMouseListener();

	return true;
}

void GSControlLayer::createSchedule()
{
	schedule([&](float){
			controlCardEnabled();
			calculatePlantPosition();
			createZombies();
			controlRefurbishMusicAndText();
			judgeLevelIsFinished();
		}, 0.1f, "mainUpdate");

	schedule([&](float) {
		zombiesComeTiming();
		}, 1.0f, "zombiesComing");
}

void GSControlLayer::createZombiesHowlSoundEffect()
{
	this->runAction(RepeatForever::create(Sequence::create(DelayTime::create(1.0f),
		CallFunc::create([=]()
			{
				if (rand() % 10000 <= 700)
				{
					const string music[6] = { "groan","groan2","groan3","groan4","groan5","groan6" };
					AudioEngine::setVolume(AudioEngine::play2d(_global->userInformation->getMusicPath().find(music[rand() % 6])->second), _global->userInformation->getSoundEffectVolume());
				}
			}), nullptr)));
}

void GSControlLayer::controlCardEnabled()
{
	for (auto& card : _global->userInformation->getUserSelectCrads())
	{
		/* ���ֲ����������������ӵ�������� */
		if (buttonLayerInformation->plantsCards[card.cardTag].plantsNeedSunNumbers > _global->userInformation->getSunNumbers())
		{
			buttonLayerInformation->plantsCards[card.cardTag].plantsCardText->setColor(Color3B::RED);
		}
		else
		{
			buttonLayerInformation->plantsCards[card.cardTag].plantsCardText->setColor(Color3B::BLACK);
		}
		/* ���������䵹��ʱ��� */
		if (buttonLayerInformation->plantsCards[card.cardTag].timeBarIsFinished)
		{
			buttonLayerInformation->plantsCards[card.cardTag].plantsCards->setEnabled(true);
			/* ���ֲ������������С�������� */
			if (buttonLayerInformation->plantsCards[card.cardTag].plantsNeedSunNumbers <= _global->userInformation->getSunNumbers())
			{
				buttonLayerInformation->plantsCards[card.cardTag].plantsCards->setColor(Color3B::WHITE);
			}
			else
			{
				buttonLayerInformation->plantsCards[card.cardTag].plantsCards->setColor(Color3B::GRAY);
			}
		}
	}
}

void GSControlLayer::calculatePlantPosition()
{
	for (int i = 0; i < _gameMapInformation->rowNumbers; i++)
	{
		for (int j = 0; j < _gameMapInformation->columnNumbers; j++)
		{
			if (GRASS_INSIDE(_cur, i, j))
			{
				_plantsPosition.x = j;
				_plantsPosition.y = i;
			}
		}
	}

	/* ������ڷ�Χ�ڣ��Ƴ��������� */
	if (GRASS_OUTSIDE(_cur))
	{
		_plantsPosition.x = -100;
		_plantsPosition.y = -100;
	}
}

void GSControlLayer::createMouseListener()
{
	/* ���������� */
	auto Mouse = EventListenerMouse::create();

	/* ����ƶ� */
	Mouse->onMouseMove = [&](Event* event)
	{
		/* ��ȡ���λ�� */
		_cur = ((EventMouse*)event)->getLocationInView();

		mouseMoveControl();
	};

	/* ��갴�� */
	Mouse->onMouseDown = [&](Event* event)
	{
		mouseDownControl((EventMouse*)event);
	};

	_director->getEventDispatcher()->addEventListenerWithSceneGraphPriority(Mouse, this);
}

void GSControlLayer::createPlantsCardListener()
{
	for (auto& card : _global->userInformation->getUserSelectCrads())
	{
		buttonLayerInformation->plantsCards[card.cardTag].plantsCards->addTouchEventListener([&, card](Ref* sender, ui::Widget::TouchEventType type)
			{
				switch (type)
				{
				case ui::Widget::TouchEventType::ENDED:
					if (!buttonLayerInformation->mouseSelectImage->isSelectPlants)
					{
						_selectPlantsTag = static_cast<PlantsType>(card.cardTag);
					}
					selectPlantsPreviewImage();
					break;
				}
			});
	}
}

void GSControlLayer::selectPlantsPreviewImage()
{
	switch (buttonLayerInformation->mouseSelectImage->isSelectPlants)
	{
	case true:
		AudioEngine::setVolume(AudioEngine::play2d(_global->userInformation->getMusicPath().find("tap2")->second), _global->userInformation->getSoundEffectVolume());
		buttonLayerInformation->plantsCards[static_cast<int>(_selectPlantsTag)].progressTimer->setPercentage(0);
		buttonLayerInformation->plantsCards[static_cast<int>(_selectPlantsTag)].plantsCards->setColor(Color3B::WHITE);

		/* ��������������������� */
		_global->userInformation->setSunNumbers(_global->userInformation->getSunNumbers() + buttonLayerInformation->plantsCards[static_cast<int>(buttonLayerInformation->mouseSelectImage->selectPlantsId)].plantsNeedSunNumbers);
		informationLayerInformation->updateSunNumbers();

		removePreviewPlant();

		/* ֲ��Ҫ����� */
		backgroundLayerInformation->gameType->updateRequirementNumbers("ֲ����������");
		break;
	case false:
		/* ���ֲ�������������ӵ�����⣬�򷢳����� */
		if (buttonLayerInformation->plantsCards[static_cast<int>(_selectPlantsTag)].plantsNeedSunNumbers > _global->userInformation->getSunNumbers())
		{
			AudioEngine::setVolume(AudioEngine::play2d(_global->userInformation->getMusicPath().find("buzzer")->second), _global->userInformation->getSoundEffectVolume());
			informationLayerInformation->sunNumberTextWarning();
		}
		/* ʣ��ֲ������С�ڵ���0 */
		else if (backgroundLayerInformation->gameType->getPlantsRequriement()->isHavePlantsRequriement && backgroundLayerInformation->gameType->getPlantsRequriement()->surPlusPlantsNumbers <= 0)
		{
			AudioEngine::setVolume(AudioEngine::play2d(_global->userInformation->getMusicPath().find("buzzer")->second), _global->userInformation->getSoundEffectVolume());
			backgroundLayerInformation->gameType->waringPlantsNull();
		}
		else
		{
			AudioEngine::setVolume(AudioEngine::play2d(_global->userInformation->getMusicPath().find("seedlift")->second), _global->userInformation->getSoundEffectVolume());

			/* ��ȥ����������������� */
			_global->userInformation->setSunNumbers(_global->userInformation->getSunNumbers() - buttonLayerInformation->plantsCards[static_cast<int>(_selectPlantsTag)].plantsNeedSunNumbers);
			informationLayerInformation->updateSunNumbers();

			/* ��Ƭ��� */
			buttonLayerInformation->plantsCards[static_cast<int>(_selectPlantsTag)].plantsCards->setColor(Color3B::GRAY);
			buttonLayerInformation->plantsCards[static_cast<int>(_selectPlantsTag)].progressTimer->setPercentage(100);

			/* ���ѡ���� */
			buttonLayerInformation->mouseSelectImage->isSelectPlants = true;
			buttonLayerInformation->mouseSelectImage->selectPlantsId = _selectPlantsTag;

			createPreviewPlants();

			/* ֲ��Ҫ����� */
			backgroundLayerInformation->gameType->updateRequirementNumbers("ֲ����������");
		}
		break;
	}
}

void GSControlLayer::createPreviewPlants()
{
	CURSOR_VISIBLE(false)

	Plants* previewPlants, * curPlants;/* Ԥ��ֲ�� */
	switch (_selectPlantsTag)
	{
	case PlantsType::SunFlower:        previewPlants = SunFlower::create(this);       curPlants = SunFlower::create(this);       break;
	case PlantsType::PeaShooter:       previewPlants = PeaShooter::create(this);      curPlants = PeaShooter::create(this);      break;
	case PlantsType::WallNut:          previewPlants = WallNut::create(this);         curPlants = WallNut::create(this);         break;
	case PlantsType::CherryBomb:       previewPlants = CherryBomb::create(this);      curPlants = CherryBomb::create(this);      break;
	case PlantsType::PotatoMine:       previewPlants = PotatoMine::create(this);      curPlants = PotatoMine::create(this);      break;
	case PlantsType::CabbagePult:      previewPlants = CabbagePult::create(this);     curPlants = CabbagePult::create(this);     break;
	case PlantsType::Torchwood:        previewPlants = Torchwood::create(this);       curPlants = Torchwood::create(this);       break;
	case PlantsType::Spikeweed:        previewPlants = Spikeweed::create(this);       curPlants = Spikeweed::create(this);       break;
	case PlantsType::Garlic:           previewPlants = Garlic::create(this);          curPlants = Garlic::create(this);          break;
	case PlantsType::FirePeaShooter:   previewPlants = FirePeaShooter::create(this);  curPlants = FirePeaShooter::create(this);  break;
	case PlantsType::Jalapeno:         previewPlants = Jalapeno::create(this);        curPlants = Jalapeno::create(this);        break;
	default: break;
	}
	
	_plantPreviewImage = previewPlants->createPlantImage();

	_plantCurImage = curPlants->createPlantImage();
	_plantCurImage->setOpacity(255);
	_plantCurImage->setPosition(_cur);
	_plantCurImage->setGlobalZOrder(10);
}

void GSControlLayer::zombiesComeTiming()
{
	if (!_zombiesAppearControl->getIsBegin())
	{
		_zombiesAppearControl->setTimeClear();
		_zombiesAppearControl->setIsBegin(true);
	}

	/* ��ʱ */
	_zombiesAppearControl->setTimeAdd();
}

void GSControlLayer::createZombies()
{
	/* ˢ�½�ʬ */
	if (_zombiesAppearControl->getLastFrequencyZombiesWasDeath())
	{
		_zombiesAppearControl->setLastFrequencyZombiesWasDeath(false);
		if (_zombiesAppearControl->getZombiesAppearFrequency() < _openLevelData->readLevelData(_openLevelData->getLevelNumber())->getZombiesFrequency())
		{
			int zombiesNumbers = _zombiesAppearControl->getZombiesNumbersForAppearFrequency(_zombiesAppearControl->getZombiesAppearFrequency());
			for (int i = 0; i < zombiesNumbers; ++i)
			{
				animationLayerInformation->createZombies();
			}
			/* ������������һ */
			_zombiesAppearControl->setZombiesAppearFrequency();

			/* ���������� */
			informationLayerInformation->updateProgressBar(_zombiesAppearControl->getZombiesAppearFrequency());
		}
	}
	informationLayerInformation->updateProgressBarHead();
	
	/* ���ƽ�ʬ��ˢ�� */
	if (controlRefurbishZombies())
	{
		if (_zombiesAppearControl->getZombiesAppearFrequency())
		{
			_zombiesAppearControl->setTimeClear(); /* ������һ��ˢ��ʱ������ */
		}
		_zombiesAppearControl->setLastFrequencyZombiesWasDeath(true);
		_zombiesAppearControl->setIsBegin(false);
		UserDefault::getInstance()->flush();
	}
}

bool GSControlLayer::controlRefurbishZombies()
{
	if ((_zombiesAppearControl->getTime() >= _openLevelData->readLevelData(_openLevelData->getLevelNumber())->getFirstFrequencyTime()&&
		_zombiesAppearControl->getZombiesAppearFrequency() == 0)                   /* ��һ��ˢ�¿��� */

		|| (_zombiesAppearControl->getTime() >= 32 + rand() % 10 &&
		   (_zombiesAppearControl->getZombiesAppearFrequency() == 1 
	    || _zombiesAppearControl->getZombiesAppearFrequency() == 2))               /* �ڶ�����ˢ�¿��� */

		|| _zombiesAppearControl->getTime() >= 45                                  /* �������45��ˢ����һ�� */

		|| (Zombies::getZombiesNumbers() <= 4 &&
			_zombiesAppearControl->getZombiesAppearFrequency() > 3)                /* ������ŵĽ�ʬ��С�ڹ涨��ˢ����һ�� */

		|| (Zombies::getZombiesNumbers() <= 0 &&                                   /* ���û�д�ʬ������ˢ�½�ʬ */
			_zombiesAppearControl->getZombiesAppearFrequency() != 0)
	   )
	{
		return true;
	}
	return false;
}

void GSControlLayer::controlRefurbishMusicAndText()
{
	/* ���ƴ󲨽�ʬ��Ϯ������������ */
	auto level = _openLevelData->readLevelData(_openLevelData->getLevelNumber());
	if (_zombiesAppearControl->getTime() >= level->getFirstFrequencyTime() && _zombiesAppearControl->getZombiesAppearFrequency() == 0)
	{
		AudioEngine::setVolume(AudioEngine::play2d(_global->userInformation->getMusicPath().find("awooga")->second), _global->userInformation->getSoundEffectVolume());
	}

	/* ���һ����ʬ��������ʾ�����Ӹ��� */
	if (_zombiesAppearControl->getZombiesAppearFrequency() == level->getZombiesFrequency() && !_zombiesAppearControl->getIsShowWords())
	{
		if (informationLayerInformation->updateProgressBarFlag())
		{
			_zombiesAppearControl->setIsShowWords(true);
		}
	}

	/* ǰ����������ʾ�����Ӹ��� */
	for (auto data = _levelData.begin(); data != _levelData.end();)
	{
		if (_zombiesAppearControl->getZombiesAppearFrequency() == *data)
		{
			if (informationLayerInformation->updateProgressBarFlag(-1) && informationLayerInformation->updateProgressBarFlag(*data))
			{
				data = _levelData.erase(data);
			}
			else
			{
				++data;
			}
		}
		else 
		{
			++data;
		}
	}
}

bool GSControlLayer::judgeMousePositionIsInMap()
{
	return (_plantsPosition.x >= 0 && _plantsPosition.x <= 8 && _plantsPosition.y >= 0 && _plantsPosition.y <= 4) ? true : false;
}

bool GSControlLayer::judgeMousePositionIsCanPlant()
{
	return (_gameMapInformation->plantsMap[static_cast<int>(_plantsPosition.y)][static_cast<int>(_plantsPosition.x)] != CAN_NOT_PLANT /* �����ڲ�����ֲ ��������ֲ�ķ�Χ���ڣ�*/
		&& _gameMapInformation->plantsMap[static_cast<int>(_plantsPosition.y)][static_cast<int>(_plantsPosition.x)] == NO_PLANTS)     /* ����ֲ�ķ�Χ�ڻ�û����ֲ */
		? true : false;
}

bool GSControlLayer::judgeMousePositionHavePlant()
{
	return (_gameMapInformation->plantsMap[static_cast<int>(_plantsPosition.y)][static_cast<int>(_plantsPosition.x)] != CAN_NOT_PLANT
		&& _gameMapInformation->plantsMap[static_cast<int>(_plantsPosition.y)][static_cast<int>(_plantsPosition.x)] != NO_PLANTS)
		? true : false;
}

void GSControlLayer::removePreviewPlant()
{
	/* �Ƴ�Ԥ��ֲ�� */
	_plantPreviewImage->removeFromParent();
	_plantCurImage->removeFromParent();
	buttonLayerInformation->mouseSelectImage->isSelectPlants = false;
	CURSOR_VISIBLE(true)
}

void GSControlLayer::removeShovel()
{
	buttonLayerInformation->mouseSelectImage->isSelectShovel = false;
	_director->getOpenGLView()->setCursor(_global->userInformation->getImagePath().find("cursor")->second, Point::ANCHOR_TOP_LEFT);
}

void GSControlLayer::recoveryPlantsColor()
{
	for (int i = 0; i < _gameMapInformation->rowNumbers; i++)
	{
		for (int j = 0; j < _gameMapInformation->columnNumbers; j++)
		{
			if (_gameMapInformation->plantsMap[i][j] != CAN_NOT_PLANT && _gameMapInformation->plantsMap[i][j] != NO_PLANTS)
			{
				auto plant = _animationLayer->getChildByTag(SET_TAG(Vec2(j, i)));
				if (plant)
				{
					plant->setColor(Color3B::WHITE);
				}
			}
		}
	}
}

void GSControlLayer::judgeLevelIsFinished()
{
	/* �ؿ����� */
	if (ZombiesGroup.size() <= 0 && _zombiesAppearControl->getZombiesAppearFrequency() >= _openLevelData->readLevelData(_openLevelData->getLevelNumber())->getZombiesFrequency())
	{
		CURSOR_VISIBLE(true)

		setGameEnd();

		auto judgeUserWin = new GSGameResultJudgement();
		if (judgeUserWin->judgeUserIsWin() == GameTypes::None)
		{
			if (_global->userInformation->getCurrentPlayLevels() >= 52)
			{
				_director->getInstance()->pushScene(TransitionFade::create(0.5f, GameEasterEggs::createScene()));
			}
			_gameEndShieldLayer->successfullEntry();
		}
		else
		{
			_gameEndShieldLayer->breakThrough(judgeUserWin->judgeUserIsWin()); /* ����ʧ�� */
		}
	}
}

void GSControlLayer::setGameEnd()
{
	unschedule("mainUpdate");
	unschedule("zombiesComing");
	Plants::stopPlantsAllAction();
	_director->getScheduler()->setTimeScale(1.0f);

	_gameEndShieldLayer = GSGameEndLayer::create();
	_director->getRunningScene()->addChild(_gameEndShieldLayer, 10);
}

void GSControlLayer::mouseMoveControl()
{
	/* ������ѡ����ֲ�� */
	if (buttonLayerInformation->mouseSelectImage->isSelectPlants)
	{
		if (_gameMapInformation->plantsMap[static_cast<int>(_plantsPosition.y)][static_cast<int>(_plantsPosition.x)] != NO_PLANTS)
		{
			_plantPreviewImage->setPosition(Vec2(-1000, -1000));
		}
		else
		{
			_plantPreviewImage->setPosition(Vec2(GRASS_POSITION_LEFT + 122 * _plantsPosition.x + 60, 110 + 138 * (_plantsPosition.y + 1) - 60));
		}
		_plantCurImage->setPosition(_cur);
	}

	/* ������в��� */
	if (buttonLayerInformation->mouseSelectImage->isSelectShovel)
	{
		if (!_animationLayer)
		{
			_animationLayer = _director->getRunningScene()->getChildByName("animationLayer");
		}

		/* ѭ����ֲ��ָ���ԭ������ɫ */
		recoveryPlantsColor();

		if (judgeMousePositionIsInMap() && judgeMousePositionHavePlant())  /* ����ڵ�ͼ��Χ�� && ����ֲ�� */
		{
			auto plant = _animationLayer->getChildByTag(SET_TAG(_plantsPosition));
			if (plant)
			{
				plant->setColor(Color3B(100, 100, 100));
			}
		}
	}
}

void GSControlLayer::mouseDownControl(EventMouse* eventmouse)
{
	if (eventmouse->getMouseButton() == EventMouse::MouseButton::BUTTON_RIGHT) /* ������Ҽ����� */
	{	
		if (buttonLayerInformation->mouseSelectImage->isSelectPlants)/* �������ֲ�� */
		{
			AudioEngine::setVolume(AudioEngine::play2d(_global->userInformation->getMusicPath().find("tap2")->second), _global->userInformation->getSoundEffectVolume());
			buttonLayerInformation->plantsCards[static_cast<int>(buttonLayerInformation->mouseSelectImage->selectPlantsId)].progressTimer->setPercentage(0);
			buttonLayerInformation->plantsCards[static_cast<int>(buttonLayerInformation->mouseSelectImage->selectPlantsId)].plantsCards->setColor(Color3B::WHITE);

			/* ��������������������� */
			_global->userInformation->setSunNumbers(_global->userInformation->getSunNumbers() + buttonLayerInformation->plantsInformation->PlantsNeedSunNumbers[static_cast<int>(buttonLayerInformation->mouseSelectImage->selectPlantsId)]);
			informationLayerInformation->updateSunNumbers();

			/* ֲ��Ҫ����� */
			backgroundLayerInformation->gameType->updateRequirementNumbers("ֲ����������");

			removePreviewPlant();
		}

		if (buttonLayerInformation->mouseSelectImage->isSelectShovel) /* ������в��� */
		{
			removeShovel();
			recoveryPlantsColor();
		}
	}

	if (eventmouse->getMouseButton() == EventMouse::MouseButton::BUTTON_LEFT) /* ������������ */
	{
		if (buttonLayerInformation->mouseSelectImage->isSelectPlants)
		{
			if (judgeMousePositionIsInMap() && judgeMousePositionIsCanPlant()) /* ����ڵ�ͼ��Χ�� && ������ֲֲ�� */
			{
				/* ��¼ʹ��ֲ������ */
				UserDefault::getInstance()->setIntegerForKey("USEPLANTSNUMBERS", ++_global->userInformation->getUsePlantsNumbers());

				/* ��ֲֲ�� */
				animationLayerInformation->plantPlants();

				/* ��ͼ��¼��ֲ��ֲ�� */
				_gameMapInformation->plantsMap[static_cast<int>(_plantsPosition.y)][static_cast<int>(_plantsPosition.x)] = static_cast<int>(buttonLayerInformation->mouseSelectImage->selectPlantsId);

				/* ���õ���ʱ���Ұ�ť������ */
				int plantsTag = static_cast<int>(buttonLayerInformation->mouseSelectImage->selectPlantsId);
				buttonLayerInformation->plantsCards[static_cast<int>(buttonLayerInformation->mouseSelectImage->selectPlantsId)].timeBarIsFinished = false;
				buttonLayerInformation->plantsCards[static_cast<int>(buttonLayerInformation->mouseSelectImage->selectPlantsId)].plantsCards->setEnabled(false);
				buttonLayerInformation->plantsCards[static_cast<int>(buttonLayerInformation->mouseSelectImage->selectPlantsId)].plantsCards->setColor(Color3B::GRAY);
				buttonLayerInformation->plantsCards[static_cast<int>(buttonLayerInformation->mouseSelectImage->selectPlantsId)].progressTimer->runAction(
					Sequence::create(ProgressFromTo::create(buttonLayerInformation->plantsInformation->PlantsCoolTime[static_cast<int>(buttonLayerInformation->mouseSelectImage->selectPlantsId)], 100, 0),
						CallFunc::create([=]() { buttonLayerInformation->plantsCards[plantsTag].timeBarIsFinished = true; }), nullptr)
				);
				removePreviewPlant();
			}
			else
			{
				if (_cur.x > CARD_BAR_RIGHT)
				{
					AudioEngine::setVolume(AudioEngine::play2d(_global->userInformation->getMusicPath().find("buzzer")->second), _global->userInformation->getSoundEffectVolume());
					/* ������ɫ�ָ� */
					buttonLayerInformation->plantsCards[static_cast<int>(buttonLayerInformation->mouseSelectImage->selectPlantsId)].progressTimer->setPercentage(0);
					buttonLayerInformation->plantsCards[static_cast<int>(buttonLayerInformation->mouseSelectImage->selectPlantsId)].plantsCards->setColor(Color3B::WHITE);

					/* ��ʾ��Ϣ */
					informationLayerInformation->createPromptText();

					removePreviewPlant();
				}
			}
		}
		if (buttonLayerInformation->mouseSelectImage->isSelectShovel) /* ������в��� */
		{
			AudioEngine::setVolume(AudioEngine::play2d(_global->userInformation->getMusicPath().find("plant2")->second), _global->userInformation->getSoundEffectVolume());
			if (judgeMousePositionIsInMap() && judgeMousePositionHavePlant())    /* ����ڵ�ͼ��Χ�� && ����ֲ�� */
			{
				animationLayerInformation->deletePlants();/* ����ֲ�� */
			}
			removeShovel();
		}
	}
}


ZombiesAppearControl::ZombiesAppearControl():
	_zombiesAppearFrequency(0)
,	_lastFrequencyZombiesWasDeath(false)
,	_isBegin(false)
,	_isShowWords(false)
,	_time(0)
,   _openLevelData(OpenLevelData::getInstance())
{
	_random.seed(time(nullptr));
}

ZombiesAppearControl::~ZombiesAppearControl()
{
}

ZombiesType ZombiesAppearControl::createDifferentTypeZombies(const int& zombiesAppearFrequency)
{
	int number = rand() % 100;
	int sum = 0, i = -1;
	auto zombiesTypeProbability = _openLevelData->readLevelData(_openLevelData->getLevelNumber())->getZombiesTypeProbabilityFrequency().at(zombiesAppearFrequency);
	for (auto& probability : zombiesTypeProbability)
	{
		++i;
		sum += probability;
		if (number < sum)
		{
			return static_cast<ZombiesType>(_openLevelData->readLevelData(_openLevelData->getLevelNumber())->getZombiesType().at(i));
		}
	}
	return ZombiesType::None;
}

int ZombiesAppearControl::getZombiesNumbersForAppearFrequency(const int& ZombiesAppearFrequency)
{
	const int number = _openLevelData->readLevelData(_openLevelData->getLevelNumber())->getZombiesNumbers().at(ZombiesAppearFrequency);
	return number + rand() % number;
}

int ZombiesAppearControl::getEqualProbabilityForRow()
{
	uniform_int_distribution<unsigned>number(0, 4);

	if (_judgeZombieRow.size() == 5)
	{
		_judgeZombieRow.clear();
	}

	int row;
	do
	{
		row = number(_random);
	} while (_judgeZombieRow.count(row));

	_judgeZombieRow.insert(row);
	
	return _zombiesPosition[row];
}