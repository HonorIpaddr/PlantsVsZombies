/**
 *Copyright (c) 2020 LZ.All Right Reserved
 *Author : LZ
 *Date: 2020.1.26
 *Email: 2117610943@qq.com
 */

#pragma once
#include "cocos2d.h"
#include "Based/LevelData.h"
#include "spine/spine.h"
#include "AudioEngine.h"
#include "Based/GlobalVariable.h"
#include "spine\spine-cocos2dx.h"

using namespace spine;
using namespace cocos2d;
using namespace cocos2d::experimental;

class Plants;
class GSGameResultJudgement;
class GSGameEndLayer;

/* ��ʬ���� */
enum class ZombiesType
{
	None = 0,
	CommonZombies,
	ConeZombies,
	BucketZombies,
	CommonDoorZombies,
	ConeDoorZombies,
	BucketDoorZombies,
	LmpZombies
};

class Zombies :public Node
{
public:
	/**
	 *��ȡ���ֽ�ʬ���ද������
	 */
	static string getZombieAniamtionName(ZombiesType zombiestype);

	/**
	 *ɾ����ʬ
	 */
	static void zombiesDeleteUpdate(list<Zombies*>::iterator& zombie);

	/**
	 *��ȡ��ʬ����
	 */
	static unsigned int getZombiesNumbers();

	/**
	 *���ý�ʬ����
	 */
	static void setZombiesNumbers(const unsigned int numbers);

	/**
	 *�жϽ�ʬӮ
	 */
	static void judgeZombieWin(list<Zombies*>::iterator zombie);

	/**
	 *��ʬ��Ӯ��ʼ��
	 */
	static void zombiesWinOrLoseInit();

	/**
	 *������ʬ
	 */
	virtual void createZombie() = 0;

	/**
	 *����Ԥ����ʬ
	 */
	virtual void createPreviewZombie() = 0;

	/**
	 *��ʬ�����¼�����
	 */
	virtual void zombieInjuredEventUpdate() = 0;

	/**
	 *������ʬӰ��
	 */
	virtual void createZombieShadow();

	/**
	 *���ý�ʬ��С
	 */
	virtual void setZombieScale(const Size& scale = Size(40, 40)) const;

	/**
	 *���ý�ʬ��С
	 */
	virtual void setZombieScale(const int& scale) const;

	/**
	 *���ö��������ٶ�
	 */
	virtual void setZombieTimeScale(const float& timeScale) const;

	/**
	 *���ý�ʬ�������
	 */
	virtual void setZombieAnimationInformation(const int tag, const bool isLoop = true) const;

	/**
	 *���ý�ʬ����
	 */
	virtual void setZombieAnimationName(const string& name, bool isLoop = true) const;

	/**
	 *���ý�ʬ�Ĵ�С
	 */
	virtual void setZombieAttributeForGameType();

	/**
	 *�����Ƿ���ʾ��ʬ����֫�嶯��
	 */
	virtual void setZombieIsShowLoseLimbsAnimation(const bool isShow);

	/**
	 *���ý�ʬ����ֲ����
	 */
	virtual void setZombieEatPlantNumber(const int& number);

	/**
	 *���ý�ʬ�ƶ�
	 */
	virtual void setZombieMove(const float delta);

	/**
	 *���ý�ʬ͸����
	 */
	virtual void setZombieOpacity(GLubyte opacity);

	/**
	 *����λ��
	 */
	virtual void setZombiePosition(const Vec2& position);

	/**
	 *��������
	 */
	virtual void setZombieName(const string& name); 

	/**
	 *���ý�ʬ���
	 */
	virtual void setZombieTag(const int& tag);

	/**
	 *���ý�ʬ��ǰ�ٶ�
	 */
	virtual void setZombieCurrentSpeed(const int currentSpeed);

	/**
	 *���ý�ʬֹͣ�ƶ�
	 */
	virtual void setZombieStop();

	/**
	 *���ý�ʬ�Ƿ���ʾ
	 */
	virtual void setZombieIsShow(const bool isShow);

	/**
	 *���ý�ʬ�Ƿ��ֲ��
	 */
	virtual void setZombieIsEat(const bool isEat);

	/**
	 *���ý�ʬ����
	 */
	virtual void setZombieDeath(const bool death);

