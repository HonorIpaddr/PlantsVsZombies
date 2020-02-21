/**
 *Copyright (c) 2019 LZ.All Right Reserved
 *Author : LZ
 *Date: 2019.8.12
 *Modify: 2020.2.3
 *Email: 2117610943@qq.com
 */

#pragma once
#include "cocos2d.h"
#include "cocos-ext.h"
#include "SPSSpriteLayer.h"
#include "Based/GlobalVariable.h" 
#include "Based/UserWinRequirement.h"

#define SCROLLRIGHTFINISHED -1010
#define SCROLLLEFTFINISHED  -220

using namespace cocos2d;
using namespace cocos2d::extension;

class SelectPlantsScene :public Scene
{
public:
	static Scene* createScene(); 
	CREATE_FUNC(SelectPlantsScene);

CC_CONSTRUCTOR_ACCESS:
	SelectPlantsScene();
	~SelectPlantsScene();
	virtual bool init();

private:
	/**
	 *����������
	 */
	void createBackgroundLayer();

	/**
	 *�������Ʋ�
	 */
	void createControlLayer();

	/**
	 *�¼�����
	 */
	void eventUpdate(float Time);

	/**
	 *����ֲ��ѡ��Ի���
	 */
	void createSelectPlantsDialog();

	/**
	 *������ʾҪ��
	 */
	void controlShowRequirement();

	/**
	 *ֲ��ѡ��ص�����
	 */
	void selectPlantsCallBack();

	/**
	 *��������
	 */
	void createReadyText(const std::string& name, const int& id);

	/**
	 *���ֻص�
	 */
	void readyTextCallBack(Node* node, const std::string& name, const int& id);

private:
	Layer* _controlLayer;
	Layer* _scrollLayer;
	Layer* _spriteLayer;
	Director* _director;
	Global* _global;
	SPSSpriteLayer* _spriteLayer_;
	extension::ScrollView* _scrollView;
	UserWinRequirement* _requirement;
};