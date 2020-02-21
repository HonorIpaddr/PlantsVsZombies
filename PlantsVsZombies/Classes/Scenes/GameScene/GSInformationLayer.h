/**
 *Copyright (c) 2020 LZ.All Right Reserved
 *Author : LZ
 *Date: 2020.1.28
 *Email: 2117610943@qq.com
 */

#pragma once
#include "cocos2d.h"
#include "Based/GlobalVariable.h"
#include "Based/LevelData.h"

using namespace cocos2d;
using namespace cocos2d::ui;

class GSInformationLayer :public Layer
{
public:
	CREATE_FUNC(GSInformationLayer);
	void addLayer(Node* node, const int order, const string& name) { node->addChild(this, order, name); }
	void updateZombiesDieNumbers();
	void updateCoinNumbers();
	void updateSunNumbers();
	void updateProgressBar(const int& zombiesAppearFrequency);
	bool updateProgressBarFlag();
	bool updateProgressBarFlag(const int& id);
	void updateProgressBarHead();
	void sunNumberTextWarning();
	void createPromptText();

CC_CONSTRUCTOR_ACCESS:
	GSInformationLayer();
	~GSInformationLayer();
	virtual bool init();

private:
	void showUserText();
	void showProgressBar();
	void showZombiesDieNumbers();
	void showCoinNumbers();
	void showSunNumbers();
	void showPromptMuchZombiesText(const string& textName);
	
private:
	float _levelLastPrecent;            // ��������һ�ν���
	Text* _zombiesDieText;              // ��ʬ�������ı�
	Text* _coinNumbersText;             // ������ı�
	Text* _sunNumbersText;              // �������ı�
	ProgressTimer* _progressBar;        // ������
	Global* _global;                    // ȫ�ֱ���
	OpenLevelData* _openLevelData;
};