	/**���ý���ʬ�Ƿ���ʾ
	 */
	virtual void setZombieVisible(const bool isVisible);

	/**
	 *���ý�ʬ���廤��Ѫ��
	 */
	virtual void setZombieCurrentBodyShieldVolume(const float currentBodyShieldVolume);

	/**
	 *���ý�ʬͷ������Ѫ��
	 */
	virtual void setZombieCurrentHeadShieldVolume(const float currentHeadShieldVolume);

	/**
	 *���ý�ʬѪ��
	 */
	virtual void setZombieCurrentBloodVolume(const float currentBloodVolume);

	/**
	 *��ȡ��ʬ����
	 */
	virtual SkeletonAnimation* getZombieAnimation() const;

	/**
	 *��ʬ�Ƿ���
	 */
	virtual bool getZombieIsSurvive() const;

	/**
	 *��ȡ��ʬ�Ƿ���ʾ
	 */
	virtual bool getZombieIsVisible() const;

	/**
	 *��ȡ��ʬ����
	 */
	virtual string getZombieName() const;

	/**
	 *��ȡ��ʬ���
	 */
	virtual int getZombieTag() const;

	/**
	 *��ȡ��ʬ����ֲ����
	 */
	virtual int getZombieEatPlantNumber() const;

	/**
	 *��ʬû�в�����������
	 */
	virtual bool getZombieIsPlayDieAnimation() const;

	/**
	 *��ȡ��ʬλ��
	 */
	virtual Vec2 getZombiePosition()  const;

	/**
	 *��ȡ��ʬx����
	 */
	virtual float getZombiePositionX() const;

	/**
	 *��ȡ����y����
	 */
	virtual float getZombiePositionY() const; 
	
	/**
	 *��ȡ��ʬ�Ƿ�����ͼ
	 */
	virtual bool getZombieIsEnterMap() const;

	/**
	 *��ȡ��ʬ���廤��Ѫ��
	 */
	virtual float getZombieCurrentBodyShieldVolume() const;

	/**
	 *��ȡ��ʬͷ������Ѫ��
	 */
	virtual float getZombieCurrentHeadShieldVolume() const;

	/**
	 *��ȡ��ʬѪ��
	 */
	virtual float getZombieCurrentBloodVolume() const;

	/**
	 *��ȡ��ʬ�Ƿ��ڳ�ֲ��
	 */
	virtual bool getZombieIsEat() const;

	/**
	 *��ȡ��ʬ�Ƿ�ֹͣ
	 */
	virtual bool getZombieIsStop() const;

	/**
	 *@ 1.��ȡ��ʬ��ǰ���ٶ�
	 */
	virtual float getZombieCurrentSpeed() const;

	/**
	 *@ 2.��ȡ��ʬԭ���ٶ�
	 */
	virtual float getZombieSpeed() const;

	/**
	 *��ȡ��ʬ�Ƿ���ʾ
	 */
	virtual bool getZombieIsShow() const;

	/**
	 *��ȡ��ʬ����
	 */
	virtual ZombiesType getZombieType() const;

	/**
	 *��ȡ��ʬͷ������������Ч 
	 */
	virtual int getZombieHeadAttackSoundEffect() const;

	/**
	 *��ȡ��ʬ���屻������Ч
	 */
	virtual int getZombieBodyAttackSoundEffect() const;

	/**
	 *��ȡ�Ƿ��ɾ��
	 */
	virtual bool* getIsCanDelete();

	/**
	 *��ȡ��ʬ�Ƿ��л���
	 */
	virtual bool getZombieIsHaveShield() const;

	/**
	 *���Ž�ʬ��������
	 */
	virtual void playZombiesDieAnimation(const string& animationName);

protected:
	/*
	 *��ʼ����ʬ
	 */
	virtual void zombieInit(const string& animation_name = "Zombies");

	/**
     *��ȡ��ʬ����ͼ��
     */
	float getLocalZOrder(const int& positiionY) const;

	/**
	 *���ýڵ�
	 */
	virtual void setNode(Node* node){ _node = node; }

	/**
	 *���ý�ʬһ������
	 */
	virtual void setZombiePrimaryInjure();

	/**
	 *���ý�ʬ��������
	 */
	virtual void setZombieSecondaryInjure();

