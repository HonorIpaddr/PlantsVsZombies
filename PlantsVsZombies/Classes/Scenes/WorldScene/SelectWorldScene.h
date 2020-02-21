/**
 *Copyright (c) 2019 LZ.All Right Reserved
 *Author : LZ
 *Date: 2019.8.15
 *Modify: 2020.1.28
 *Email: 2117610943@qq.com
 */

#pragma once
#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "Based/GlobalVariable.h"

using namespace cocos2d;
using namespace cocos2d::ui;
using namespace cocos2d::experimental;

class SelectWorldScene :public Scene
{
public:
	CREATE_FUNC(SelectWorldScene);
	static Scene* createScene();             /* �������� */

CC_CONSTRUCTOR_ACCESS:
	SelectWorldScene(): _scrollView(nullptr),_global(Global::getInstance()) {}
	virtual bool init();

private:
	void showTimeTravelAnimation();
	void showBackground();
	void createScrollView();
	void showDifferentWorlds();
	void createMouseListener();
    void createGoBack();

private:
	Size _backgroundSize;
	ui::ScrollView* _scrollView;
	Button* _world[13];
	Global* _global;
};