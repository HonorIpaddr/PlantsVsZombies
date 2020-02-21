/**
 *Copyright (c) 2020 LZ.All Right Reserved
 *Author : LZ
 *Date: 2020.1.28
 *Email: 2117610943@qq.com
 */
#include "spine/spine-cocos2dx.h"

#include "GSPauseQuitLayer.h"

#include "Scenes/SelectPlantsScene/SelectPlantsScene.h"
#include "Scenes/WorldScene/World_1.h"

using namespace spine;

Layer* GSPauseQuitLayer::addLayer()
{
	return GSPauseQuitLayer::create();
}

void GSPauseQuitLayer::pauseLayer()
{
	Director::getInstance()->getRunningScene()->getChildByName("backgroundLayer")->onExit();/* ��ͣ�� */
	Director::getInstance()->getRunningScene()->getChildByName("buttonLayer")->onExit();
	Director::getInstance()->getRunningScene()->getChildByName("animationLayer")->onExit();
	Director::getInstance()->getRunningScene()->getChildByName("controlLayer")->onExit();
	Director::getInstance()->getRunningScene()->getChildByName("informationLayer")->onExit();
	Director::getInstance()->getRunningScene()->getChildByName("sunLayer")->onExit();
}

void GSPauseQuitLayer::resumeLayer()
{
	Director::getInstance()->getRunningScene()->getChildByName("backgroundLayer")->onEnter();/* �ָ��� */
	Director::getInstance()->getRunningScene()->getChildByName("buttonLayer")->onEnter();
	Director::getInstance()->getRunningScene()->getChildByName("animationLayer")->onEnter();
	Director::getInstance()->getRunningScene()->getChildByName("controlLayer")->onEnter();
	Director::getInstance()->getRunningScene()->getChildByName("informationLayer")->onEnter();
	Director::getInstance()->getRunningScene()->getChildByName("sunLayer")->onEnter();
}

bool GSPauseQuitLayer::init()
{
	if (!Layer::init())return false;

	/* �����Ի��� */
	createDialog();

	return true;
}

void GSPauseQuitLayer::createDialog()
{
	_grayLayer = LayerColor::create(Color4B(0, 0, 0, 180));
	this->addChild(_grayLayer);

	_option = Sprite::createWithTexture(_director->getTextureCache()->getTextureForKey(_global->userInformation->getImagePath().find("LevelObjiectivesBg")->second));
	_option->setPosition(_director->getWinSize() / 2);
	_option->setScale(0.9f);
	_grayLayer->addChild(_option);

	auto PauseAnimation= SkeletonAnimation::createWithData(_global->userInformation->getAnimationData().find("PauseAnimation")->second);
	PauseAnimation->setAnimation(0, "animation", true);
	PauseAnimation->setPosition(Vec2(530, 650));
	_option->addChild(PauseAnimation);

	/* �����ƶ����� */
	_touchListener = createTouchtListener(_option);

	/* ���������� */
	auto musicslider = createSlider(Vec2(600, 520), Vec2(150, 520), _global->userInformation->getGameText().find("����")->second, OptionScene_Slider::����,
		Sprite::create(_global->userInformation->getImagePath().find("bgFile")->second),
		Sprite::create(_global->userInformation->getImagePath().find("progressFile")->second),
		Sprite::create(_global->userInformation->getImagePath().find("thumbFile")->second),
		true);
	auto SoundEffectslider = createSlider(Vec2(600, 450), Vec2(150, 450), _global->userInformation->getGameText().find("��Ч")->second, OptionScene_Slider::��Ч,
		Sprite::create(_global->userInformation->getImagePath().find("bgFile")->second),
		Sprite::create(_global->userInformation->getImagePath().find("progressFile")->second),
		Sprite::create(_global->userInformation->getImagePath().find("thumbFile")->second),
		true);

	musicslider->setScale(1.2f);
	SoundEffectslider->setScale(1.2f);

	/* ������ѡ�� */
	auto check    = createCheckBox(Vec2(350, 380), Vec2(150, 380), _global->userInformation->getGameText().find("��Ϣ")->second, OptionScene_CheckBox::��ʾ��Ϣ, "CheckBox2", "CheckBox", true);
	auto check1   = createCheckBox(Vec2(800, 380), Vec2(600, 380), _global->userInformation->getGameText().find("ȫ��")->second, OptionScene_CheckBox::ȫ��, "CheckBox2", "CheckBox", true);
	auto check2   = createCheckBox(Vec2(350, 310), Vec2(150, 310), _global->userInformation->getGameText().find("��֡��")->second, OptionScene_CheckBox::��֡��, "CheckBox2", "CheckBox", true);
	auto check3   = createCheckBox(Vec2(800, 310), Vec2(600, 310), _global->userInformation->getGameText().find("�����ʾ")->second, OptionScene_CheckBox::�������, "CheckBox2", "CheckBox", true);
	auto check4   = createCheckBox(Vec2(350, 240), Vec2(150, 240), _global->userInformation->getGameText().find("������ʾ")->second, OptionScene_CheckBox::������ʾ, "CheckBox2", "CheckBox", true);

	check->setScale(0.6f);
	check1->setScale(0.6f);
	check2->setScale(0.6f);
	check3->setScale(0.6f);
	check4->setScale(0.6f);

	/* ������ť */
	createButton(Vec2(210, 170), _global->userInformation->getGameText().find("�鿴ͼ��")->second, PauseQuitLayer_Button::�鿴ͼ��);
	createButton(Vec2(520, 170), _global->userInformation->getGameText().find("���¿�ʼ")->second, PauseQuitLayer_Button::���¿�ʼ);
	createButton(Vec2(830, 170), _global->userInformation->getGameText().find("������Ϸ")->second, PauseQuitLayer_Button::������Ϸ);
	createButton(Vec2(365, 70), _global->userInformation->getGameText().find("����˵��")->second, PauseQuitLayer_Button::����˵��);
	createButton(Vec2(665, 70), _global->userInformation->getGameText().find("�˳���Ϸ")->second, PauseQuitLayer_Button::�˳���Ϸ);

}