	/**
	 *��ʬ���첲����
	 */
	virtual void zombieLoseArmAnimation(const std::string& name);

	/**
	 *��ʬ��ͷ����
	 */
	virtual void zombieLoseHeadAnimation(const std::string& name);

	/**
	 *��ʬ�����ܶ���
	 */
	virtual void zombieLoseShieldAnimation(const std::string& name);

	/**
	 *��ʬ��ʧ����
	 */
	virtual void zombiesFadeOutAnimation();

	/**
	 *����ͷ������һ������
	 *����������֮ǰ����
	 *������Ҫ����������
	 *���������˵ȼ�
	 */
	virtual void setZombieHeadShieldPrimaryInjure(const string& oldName, const string& newName);

	/**
	 *����ͷ�����ܶ�������
	 *����������֮ǰ����
	 *������Ҫ����������
	 *���������˵ȼ�
	 */
	virtual void setZombieHeadShieldSecondaryInjure(const string& oldName, const string& newName);

	/**
	 *����ͷ��������������
	 *����������֮ǰ����
	 *������Ҫ����������
	 *���������˵ȼ�
	 */
	virtual void setZombieHeadShieldThirdInjure(const string& oldName, const string& newName);

	/**
	 *�������廤��һ������
	 *����������֮ǰ����
	 *������Ҫ����������
	 *���������˵ȼ�
	 */
	virtual void setZombieBodyShieldPrimaryInjure(const string& oldName, const string& newName);

	/**
	 *�������廤�ܶ�������
	 *����������֮ǰ����
	 *������Ҫ����������
	 *���������˵ȼ�
	 */
	virtual void setZombieBodyShieldSecondaryInjure(const string& oldName, const string& newName);

	/**
	 *�������廤����������
	 *����������֮ǰ����
	 *������Ҫ����������
	 *���������˵ȼ�
	 */
	virtual void setZombieBodyShieldThirdInjure(const string& oldName, const string& newName);

CC_CONSTRUCTOR_ACCESS:
	Zombies();
	~Zombies();

private:
	void setSmallZombieAttribute();
	void setBigZombieAttribute();
	void setZombieAttributeForGameType(Node* sprite);
	void setOpacityZombieAttribute();
	string& createZombieName();

protected:
	int _attackHeadSoundEffectType;           // ����ͷ����Ч   
	int _attackBodySoundEffectType;           // ����������Ч     
	int _animationId;                         // �������
	int _zombieTag;                           // ��ʬ���(��δʹ��)
	float _bloodVolume;                       // Ѫ��
	float _headShieldVolume;                  // ����Ѫ��
	float _bodyShieldVolume;                  // ����1Ѫ��
	float _currentBloodVolume;                // ��ǰѪ��
	float _currentHeadShieldVolume;           // ��ǰ����Ѫ��
	float _currentBodyShieldVolume;           // ��ǰ����1Ѫ��
	float _speed;                             // �ƶ��ٶ�
	float _currentSpeed;                      // �洢�ƶ��ٶ�
	bool _isHaveShield;                       // �Ƿ��л���
	bool _isUseForGameType;                   // �Ƿ�ʹ������Ϸ����
	bool _isEat;                              // �Ƿ��ڳ�ֲ��
	bool _isEatGarlic;                        // �Ƿ�Դ���
	bool _isShow;                             // �Ƿ���ʾ
	bool _isPreview;                          // �Ƿ���Ԥ��ģʽ
	bool _isShowLoseLimbsAnimation;           // �Ƿ���ʾ��ʬ����֫�嶯��
	bool _isCanDelete[2];                     // �Ƿ����ɾ��
	string _zombieName;                       // ��ʬ����
	Vec2 _position;                           // λ��
	Node* _node;                              // �ڵ�
	Global* _global;                          // ȫ�ֱ���
	SkeletonAnimation* _zombiesAnimation;     // ��ʬ����
	ZombiesType _zombiesType;                 // ��ʬ����
	string _animationName[4];                 // ��������
	static unsigned int _zombiesNumbers;      // ��ʬ����

private:
	int _zombieEatPlantNumber;
	OpenLevelData* _openLevelData;

	static bool _zombieIsWin;
	static GSGameEndLayer* _gameEndLayer;
	static GSGameResultJudgement* _gameResultJudgement;
};
