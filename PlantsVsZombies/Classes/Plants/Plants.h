/**
 *Copyright (c) 2020 LZ.All Right Reserved
 *Author : LZ
 *Date: 2020.2.5
 *Email: 2117610943@qq.com
 */

#pragma once
#include "cocos2d.h"
#include "spine/spine.h"
#include "AudioEngine.h"
#include "Based/GlobalVariable.h"
#include "spine\spine-cocos2dx.h"

#define INIT Vec2(-1000,-1000)
#define NOINITIALIZATION  -1

using namespace spine;
using namespace cocos2d;
using namespace cocos2d::experimental;

class Zombies;

enum class PlantsType
{
	None = -1,
	SunFlower,          /* ���տ� */
	PeaShooter,         /* �㶹���� */
	WallNut,            /* ���ǽ */
	CherryBomb,         /* ӣ��ը�� */
	PotatoMine,         /* ������ */
	CabbagePult,        /* ���Ĳ�Ͷ�� */
	Torchwood,          /* �����׮ */
	Spikeweed,          /* �ش� */
	Garlic,             /* ���� */
	FirePeaShooter,     /* �����㶹���� */
	Jalapeno,           /* ������ */
	AcidLemonShooter    /* ǿ���������� */
	//...
};  

class  Plants :public Node
{
public:
	/**
	 *������Ч
	 */
	static void playSoundEffect(const std::string& MusicName);

	/**
	 *��ֲͣ�����ж���
	 */
	static void stopPlantsAllAction();

	/**
	 *����ͼƬ
	*/
	virtual Sprite* createPlantImage() = 0;

	/**
	 *����ֲ�ﶯ��
	 */
	virtual void createPlantAnimation() = 0;

	/**
	 *�жϽ�ʬ��ֲ��֮������λ��
	 */
	virtual void determineRelativePositionPlantsAndZombies();

	/**
	 *���ֲ���������
	 */
	virtual void checkPlantHealthPoint() {}

	/**
	 *���ýڵ�
	 */
	virtual void setPlantNode(Node* node) { _node = node; }

	/**
	 *����λ��
	 */
	virtual void setPlantPosition(const Vec2& position) { _position = position; }

	/**
	 *���û���˳��
	 */
	virtual void setPlantLocalZOrder(const int& order) { _zOrder = order; }

	/**
	 *����ֲ����������
	 */
	virtual void setPlantRowAndColumn(const Vec2& rowAndColumn) { _rowAndColumn = rowAndColumn; }

	/**
	 *����ֲ���Ƿ�ɼ�
	 */
	virtual void setPlantVisible(const bool visible);

	/**
	 *��ȡֲ������
	 */
	PlantsType getPlantType() const { return _plantsType; }

	/**
	 *����ֲ����
	 */
	virtual void setPlantTag(const int& tag) { _plantTag = tag; }

	/**
	 *����ֲ������ֵ
	 */
	virtual void setPlantHealthPoint(int healthPoint) { _healthPoint = healthPoint; }

	/**
	 *��ȡֲ����
	 */
	virtual int getPlantTag() const { return _plantTag; }

	/**
	 *��ȡ����
	 */
	SkeletonAnimation* getPlantAnimation() { return _plantAnimation; }

	/**
	 *�ж�ֲ���Ƿ���
	 */
	virtual bool getPlantIsSurvive() const { return _healthPoint > 0 ? true : false; }

	/**
	 *��ȡֲ������ֵ
	 */
	virtual float getPlantHealthPoint() const { return _healthPoint; }

	/**
	 *@ 1.��ȡֲ����������
	 */
	virtual Vec2 getPlantRowAndColumn() const { return _rowAndColumn; }

	/**
	 *@ 2.��ȡֲ��������
	 */
	virtual int getPlantRow() const { return _rowAndColumn.x; }

	/**
	 *@ 3.��ȡֲ��������
	 */
	virtual int getPlantColumn() const { return _rowAndColumn.y; }

	/**
	 *��ȡ�Ƿ���ʾ
	 */
	virtual bool getPlantVisible();

	/**
	 *��ȡֲ���Ƿ����ɾ��
	 */
	virtual bool* getPlantIsCanDelete();

CC_CONSTRUCTOR_ACCESS:
	Plants(Node* node = nullptr, const Vec2& position = Vec2::ZERO);
	~Plants();
	
protected:
	/**
	 *��ֲֲ��
	 */
	virtual SkeletonAnimation* plantInit(const std::string& plantname, const std::string& animaionname);

	/**
	 *����ֲ��ͼƬ
	 */
	virtual void imageInit(const std::string& name, const Vec2& position);

	/**
	 *����ֲ������ֵ
	 */
	virtual void reducePlantHealthPoint(int number) { _healthPoint -= number; }

	/**
	 *�����ɽ�����
	 */
	virtual void setPlantSoilSplashAnimation(const float& scale);

	/**
	 *����ֲ��Ӱ��
	 */
	virtual void setPlantShadow(const float& scale);

	/**
	 *��ȡ��ʬ�Ƿ���ֲ��ǰ��
	 */
	virtual bool getZombieIsTheFrontOfPlant(Zombies* zombie) const;

	/**
	 *��ȡ��ʬ�Ƿ���ֲ����ͬһ��
	 */
	virtual bool getZombieIsSameLineWithPlant(Zombies* zombie) const;

	/**
	 *��ȡ��ʬ�Ƿ�����ֲ��
	 */
	virtual bool getzombieIsEncounterPlant(Zombies* zombie) const;

	/**
	 *��ʬ��ֲ��
	 */
	virtual void zombieEatPlant(Zombies* zombie);

	/**
	 *��ʬ�ӳ�ֲ���лָ�
	 */
	virtual void zombieRecoveryMove(Zombies* zombie);

protected:
	int _sunNeed;                               // ��������
	int _costGold;                              // ���ѽ��
	int _costMasonry;                           // ����שʯ
	int _combatEffecttiveness;                  // ս����
	int _plantTag;                              // ֲ���ǩ
	int _zOrder;                                // ����˳��
	int _plantNumber;                           // �洢ֲ���ţ�Ψһ�� ��
	bool _isLoop;                               // �Ƿ�ѭ��
	bool _isCanDelete[2];                       // �Ƿ����ɾ��
	float _coolDownTime;                        // ��ȴʱ��
	float _healthPoint;                         // ����ֵ
	float _totalHealthPoint;                    // ������ֵ
	Vec2 _position;                             // λ��
	Vec2 _rowAndColumn;                         // ��ϸ��ַ,��������
	SkeletonAnimation* _plantAnimation;         // ֲ�ﶯ��
	Node* _node;                                // �ڵ�
	Global* _global;                            // ȫ�ֱ���
	Sprite* _plantImage;                        // ֲ��ͼƬ
	Sprite* _plantShadow;                       // ֲ��Ӱ��
	PlantsType _plantsType;                     // ֲ������
	SkeletonAnimation* _soilSplashAnimation;    // �����ɽ�����
	static int plantNumber;                     // ֲ���ţ�Ψһ�ԣ�
};