void GSPauseQuitLayer::createButton(const Vec2& vec2, const std::string name, PauseQuitLayer_Button button_type)
{
	/* �����������˵���ť */
	auto button = ui::Button::create(_global->userInformation->getImagePath().find("ButtonNew2")->second, _global->userInformation->getImagePath().find("ButtonNew")->second);
	auto label = Label::createWithTTF(name, "resources/fonts/fzse_gbk.ttf", 35);
	label->enableShadow(Color4B(0, 0, 0, 200));//������Ӱ
	label->setScale(2.0f);
	button->setTitleLabel(label);
	button->setTitleColor(Color3B::WHITE);
	button->setPosition(vec2);
	button->setScale(0.5f);
	_option->addChild(button);

	button->addTouchEventListener([=](Ref* sender, ui::Widget::TouchEventType type)
		{
			switch (type)
			{
			case ui::Widget::TouchEventType::BEGAN:
				AudioEngine::setVolume(AudioEngine::play2d(_global->userInformation->getMusicPath().find("gravebutton")->second), _global->userInformation->getSoundEffectVolume());
				break;
			case ui::Widget::TouchEventType::ENDED:
				switch (button_type)
				{
				case PauseQuitLayer_Button::�鿴ͼ��:
					Application::getInstance()->openURL("https://share.weiyun.com/5TewoDc");
					break;
				case PauseQuitLayer_Button::���¿�ʼ:
					_director->getScheduler()->setTimeScale(1.0f);
					_director->replaceScene(TransitionFade::create(1.0f, SelectPlantsScene::createScene()));
					break;
				case PauseQuitLayer_Button::�˳���Ϸ:
					_director->getScheduler()->setTimeScale(1.0f);
					_director->replaceScene(TransitionFade::create(1.0f, World_1::createScene()));
					break;
				case PauseQuitLayer_Button::����˵��:
					showPrompt();
					break;
				case PauseQuitLayer_Button::������Ϸ:
					resumeLayer();
					this->removeFromParent();
					break;
				default:
					break;
				}
			}
		});
}

void GSPauseQuitLayer::showPrompt()
{
	_promptLayer = LayerColor::create(Color4B(0, 0, 0, 180));
	createShieldLayer(_promptLayer);
	this->addChild(_promptLayer);

	auto prompt = Sprite::create(_global->userInformation->getImagePath().find("Prompt")->second);
	prompt->setPosition(_director->getWinSize() / 2.0f);
	prompt->setScale(1.2f);
	prompt->setOpacity(200);
	_promptLayer->addChild(prompt);

	this->createTouchtListener(prompt);

	auto Close = ui::Button::create(_global->userInformation->getImagePath().find("CloseDown")->second, _global->userInformation->getImagePath().find("Close")->second);
	Close->setPosition(Vec2(1150, 520));
	Close->setScale(0.6f);
	prompt->addChild(Close);

	Close->addTouchEventListener([=](Ref* sender, ui::Widget::TouchEventType type)
		{
			switch (type)
			{
			case ui::Widget::TouchEventType::BEGAN:
				AudioEngine::setVolume(AudioEngine::play2d(_global->userInformation->getMusicPath().find("tap")->second), _global->userInformation->getSoundEffectVolume());
				break;
			case ui::Widget::TouchEventType::ENDED:
				_promptLayer->removeFromParent();
				break;
			}
		});
}