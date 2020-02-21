/**
 *Copyright (c) 2019 LZ.All Right Reserved
 *Author : LZ
 *Date: 2019.7.16
 *Email: 2117610943@qq.com
 */
 
#pragma once
#include "ui/CocosGUI.h"
#include "cocos2d.h"
#include "spine/spine.h"
#include "Based/GlobalVariable.h"
#include "spine/spine-cocos2dx.h"

#ifdef WIN32
#pragma execution_character_set("utf-8")
#endif

using namespace cocos2d;
using namespace cocos2d::ui;
using namespace spine;
using namespace cocos2d::experimental;

class MainMenu :public Scene
{
public:
	static Scene* createScene();            /* �������� */
	CREATE_FUNC(MainMenu);

CC_CONSTRUCTOR_ACCESS:
	MainMenu();
	~MainMenu();
	virtual bool init();

private:
	short checkCurInButtons();                                    /* �ж������λ��*/
	void curUpdate(float time);                                   /* ����궨ʱ�� */
	void updateUserNameOnce(float time);
	void setCloudPosition(Node* node, int ID, const Vec2& vec2);  /* �����Ƶ�λ�� */
	void playMusicBleepInGameButtons(int ID);                     /* �������� */
	void playMusicBleepInMainBuutons(int ID, const Vec2& vec2);

	/*��Ϸѡ����*/
	void beginAdventureGame();                                    /* ð��ģʽ */
	void beginSurvivalGame();                                     /* ����С��Ϸ */
	void beginChallengesGame();                                   /* ����ģʽ */
	void beginVasebreakerGame();                                  /* ����ģʽ */

	/*�ص�����*/
	void menuHelpCallBack(Ref* pSender);                          /* ���� */
	void menuOptionCallBack(Ref* pSender);                        /* ѡ�� */
	void menuQuitCallBack(Ref* pSender);                          /* �˳� */
	void menuDataCallBack(Ref* pSender);                          /* ���� */

	/*��������*/
	void createMainSprite();                                      /* ���������˵����� */
	void createParticle();                                        /* ����������Ч */
	void createSmoke(const float& Scale, const Vec2& vec2);       /* ��������������Ч */
	void createMouseListener();                                   /* ������������ */
	void createMainButton();                                      /* ������ť */
	void createClouds();                                          /* ������ */
	void createAnimation();                                       /* �������� */
	void createFlowers();                                         /* �������� */
	void createFlowers(const float& Scale, const Vec2& vec2, const std::string& fileName, const int& ID);

	void setMouseListenerEnable(bool isEnable) { _mouse->setEnabled(isEnable); }

private:
	bool _playMusic[8] = { false };          /* �����Ƿ񲥷� */
	Vec2 _cur;                               /* �����λ�� */
	Text* _userText;                         /* �����û����� */
	Node* _userNameActionParent;             /* �û����ƶ���parent */
	Menu* _menu[2];                          /* �˵� */
	Sprite* _sprite[17];
	Sprite* _mainButton[9];                  /* ���� */
	MenuItemImage* _menuItem[5];             /* �˵��� */
	EventListenerMouse* _mouse;              /* ������ */
	Global* _global;
	Director* _director;
	LayerColor* _inputLayer;                 /* �������β� */
	LayerColor* _quitLayer;                  /* �˳����β� */
	LayerColor* _optionLayer;                /* ѡ�����β� */
};
