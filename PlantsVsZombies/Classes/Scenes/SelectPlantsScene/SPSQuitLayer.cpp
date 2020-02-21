/**
 *Copyright (c) 2019 LZ.All Right Reserved
 *Author : LZ
 *Date: 2019.8.12
 *Modify: 2020.2.3
 *Email: 2117610943@qq.com
 */

#include "SPSQuitLayer.h"
#include "spine/spine-cocos2dx.h"

using namespace spine;

void SPSQuitLayer::addLayer(Node* node)
{
	_dialogNode = node;
	_dialogNode->addChild(this, 1);
}

bool SPSQuitLayer::init()
{
	if (!Layer::init())return false;

	createShieldLayer(this);

	createDialog();

	return true;
}

void SPSQuitLayer::createDialog()
{
	_option = Sprite::createWithTexture(_director->getTextureCache()->getTextureForKey(_global->userInformation->getImagePath().find("LevelObjiectivesBg")->second));
	_option->setPosition(_director->getWinSize() / 2);
	_option->setScale(0.9f);
	this->addChild(_option);

	auto PauseAnimation = SkeletonAnimation::createWithData(_global->userInformation->getAnimationData().find("PauseAnimation")->second);
	PauseAnimation->setAnimation(0, "animation", true);
	PauseAnimation->setPosition(Vec2(530, 650));
	_option->addChild(PauseAnimation);

	/* �����ƶ����� */
	_touchListener = createTouchtListener(_option);

	/* ���������� */
	auto musicslider = createSlider(Vec2(600, 500), Vec2(150, 500), _global->userInformation->getGameText().find("����")->second, OptionScene_Slider::����,
		Sprite::create(_global->userInformation->getImagePath().find("bgFile")->second),
		Sprite::create(_global->userInformation->getImagePath().find("progressFile")->second),
		Sprite::create(_global->userInformation->getImagePath().find("thumbFile")->second),
		true);
	auto SoundEffectslider = createSlider(Vec2(600, 410), Vec2(150, 410), _global->userInformation->getGameText().find("��Ч")->second, OptionScene_Slider::��Ч,
		Sprite::create(_global->userInformation->getImagePath().find("bgFile")->second),
		Sprite::create(_global->userInformation->getImagePath().find("progressFile")->second),
		Sprite::create(_global->userInformation->getImagePath().find("thumbFile")->second),
		true);

	musicslider->setScale(1.2f);
	SoundEffectslider->setScale(1.2f);

	/* ������ѡ�� */
	auto check = createCheckBox(Vec2(350, 340), Vec2(150, 340), _global->userInformation->getGameText().find("��Ϣ")->second, OptionScene_CheckBox::��ʾ��Ϣ, "CheckBox2", "CheckBox", true);
	auto check1 = createCheckBox(Vec2(800, 340), Vec2(600, 340), _global->userInformation->getGameText().find("ȫ��")->second, OptionScene_CheckBox::ȫ��, "CheckBox2", "CheckBox", true);
	auto check2 = createCheckBox(Vec2(350, 265), Vec2(150, 265), _global->userInformation->getGameText().find("��֡��")->second, OptionScene_CheckBox::��֡��, "CheckBox2", "CheckBox", true);
	auto check3 = createCheckBox(Vec2(800, 265), Vec2(600, 265), _global->userInformation->getGameText().find("�����ʾ")->second, OptionScene_CheckBox::�������, "CheckBox2", "CheckBox", true);
	auto check4 = createCheckBox(Vec2(350, 190), Vec2(150, 190), _global->userInformation->getGameText().find("������ʾ")->second, OptionScene_CheckBox::������ʾ, "CheckBox2", "CheckBox", true);

	check->setScale(0.6f);
	check1->setScale(0.6f);
	check2->setScale(0.6f);
	check3->setScale(0.6f);
	check4->setScale(0.6f);

	/* ������ť */
	GSPauseQuitLayer::createButton(Vec2(210, 100), _global->userInformation->getGameText().find("�鿴ͼ��")->second, PauseQuitLayer_Button::�鿴ͼ��);
	GSPauseQuitLayer::createButton(Vec2(830, 100), _global->userInformation->getGameText().find("�˳���Ϸ")->second, PauseQuitLayer_Button::�˳���Ϸ);
	SPSQuitLayer::createButton(Vec2(520, 100), _global->userInformation->getGameText().find("������Ϸ")->second, PauseQuitLayer_Button::������Ϸ);
}

void SPSQuitLayer::createButton(const Vec2& vec2, const std::string name, PauseQuitLayer_Button button_type)
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
				case PauseQuitLayer_Button::������Ϸ:
					Dialog::deleteDialog();
					break;
				default:
					break;
				}
			}
		});
}