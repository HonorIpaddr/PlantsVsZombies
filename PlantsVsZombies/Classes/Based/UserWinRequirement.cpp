#include "Based/LevelData.h"
#include "AudioEngine.h"
#include "Based/UserWinRequirement.h"
#include "Scenes/WorldScene/World_1.h"
#include "Based/GameType.h"

UserWinRequirement::UserWinRequirement(Node* node):
	_node(node),
	_layer(nullptr),
	_listener(nullptr),
	_shieldListener(nullptr),
	_levelObjiectives(nullptr),
	_global(Global::getInstance())
{
}

UserWinRequirement::~UserWinRequirement()
{
}

void UserWinRequirement::createShieldLayer()
{
	_layer = LayerColor::create(Color4B(0, 0, 0, 180));
	_layer->setGlobalZOrder(10);
	_layer->setName("_layer");
	_node->addChild(_layer);

	// set shieldLayer
	_shieldListener = EventListenerTouchOneByOne::create();
	_shieldListener->onTouchBegan = [](Touch* touch, Event* event)-> bool { return true; };
	_shieldListener->setSwallowTouches(true);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(_shieldListener, _layer);
}

void UserWinRequirement::createDialogBox(GameTypes finishedid)
{
	createShieldLayer();

	_levelObjiectives = Scale9Sprite::create(_global->userInformation->getImagePath().find("LevelObjiectives")->second);
	_levelObjiectives->setPosition(Director::getInstance()->getWinSize() / 2.0f);
	_levelObjiectives->setGlobalZOrder(10);
	_layer->addChild(_levelObjiectives);

	auto LevelObjiectivesText = Text::create();
	LevelObjiectivesText->setString(_global->userInformation->getGameText().find("ͨ��Ҫ��")->second);
	LevelObjiectivesText->setFontName("resources/fonts/fzse_gbk.ttf");
	LevelObjiectivesText->setFontSize(50);
	LevelObjiectivesText->setColor(Color3B(0, 255, 255));
	LevelObjiectivesText->setPosition(Vec2(_levelObjiectives->getContentSize().width / 2, 490));
	LevelObjiectivesText->setGlobalZOrder(10);
	_levelObjiectives->addChild(LevelObjiectivesText);

	/* ��ʾҪ�� */
	showRequirement(finishedid);

	createListener();
}

void UserWinRequirement::setParent(Node* node)
{
	_node = node;
}

void UserWinRequirement::setButtonVisible(bool visible)
{
	_continuesGame->setVisible(visible); 
}

void UserWinRequirement::setDialogOpacity(const int opacity)
{
	_levelObjiectives->setOpacity(opacity);
}

void UserWinRequirement::setListenerEnable(bool enable)
{
	_listener->setEnabled(enable);
}

void UserWinRequirement::setShowDialogAction()
{
	if (_levelObjiectives)
	{
		_levelObjiectives->setPosition(Vec2(Director::getInstance()->getWinSize().width / 2.0f, 3000));
		_levelObjiectives->runAction(EaseBounceOut::create(MoveTo::create(0.5f, Director::getInstance()->getWinSize() / 2.0f)));
	}
	if (_layer)
	{
		_layer->setOpacity(0);
		_layer->runAction(FadeTo::create(0.5f, 180));
	}
}

void UserWinRequirement::setDelectDialogAction()
{
	if (_levelObjiectives)
	{
		_levelObjiectives->runAction(MoveBy::create(0.2f, Vec2(0, -500)));
	}
	if (_layer)
	{
		_layer->runAction(Sequence::create(FadeOut::create(0.2f), CallFunc::create([&]() {deleteDialog(); }), nullptr));
	}
}

void UserWinRequirement::setLayerColorOpacity(Color3B& color, GLubyte& opacity)
{
	_layer->setColor(color);
	_layer->setOpacity(opacity);
}

void UserWinRequirement::deleteDialog()
{
	_layer->removeFromParent();
}

Sprite* UserWinRequirement::getDialog() const
{
	return _levelObjiectives;
}

