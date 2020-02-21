/**
 *Copyright (c) 2020 LZ.All Right Reserved
 *Author : LZ
 *Date: 2020.1.28
 *Email: 2117610943@qq.com
 */

#pragma once
#include "cocos2d.h"
#include "Scenes/SelectPlantsScene/SPSControlLayer.h"
#include "Based/UserWinRequirement.h"
#include "Based/GameType.h"
#include "Based/LevelData.h"
#include "Plants/Plants.h"

using namespace cocos2d;
using namespace cocos2d::ui;

struct MouseSelectImage /* ���ѡ���ֲ�� */
{
	MouseSelectImage():Iamge(nullptr), isSelectPlants(false), isSelectShovel(false){}
	Sprite* Iamge;                    /* ͼƬ */
	PlantsType selectPlantsId;        /* ��ѡֲ���� */
	bool isSelectPlants;              /* �Ƿ�ѡ����ֲ�� */
	bool isSelectShovel;              /* �Ƿ�ѡ���˲��� */
};

struct PlantsInformation
{
	struct PlantsCards
	{
		PlantsCards():timeBarIsFinished(false){}
		Button* plantsCards;            /* ���� */
		Text* plantsCardText;           /* �������� */
		ProgressTimer* progressTimer;   /* ����ʱ */
		int plantsNeedSunNumbers;       /* �������� */
		int tag;                        /* ��� */
		bool timeBarIsFinished;         /* ����ʱ�Ƿ���� */
	};

	/* ֲ����ȴʱ�� */
	float PlantsCoolTime[11] =
	{
		/*���տ�*/  7.5f, /*�㶹����    */  7.5f, /*���    */  30,   /*ӣ��ը��*/  35,
		/*������*/  30,   /*���Ĳ�Ͷ��  */  7.5f, /*�����׮*/  7.5f, /*�ش�    */  7.5f,
		/*����  */  10,   /*�����㶹����*/  10,   /*������*/  35
	};
	/* ֲ���������� */
	int PlantsNeedSunNumbers[11] =
	{
		/*���տ�*/ 50, /*�㶹����    */ 100, /*���    */ 50,  /*ӣ��ը��*/  150,
		/*������*/ 25, /*���Ĳ�Ͷ��  */ 100, /*�����׮*/ 175, /*�ش�    */  100,
		/*����  */ 50, /*�����㶹����*/ 200, /*������*/ 150
	};
};

enum class GSLayerButton
{
	stopButton = 1,
	accelerateButton,
	decelerateButton
};

class GSButtonLayer :public SPSControlLayer
{
public:
	CREATE_FUNC(GSButtonLayer);

	/**
	 *��ӵ���������
	 */
	void addLayer(Node* node, const int order, const string& name) { node->addChild(this, order, name); }

CC_CONSTRUCTOR_ACCESS:
	GSButtonLayer();
	~GSButtonLayer();
	virtual bool init();

private:
	Button* createButton(const string& normal, const string& select, const Vec2& position, GSLayerButton buttonName, const bool isFlippedX = false);
	void createKeyBoardListener();
	void createQuitDialog();
	void createRequirementButton();
	void createRequirementLayer();
	void controlAccelerateScheduler();
	void controlDecelerateScheduler();
	void showSeedBank();
	void showShovelBank();
	void createPlantsCard();
	ProgressTimer* createProgressTimer(Button* button, const float _time, const int& id);
	
public:
	MouseSelectImage* mouseSelectImage;                 // ���ѡ��
	PlantsInformation* plantsInformation;               // ֲ������
	PlantsInformation::PlantsCards plantsCards[20];     // ֲ�￨Ƭ
	
private:
	Global* _global;                                     // ȫ�ֱ���
	Director* _director;                                 // ���ݵ���
	Layer* _quitLayer;                                   // �˳����β�
	Button* _accelerateButton;                           // ���ٰ�ť
	Button* _decelerateButton;                           // ���ٰ�ť
	OpenLevelData* _openLevelData;
};

