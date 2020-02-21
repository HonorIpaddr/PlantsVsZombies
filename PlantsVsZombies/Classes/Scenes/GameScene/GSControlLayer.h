/**
 *Copyright (c) 2020 LZ.All Right Reserved
 *Author : LZ
 *Date: 2020.1.28
 *Email: 2117610943@qq.com
 */

#pragma once
#include <random>

#include "cocos2d.h"

#include "Based/GlobalVariable.h"

using namespace cocos2d;

class ZombiesAppearControl;
class GSGameEndLayer;
class OpenLevelData;
enum class ZombiesType;
enum class PlantsType;


/* ��Ϸ��ͼ */
struct GameMapInformation
{
	GameMapInformation();
	int plantsMap[5][9];      /* ֲ����ֲ��ͼ */
	int rowNumbers;           /* ���� */
	int columnNumbers;        /* ���� */
};

class GSControlLayer :public Layer
{
public:
	CREATE_FUNC(GSControlLayer);
	void addLayer(Node* node, const int order, const string& name) { node->addChild(this, order, name); }

CC_CONSTRUCTOR_ACCESS:
	GSControlLayer();
	~GSControlLayer();
	virtual bool init();

private:
	void createSchedule();                                                              /* ������ʱ�� */
	void createZombiesHowlSoundEffect();                                                /* ������ʬ������Ч */
	void controlCardEnabled();                                                          /* ���ƿ����Ƿ���� */
	void calculatePlantPosition();                                                      /* ����ֲ����ֲλ�� */
	void createMouseListener();                                                         /* ������� */
	void mouseMoveControl();                                                            /* ����ƶ����� */
	void mouseDownControl(EventMouse* eventmouse = nullptr);                            /* ��갴�¿��� */
	void createPlantsCardListener();                                                    /* ����ֲ�￨�Ƽ��� */
	void selectPlantsPreviewImage();                                                    /* ѡ��ֲ��Ԥ��ͼƬ */
	void createPreviewPlants();                                                         /* ����Ԥ��ֲ�� */
	void zombiesComeTiming();                                                           /* ��ʬ���ּ�ʱ */
	void createZombies();                                                               /* ������ʬ */
	bool controlRefurbishZombies();                                                     /* ���ƽ�ʬˢ�� */
	void controlRefurbishMusicAndText();                                                /* �������������ֵ�ˢ��*/
	bool judgeMousePositionIsInMap();                                                   /* �ж��������λ�Ƿ��ڵ�ͼ�� */
	bool judgeMousePositionIsCanPlant();                                                /* �ж��������λ�Ƿ������ֲֲ�� */
	bool judgeMousePositionHavePlant();                                                 /* �ж��������λ�Ƿ���ֲ�� */
	void removePreviewPlant();                                                          /* �Ƴ�Ԥ��ֲ�� */
	void removeShovel();                                                                /* �Ƴ����� */
	void judgeLevelIsFinished();                                                        /* �жϹؿ��Ƿ���� */
  	void setGameEnd();                                                                  /* ������Ϸ���� */
	
public:
	Vec2 _cur;                                           // ������� 
	Vec2 _plantsPosition;                                // ֲ����ֲ����
	PlantsType _selectPlantsTag;                         // ��ѡֲ����
	GameMapInformation* _gameMapInformation;             // ��Ϸ��ͼ��Ϣ
	ZombiesAppearControl* _zombiesAppearControl;         // ��ʬ���ּ���

private:
	Sprite* _plantPreviewImage;                          // ֲ��Ԥ��ͼƬ
	Sprite* _plantCurImage;                              // ֲ�����ͼƬ
	Global* _global;                                     // ȫ�ֵ���
	Node* _animationLayer;                               // ��Ϸ��
	GSGameEndLayer* _gameEndShieldLayer;                 // ��Ϸ�������β�
	OpenLevelData* _openLevelData;                       // �ؿ����ݵ���
	default_random_engine _random;                       // ���������
	vector<int> _levelData;                              // ��ʱ�ؿ�����
};


/* ��ʬ���ֲ������� */
class ZombiesAppearControl
{
public:
	/**
	 *���ڽ����Ĳ�ͬ����������ͬ������ʬ
	 */
	int getZombiesNumbersForAppearFrequency(const int& ZombiesAppearFrequency);

	/**
	 *��ȡ��ʬ����
	 */
	int getZombiesAppearFrequency() const { return _zombiesAppearFrequency; }

	/**
	 *��ȡ��һ����ʬ�Ƿ��Ѿ�����
	 */
	bool getLastFrequencyZombiesWasDeath() const { return _lastFrequencyZombiesWasDeath; }

	/**
	 *��ȡ��ʱ�Ƿ�ʼ
	 */
	bool getIsBegin() const { return _isBegin; }

	/**
	 *��ȡ�Ƿ���ʾ����
	 */
	bool getIsShowWords() const { return _isShowWords; }

	/**
	 *��ȡʱ��
	 */
	double getTime() const { return _time; }

	/**
	 *ʹÿһ�еĽ�ʬ�ȸ��ʳ���
	 */
	int getEqualProbabilityForRow();

	/**
	 *���ý�ʬ���ֲ���
	 * 1.���ݲ�������
	 * 2.������һ
	 */
	void setZombiesAppearFrequency(const int zombiesAppearFrequency) { _zombiesAppearFrequency = zombiesAppearFrequency; }
	void setZombiesAppearFrequency() { ++_zombiesAppearFrequency; }

	/**
	 *������һ����ʬ�Ƿ�����
	 */
	void setLastFrequencyZombiesWasDeath(bool lastFrequencyZombiesWasDeath) { _lastFrequencyZombiesWasDeath = lastFrequencyZombiesWasDeath; }

	/**
	 *����ʱ������
	 */
	void setTimeClear() { _time = 0; }

	/**
	 *����ʱ������
	 */
	void setTimeAdd() { ++_time; }

	/**
	 *�����Ƿ�ʼ
	 */
	void setIsBegin(const bool isBegin) { _isBegin = isBegin; }

	/**
	 *�����Ƿ���ʾ����
	 */
	void setIsShowWords(const bool isShowWords) { _isShowWords = isShowWords; }

	/**
	 *������ͬ����Ľ�ʬ
	 */
	ZombiesType createDifferentTypeZombies(const int& ZombiesAppearFrequency);

CC_CONSTRUCTOR_ACCESS:
	ZombiesAppearControl();
	~ZombiesAppearControl();

private:
	int _zombiesAppearFrequency;               // ��ʬ���ֲ���
	bool _lastFrequencyZombiesWasDeath;        // ��һ����ʬ������ 
	bool _isBegin;                             // ��ʱ�Ƿ�ʼ
	bool _isShowWords;                         // �Ƿ���ʾ����
	double _time;                              // ��ʱ
	OpenLevelData* _openLevelData;
	default_random_engine _random;             // ���������
	set<int> _judgeZombieRow;                  // �жϽ�ʬ������
	int _zombiesPosition[5] =                  // ��ʬλ��
	{
		130,268,406,544,682
	};
};