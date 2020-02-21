#pragma once
#include "cocos2d.h"
#include "spine/spine-cocos2dx.h"
#include "Based/LevelData.h"
#include "Based/GlobalVariable.h"

using namespace spine;
using namespace cocos2d;
using namespace cocos2d::ui;

struct SunNumberRequriement
{
	SunNumberRequriement();
	Text* allSunNumbersText;                     /* ��ʾ�������� */
	int atLeastSunNumbers;                       /* ������Ҫ�������� */
	int allSunNumbers;                           /* ������ */
	bool isHaveSunNumberRequriement;             /* �Ƿ�������Ҫ�� */
	bool isUpdateImage;                          /* ͼƬ�Ƿ���� */
};

struct PlantsRequriement
{
	PlantsRequriement();
	Text* palntsNumbersText;                     /* ��ʾֲ������ */
	int userPlantsNumbers;                       /* ����ʹ�õ�ֲ������ */
	int surPlusPlantsNumbers;                    /* ʣ��ֲ������ */
	bool isHavePlantsRequriement;                /* �Ƿ���ֲ��Ҫ�� */
	bool isUpdateImage;                          /* ͼƬ�Ƿ���� */
};

enum class GameTypes
{
	None = -1,
	CreateWall,
	AtLeastSunNumbers,
	FlowerPosition,
	CarNumbers,
	UserPlantsNumbers,
	ZombiesInvisible,
	SmallZombies,
	BigZombies,
	NoPlants,
	UserLose
};

class GameType
{
public:
	void createGameType();                                            /* ������Ϸ���� */
	void updateRequirementNumbers(const std::string name);            /* �������� */
	void waringPlantsNull();                                          /* ����ֲ��ʹ���� */
	SunNumberRequriement* getSunNumberRequriement() const;
	PlantsRequriement* getPlantsRequriement() const;

CC_CONSTRUCTOR_ACCESS:
	GameType(Node* node);
	~GameType();

private:
	void showNumbers(const int& ID);     /* ��ʾ���� */
	void updateNumbers(const int& ID);   /* �������� */

private:
	Global* _global;
	Node* _node;
	OpenLevelData* _openlevelData;
	SunNumberRequriement* _sunNumberRequriement;
	PlantsRequriement* _plantsRequriement;
};