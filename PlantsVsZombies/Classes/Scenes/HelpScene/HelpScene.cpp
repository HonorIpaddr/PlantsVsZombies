/**
 *Copyright (c) 2019 LZ.All Right Reserved
 *Author : LZ
 *Date: 2019.7.1
 *Email: 2117610943@qq.com
 */

#include "HelpScene.h"
#include "Scenes/MainMenuScene/MainMenu.h"
#include "ui/CocosGUI.h"
#include "Scenes/EasterEggsScene/GameEasterEggs.h"

Scene* HelpScene::createHelpScene()
{
	return HelpScene::create();
}

bool HelpScene::init()
{
	if (!Scene::init()) return false;

	AudioEngine::setVolume(AudioEngine::play2d(_global->userInformation->getMusicPath().find("paper")->second), _global->userInformation->getSoundEffectVolume());

	createBackground();
	createText();
	createGithubUrl();
	createButton();
	
	return true;
}

void HelpScene::createBackground()
{
	/* ��ȡ���ڴ�С */
	auto const size = Director::getInstance()->getWinSize();

	/* �������� */
	auto sprite = Sprite::createWithTexture(Director::getInstance()->getTextureCache()->getTextureForKey(_global->userInformation->getImagePath().find("WorldBackground")->second));
	sprite->setPosition(size / 2);
	this->addChild(sprite);

	auto rotate = Sprite::create(_global->userInformation->getImagePath().find("RunBackground")->second);
	rotate->setScale(3.5f);
	rotate->setPosition(size / 2);
	this->addChild(rotate);
	rotate->runAction(RepeatForever::create(RotateBy::create(1.0f, -10)));

	auto WorldBackgroundBlack = Sprite::create(_global->userInformation->getImagePath().find("WorldBackgroundBlack")->second);
	auto WorldBackgroundBlack1 = Sprite::create(_global->userInformation->getImagePath().find("WorldBackgroundBlack")->second);
	WorldBackgroundBlack->setAnchorPoint(Vec2(0, 1));
	WorldBackgroundBlack->setPosition(Vec2(0, 1140));
	WorldBackgroundBlack1->setAnchorPoint(Vec2(0, 0));
	WorldBackgroundBlack1->setPosition(Vec2(0, -60));
	WorldBackgroundBlack1->setFlippedY(true);
	this->addChild(WorldBackgroundBlack);
	this->addChild(WorldBackgroundBlack1);
}

void HelpScene::createText()
{
	/* �������ֱ��� */
	auto note = Sprite::createWithTexture(Director::getInstance()->getTextureCache()->getTextureForKey(_global->userInformation->getImagePath().find("ZombieNote")->second));
	note->setScale(1.7f);
	note->setScaleX(2.2f);
	note->setPosition(Vec2(size.width / 2 + 60, size.height / 2));
	this->addChild(note);

	auto helptext = Label::createWithTTF("\
		����Ϸȫ���ɱ����Լ�������ɡ��˰汾Ϊ���԰�(����һ���Ŀ�����)�����᲻��ʱ���£��������ƣ���Ŀǰ�Ҽ�����\
����Ŀ��Դ�뿪Դ����GitHub���ڴ˸�л��ҵĹ�ע����                                    ��\n\
		��Ϸ�󲿷ֵ��ز���Դ��ԭ����Ϸ�زģ��ٲ����Ѽ������磬�Լ��Լ�������\n\
		����������Ҫ������ϷΪͬ����Ϸ���ҽ���ѧϰ����ʹ�ã�������Ϸ��Դ���ܴ�����Ȩ�����⣬��������������ҵ��;���������Ը���\n\
		Ŀǰ��7�ֽ�ʬ��11��ֲ�ֲ��ͽ�ʬ�Ķ������Ǳ������ģ��������������������ޣ���Щ��ʬ��ֲ�ﶯ������ʵ�֣�\
�����������������Ը������ҵ��˿���˽���ң���������spine��������Ĺ�����������������ֳ�����ʲô������߶���Ϸ��\
ʲô������Է��͵��ҵ�qq��2117610943", "resources/fonts/fzse_gbk.ttf", 35);

	helptext->setPosition(size / 2.0f);
	helptext->setColor(Color3B::BLACK);
	helptext->setMaxLineWidth(900);
	this->addChild(helptext);
}

void HelpScene::createGithubUrl()
{
	auto github = Button::create();
	github->setTitleFontName("resources/fonts/fzse_gbk.ttf");
	github->setTitleFontSize(35);
	github->setTitleText("����˴���github��Ŀ");
	github->setPosition(Vec2(835, 700));
	github->setTitleColor(Color3B(0, 255, 255));
	github->addTouchEventListener([&](Ref* sender, Widget::TouchEventType type)
		{
			switch (type)
			{
			case Widget::TouchEventType::BEGAN:
				AudioEngine::setVolume(AudioEngine::play2d(_global->userInformation->getMusicPath().find("tap2")->second), _global->userInformation->getSoundEffectVolume());
				break;
			case Widget::TouchEventType::ENDED:
				Application::getInstance()->openURL("https://github.com/ErLinErYi/PlantsVsZombies");
				break;
			}
		});
	this->addChild(github);
	
	auto draw = DrawNode::create();
	draw->drawLine(Vec2(0, 5), Vec2(github->getContentSize().width, 5), Color4F(0, 1, 1, 1));
	draw->setLineWidth(2);
	github->addChild(draw);
}

void HelpScene::createButton()
{
	/* ������ť */
	auto button = Button::create(_global->userInformation->getImagePath().find("SeedChooser_Button2")->second, _global->userInformation->getImagePath().find("SeedChooser_Button2_Glow")->second);
	button->setTitleText(_global->userInformation->getGameText().find("���˵�")->second);
	button->setTitleColor(Color3B(0, 255, 255));
	button->setTitleFontSize(18);
	button->setScale(2.0f);
	button->setPosition(Vec2(size.width / 2, 100));
	this->addChild(button);
	button->addTouchEventListener([&](Ref* sender, Widget::TouchEventType type)
		{
			switch (type)
			{
			case Widget::TouchEventType::BEGAN:
				AudioEngine::setVolume(AudioEngine::play2d(_global->userInformation->getMusicPath().find("tap2")->second), _global->userInformation->getSoundEffectVolume());
				break;
			case Widget::TouchEventType::ENDED:
				Director::getInstance()->replaceScene(MainMenu::createScene());
				break;
			}
		});

}