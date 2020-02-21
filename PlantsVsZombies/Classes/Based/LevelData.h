#pragma once
#include "cocos2d.h"
#include "json/writer.h"
#include "json/document.h"
#include "json/stringbuffer.h"

using namespace std;
using namespace cocos2d;
using namespace rapidjson;

class LevelData;

class OpenLevelData
{
public:
	/**
	 *����
	 */
	static OpenLevelData* getInstance();

	/**
	 *�򿪹ؿ�����
	 */
	bool openLevelsData(const string& worlddata);

	/*
	 *���ܹؿ�����
	 */
	void decrypt(char* cSrc, char* cDest);
	void decrypt(string& cSrc, char* cDest);

	/**
	 *��ȡ���йؿ�����
	 */
	Document* getDocument();

	/**
	 *����ĳһ���ؿ�����
	 */
	void createLevelData(const int level, const char* levelName);

	/**
	 *��ȡĳһ���ؿ�
	 */
	LevelData* readLevelData(const int level);

	/**
	 *���ùؿ���
	 */
	void setLevelNumber(const int levelNumber);

	/**
	 *��ȡ�ؿ���
	 */
	int getLevelNumber() const;

private:
	OpenLevelData():_document(new Document), _levelNumber(-1){}
	~OpenLevelData() {}

private:
	static OpenLevelData* _instance;
	Document* _document;
	map<int, LevelData*>_levelData;
	int _levelNumber;
};

struct MyPoint
{
	MyPoint():x(0),y(0){}
	int x, y;
};

class LevelData
{
public:
	bool readLevelData(const char* LevelName);
	bool getZombiesVisible() const { return _zombiesIsVisible; }
	bool getZombiesIsSmall() const { return _zombiesIsSmall; }
	bool getZombiesIsBig() const { return _zombiesIsBig; }
	bool getIsNoPlants() const { return _isNoPlants; }
	int getZombiesFrequency() const { return _zombiesFrequency; }
	int getCoinNumbers() const { return _coinNumbers; }
	int getAtLeastSunNumbers() const { return _atLeastSunNumbers; }
	int getFlowerPosition() const { return _flowerPosition; }
	int getCarNumbers() const { return _carNumbers; }
	int getUsePlantsNumbers() const { return _usePlantsNumbers; }
	int getFirstFrequencyTime() const { return _firstFrequencyTime; }
	float getUserLostPosition() const { return _userLose; }
	vector<int>& getGameType() { return _gameType; }
	vector<int>& getZombiesType() { return _zombiesType; }
	vector<int>& getZombiesNumbers() { return _zombiesNumbers; }
	vector<int>& getMunchZombiesFrequency() { return _munchZombiesFrequency; }
	vector<MyPoint>& getNoPlantsPosition() { return _noPlantsPosition; }
	vector<vector<int> >& getZombiesTypeProbabilityFrequency() { return _zombiesTypeProbabilityFrequency; }

CC_CONSTRUCTOR_ACCESS:
	LevelData();
	~LevelData();

private:
	void setGameTypes(const char* LevelName);
	
private:
	bool _isEncryption;                                              /* �Ƿ���� */
	bool _zombiesIsVisible;                                          /* ��ʬ�Ƿ����� */
    bool _zombiesIsSmall;                                            /* �Ƿ���С��ʬ */
	bool _zombiesIsBig;                                              /* �Ƿ��Ǿ��˽�ʬ */
	bool _isNoPlants;                                                /* �Ƿ񲻿���ֲ */
	int _zombiesFrequency;                                           /* ��ʬ�ܲ��� */
	int _coinNumbers;                                                /* ����� */
	int _atLeastSunNumbers;                                          /* ���ٲ����������� */
	int _flowerPosition;                                             /* ��̳λ�� */
	int _carNumbers;                                                 /* С������ */
	int _usePlantsNumbers;                                           /* ʹ��ֲ������ */
	int _firstFrequencyTime;                                         /* ��һ����ʬ����ʱ�� */
	float _userLose;                                                 /* ���ʧ�� */
	vector<int>_gameType;                                            /* ��Ϸ���� */
    vector<int>_zombiesType;                                         /* ��ʬ���� */
	vector<int>_zombiesNumbers;                                      /* ��ʬ�� */
	vector<int>_munchZombiesFrequency;                               /* �ཀྵʬ���� */
	vector<vector<int> >_zombiesTypeProbabilityFrequency;            /* ÿһ��ÿ�ֽ�ʬ���ֵĸ��� */
	vector<MyPoint>_noPlantsPosition;                                /* ��������ֲ�ĵط� */
	Document* _document;
};