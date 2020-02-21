/**
 *Copyright (c) 2019 LZ.All Right Reserved
 *Author : LZ
 *Date: 2019.7.17
 *Modify: 2020.2.3
 *Email: 2117610943@qq.com
 */

#include "QuitScene.h"
#include "Based/GlobalVariable.h"

void QuitMenu::addLayer(Node* node)
{
	_dialogNode = node;
	_dialogNode->addChild(this, 1);
}

bool QuitMenu::init()
{
	if (!Layer::init())return false;

	createShieldLayer(this);

	this->createDiglog();

	return true;
}

void QuitMenu::deleteDialog()
{
	setMouseListenerEnable();
	_dialogNode->removeFromParent();
}

void QuitMenu::createDiglog()
{
	_quitDialog = Sprite::createWithTexture(Director::getInstance()->getTextureCache()->getTextureForKey(_global->userInformation->getImagePath().find("QuitDialog")->second));
	_quitDialog->setPosition(Director::getInstance()->getWinSize() / 2);
	_quitDialog->setScale(2.0f);
	this->addChild(_quitDialog);

	this->createButtons(_global->userInformation->getGameText().find("退出游戏")->second, Vec2(90, 65), 1);
	this->createButtons(_global->userInformation->getGameText().find("取消")->second, Vec2(225, 65), 2);

	/* 创建触摸监听 */
	this->createTouchtListener(_quitDialog);
}

void QuitMenu::createButtons(const std::string &Label, Vec2 &vec2,const int& ID)
{
	auto button = Button::create(_global->userInformation->getImagePath().find("button")->second, _global->userInformation->getImagePath().find("button_down")->second);
	button->setTitleLabel(label(Label, 20));
	button->setPosition(vec2);
	_quitDialog->addChild(button);

	_quitDialog->addChild(label(_global->userInformation->getGameText().find("退出")->second, 25, Vec2(155, 160), 0, Color3B::YELLOW));
	_quitDialog->addChild(label(_global->userInformation->getGameText().find("确定要退出游戏吗？")->second, 15, Vec2(160, 130), 0, Color3B::YELLOW));

	button->addTouchEventListener([=](Ref* sender, Widget::TouchEventType type)
	{
		switch (type)
		{
		case Widget::TouchEventType::BEGAN:
			AudioEngine::setVolume(AudioEngine::play2d(_global->userInformation->getMusicPath().find("gravebutton")->second), _global->userInformation->getSoundEffectVolume());
			break;
		case Widget::TouchEventType::ENDED:
			switch (ID)
			{
			case 1:
				Director::getInstance()->end();
				break;
			case 2:
				deleteDialog();
				break;
			}
		}
	});
}