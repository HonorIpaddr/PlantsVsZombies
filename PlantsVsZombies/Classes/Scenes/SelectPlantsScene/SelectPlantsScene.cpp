/**
 *Copyright (c) 2019 LZ.All Right Reserved
 *Author : LZ
 *Date: 2019.8.12
 *Modify: 2020.2.3
 *Email: 2117610943@qq.com
 */

#include "SPSControlLayer.h"
#include "SPSBackgroundLayer.h"
#include "SelectPlantsScene.h"
#include "Scenes/GameScene/GameScene.h"

#include "Based/GameType.h"

SelectPlantsScene::SelectPlantsScene() :
	_controlLayer(Layer::create()),
	_scrollLayer(Layer::create()),
	_spriteLayer(nullptr),
	_spriteLayer_(nullptr),
	_requirement(nullptr),
	_scrollView(extension::ScrollView::create()),
	_director(Director::getInstance()),
	_global(Global::getInstance())
{
}

SelectPlantsScene::~SelectPlantsScene()
{
	delete _spriteLayer;
	delete _requirement;
}

Scene* SelectPlantsScene::createScene()
{
	return SelectPlantsScene::create();
}

bool SelectPlantsScene::init()
{
	if (!Scene::init())
	{
		return false;
	}

	createBackgroundLayer();
	createControlLayer();
	schedule(schedule_selector(SelectPlantsScene::eventUpdate), 0.25f);

	return true;
}

void SelectPlantsScene::createBackgroundLayer()
{
	/* ����������С */
	_scrollLayer->setContentSize(Size(2930, 1080));

	/* ���������еĶ��������飩*/
	SPSBackgroundLayer::create()->addLayer(_scrollLayer);
	
	//����������ͼ
	_scrollView = extension::ScrollView::create(_director->getWinSize(), _scrollLayer);
	_scrollView->setPosition(Vec2(0, 0));
	_scrollView->setDirection(extension::ScrollView::Direction::HORIZONTAL);//����ֻ���������
	_scrollView->setTouchEnabled(false);//�رմ����¼�
	_scrollView->runAction(Sequence::create(DelayTime::create(1.0f),
		CallFuncN::create([&](Node* node)
			{
				_scrollView->setContentOffsetInDuration(Vec2(-1010, 0), 2.5f);
			}), nullptr));
	this->addChild(_scrollView);

}

void SelectPlantsScene::createControlLayer()
{
	SPSControlLayer::create()->addLayer(_controlLayer);
	this->addChild(_controlLayer,1000);
}

void SelectPlantsScene::eventUpdate(float Time)
{
	if (Time == 0.22f && _spriteLayer_->_selectFinished)
	{
		unschedule(schedule_selector(SelectPlantsScene::eventUpdate));
		selectPlantsCallBack();
	}
	else if (Time == 0.25f)
	{
		if (_scrollView->getContentOffset().x <= SCROLLRIGHTFINISHED)
		{
			/* ȡ����ʱ���� */
			unschedule(schedule_selector(SelectPlantsScene::eventUpdate)); 

			runAction(Sequence::create(DelayTime::create(1.0f), CallFunc::create([&]() {controlShowRequirement(); }), nullptr));
		}
	}
	else if (Time == 0.3f)
	{
		if (_scrollView->getContentOffset().x >= SCROLLLEFTFINISHED)
		{
			/* �������� */
			AudioEngine::setVolume(AudioEngine::play2d(_global->userInformation->getMusicPath().find("readysetplant")->second), _global->userInformation->getSoundEffectVolume());
			/* ȡ����ʱ���� */
			this->unschedule(schedule_selector(SelectPlantsScene::eventUpdate));

			createReadyText(_global->userInformation->getImagePath().find("StartReady")->second, 1);
		}
	}
}