void UserWinRequirement::showRequirement(GameTypes finishedid)
{
	auto leveldata = OpenLevelData::getInstance()->readLevelData(OpenLevelData::getInstance()->getLevelNumber());
	for (unsigned int i = 0; i < leveldata->getGameType().size(); i++)
	{
		switch (static_cast<GameTypes>(leveldata->getGameType().at(i)))
		{
		case GameTypes::CreateWall:
			showText(_global->userInformation->getGameText().find("������ķ��ߣ���ֹ��ʬ�Ľ�����")->second, i);
			break;
		case GameTypes::AtLeastSunNumbers:
		{
			char buff[128];
			sprintf(buff, _global->userInformation->getGameText().find("���ٲ��� %d �����⣡")->second.c_str(), leveldata->getAtLeastSunNumbers());
			finishedid == GameTypes::AtLeastSunNumbers ? showText(buff, i, Color3B::RED) : showText(buff, i);
		}
			break;
		case GameTypes::FlowerPosition:
			finishedid == GameTypes::FlowerPosition ? showText(_global->userInformation->getGameText().find("��ʬ�Ȼ�����Ļ�̳��")->second, i, Color3B::RED) :
				showText(_global->userInformation->getGameText().find("��Ҫ�ý�ʬ�Ȼ���Ļ�̳��")->second, i);
			break;
		case GameTypes::CarNumbers:
		{
			char buff[128];
			sprintf(buff, _global->userInformation->getGameText().find("��ʬ�������������ٴ��� %d ��С����")->second.c_str(), leveldata->getCarNumbers());
			finishedid == GameTypes::CarNumbers ? showText(buff, i, Color3B::RED) : showText(buff, i);
		}
			break;
		case GameTypes::UserPlantsNumbers:
		{
			char buff[128];
			sprintf(buff, _global->userInformation->getGameText().find("���ʹ�� %d ��ֲ����������ķ��ߣ�")->second.c_str(), leveldata->getUsePlantsNumbers());
			showText(buff, i);
		}
			break;
		case GameTypes::ZombiesInvisible:
			showText(_global->userInformation->getGameText().find("��ֹ���εĽ�ʬ�Ľ�����")->second, i);
			break;
		case GameTypes::SmallZombies:
			showText(_global->userInformation->getGameText().find("С��ʬ���鷳��")->second, i);
			break;
		case GameTypes::BigZombies:
			showText(_global->userInformation->getGameText().find("�������˽�ʬ�Ľ�����")->second, i);
			break;
		case GameTypes::NoPlants:
			showText(_global->userInformation->getGameText().find("���ֲ����ֲ�ں��ʵĵط���")->second, i);
			break;
		default:
			break;
		}
	}
}

void UserWinRequirement::showText(const string& text, const int& ID, Color3B color)
{
	auto requiretext = Label::createWithTTF(text, "resources/fonts/GameFont.ttf", 40);
	requiretext->setColor(Color3B::BLACK);
	requiretext->setLineBreakWithoutSpace(true);
	requiretext->setWidth(790);
	requiretext->setPosition(_textPosition[OpenLevelData::getInstance()->readLevelData(OpenLevelData::getInstance()->getLevelNumber())->getGameType().size() - 1][ID]);
	requiretext->setAnchorPoint(Vec2(0, 1));
	requiretext->setGlobalZOrder(10);
	requiretext->setColor(color);
	_levelObjiectives->addChild(requiretext);

	auto LevelObjiectives2 = Sprite::create(_global->userInformation->getImagePath().find("LevelObjiectives2")->second);
	LevelObjiectives2->setPosition(_textPosition[OpenLevelData::getInstance()->readLevelData(OpenLevelData::getInstance()->getLevelNumber())->getGameType().size() - 1][ID] + Vec2(-90, +13));
	LevelObjiectives2->setAnchorPoint(Vec2(0, 1));
	LevelObjiectives2->setGlobalZOrder(10);
	_levelObjiectives->addChild(LevelObjiectives2);
}

void UserWinRequirement::createListener()
{
	/* ������������ */
	_listener = EventListenerTouchOneByOne::create();
	_listener->onTouchBegan = [=](Touch* t, Event* e)
	{
		if (_levelObjiectives->getBoundingBox().containsPoint(t->getLocation()))
		{
			_phasePosition = t->getLocation() - _levelObjiectives->getPosition();
			return true;
		}
		else return false;
	};
	_listener->onTouchMoved = [=](Touch* t, Event* e)
	{
		_levelObjiectives->setPosition(t->getLocation() - _phasePosition);
	};

	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(_listener, _levelObjiectives);
}