void SelectPlantsScene::createSelectPlantsDialog()
{
	/* ɾ������ */
	_controlLayer->getChildByName("ControlLayer")->removeChildByName("username");

	_spriteLayer_ = SPSSpriteLayer::create();
	_spriteLayer = new Layer;
	_spriteLayer_->addLayer(_spriteLayer);
	_spriteLayer->setGlobalZOrder(1);
	this->addChild(_spriteLayer);

	schedule(schedule_selector(SelectPlantsScene::eventUpdate), 0.22f);
}

void SelectPlantsScene::controlShowRequirement()
{
	/* �����Ҫ������ʾͨ��Ҫ�� */
	auto _levelData = OpenLevelData::getInstance()->readLevelData(OpenLevelData::getInstance()->getLevelNumber());
	if (_levelData->getGameType().size())
	{
		_requirement = new UserWinRequirement(this);
		_requirement->createDialogBox(GameTypes::None);
		_requirement->setShowDialogAction();

		auto button = Button::create(_global->userInformation->getImagePath().find("Continue1")->second, _global->userInformation->getImagePath().find("Continue")->second);
		button->setTitleText(_global->userInformation->getGameText().find("ȷ��")->second);
		button->setTitleFontName("resources/fonts/fzse_gbk.ttf");
		button->setTitleFontSize(30);
		button->setTitleColor(Color3B::YELLOW);
		button->setPosition(Vec2(_requirement->getDialog()->getContentSize().width / 2.0f, 20));
		button->setGlobalZOrder(30);
		button->setOpacity(0);
		button->runAction(FadeIn::create(0.5f));
		button->addTouchEventListener([=](Ref* sender, ui::Widget::TouchEventType type)
			{
				switch (type)
				{
				case ui::Widget::TouchEventType::BEGAN:
					AudioEngine::setVolume(AudioEngine::play2d(_global->userInformation->getMusicPath().find("tap")->second), _global->userInformation->getSoundEffectVolume());
					break;
				case ui::Widget::TouchEventType::ENDED:
					_requirement->setDelectDialogAction();
					runAction(Sequence::create(DelayTime::create(0.5f), CallFunc::create([&]() { createSelectPlantsDialog();/* �����Ի��� */}), nullptr));
					break;
				}
			});
		_requirement->getDialog()->addChild(button);
	}
	else
	{
		createSelectPlantsDialog();
	}
}

void SelectPlantsScene::selectPlantsCallBack()
{
	_scrollView->setContentOffsetInDuration(Vec2(-220, 0), 2.0f);//���ù���������ʱ��
	this->schedule(schedule_selector(SelectPlantsScene::eventUpdate), 0.3f);
}

void SelectPlantsScene::createReadyText(const std::string& name, const int& id)
{
	static float time[] = { 0,0.2f,0.2f,1.5f };
	auto text = Sprite::create(name);
	text->setScale(15.0f);
	text->setName(name);
	text->setPosition(Director::getInstance()->getWinSize() / 2.0f);
	text->runAction(Sequence::create(ScaleTo::create(0.3f, 2.0f), DelayTime::create(time[id]), CallFuncN::create(CC_CALLBACK_1(SelectPlantsScene::readyTextCallBack, this, name, id)), nullptr));
	this->addChild(text);
}

void SelectPlantsScene::readyTextCallBack(Node* node, const std::string& name, const int& id)
{
	this->removeChildByName(name);
	switch (id)
	{
	case 1:
		createReadyText(_global->userInformation->getImagePath().find("StartSet")->second, 2);
		break;
	case 2:
		createReadyText(_global->userInformation->getImagePath().find("StartPlant")->second, 3);
		break;
	default:
		_scrollLayer->getChildByName("_scrollLayer")->removeChildByName("previewBackgroundImage");

		_global->userInformation->setUserSelectCrads(_spriteLayer_->seedBankButton);
		_global->userInformation->setSunNumbers(100); //�趨��ʼ������ 

		Director::getInstance()->replaceScene(GameScene::createScene());
		break;
	}
}