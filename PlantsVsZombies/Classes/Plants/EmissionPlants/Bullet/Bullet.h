/**
 *Copyright (c) 2020 LZ.All Right Reserved
 *Author : LZ
 *Date: 2020.2.11
 *Email: 2117610943@qq.com
 */

#pragma once
#include "cocos2d.h"
#include "spine/spine-cocos2dx.h"
#include "Based/GlobalVariable.h"

using namespace spine;
using namespace cocos2d;

class Zombies;

enum class BulletType
{
	None = 0,
	Pea,
	FirePea,
	Cabbage,
	AcidLemonBullet
};

enum class SoundEffectType
{
	kernelpult = 0,
	shieldhit,
	firepea,
};

class Bullet
{
public:
	/**
	 *�����ӵ�
	 */
	virtual void createBullet() = 0;

	/**
	 *�ӵ��뽩ʬ��ײ���
	 */
	virtual void bulletAndZombiesCollision() = 0;

	/**
	 *��ȡ�ӵ�����
	 */
	virtual SkeletonAnimation* getBullet() const;

	/**
	 *�����ӵ���λ��
	 */
	virtual void setBulletPosition(const Vec2& position);

	/**
	 *�����ӵ�������
	 */
	virtual void setBulletName(const string& name);

	/**
	 *�����ӵ��Ƿ�ʹ��
	 */
	virtual void setBulletIsUsed(const bool isUsed);

	/**
	 *�����ӵ�������
	 */
	virtual void setBulletAttack(const float attack);

	/**
	 *�����ӵ�͸����
	 */
	virtual void setBulletOpacity(GLubyte opacity);
	virtual void setBulletOpacity();

	/**
	 *��ȡ�ӵ��Ƿ�ʹ��
	 */
	virtual bool getBulletIsUsed() const;

	/**
	 *��ȡ�ӵ�������
	 */
	virtual int getBulletAttack() const;

	/**
	 *��ȡ�ӵ�λ��
	 */
	virtual Vec2 getBulletPosition() const;

	/**
	 *��ȡ�ӵ�Xλ��
	 */
	virtual float getBulletPositionX() const;

	/**
	 *��ȡ�ӵ�Yλ��
	 */
	virtual float getBulletPositionY() const;

	/**
	 *��ȡ�ӵ�����
	 */
	virtual BulletType getBulletType() const;

	/**
	 *��ȡ�ӵ��Ƿ��뽩ʬ��ͬһ��
	 */
	template<typename T>
	bool getBulletIsSameLineWithZombie(T& zombie) {
		return fabs(_bulletAnimation->getPositionY() - 83 - zombie->getZombiePositionY()) <= 10 ? true : false;
	}

	/**
	 *��ȡ�ӵ��Ƿ��뽩ʬ����
	 */
	template<typename T>
	bool getBulletIsEncounterWithZombie(T& zombie) {
		return fabs(_bulletAnimation->getPositionX() - zombie->getZombiePositionX()) <= 25 ? true : false;
	}

	/*
	 *��ȡ�ӵ�������
	 */
	virtual string& getBulletName();

	/**
	 *��ȡ�ӵ��Ƿ���ʾ
	 */
	virtual bool getBulletVisible() const;

	/**
	 *�ӵ�������ʬ
	 */
	virtual void bulletAttackHurtZombies(Zombies* zombie);

	/**
	 *ը����ը�˺�
	 */
	virtual void bulletExplodeHurtZombies(Zombies* zombie);

	/**
	 *��̬����ɾ���ӵ�
	 */
	static void bulletDeleteUpdate(list<Bullet*>::iterator& bullet);

CC_CONSTRUCTOR_ACCESS:
	Bullet();
	~Bullet();

protected:
	virtual void bulletInit() = 0;
	virtual void createShadow() = 0;
	virtual float getZOrder(const int& positionY) const;

private:
	void releaseBullet() const;

protected:
	SkeletonAnimation* _bulletAnimation;  // ����
	float _attack;                        // ������
	bool _isUsed;                         // �Ƿ�ʹ��
	Node* _node;                          // ���ڵ�
	Vec2 _position;                       // λ��
	string _bulletName;                   // �㶹����
	BulletType _bulletType;               // �ӵ�����
	Global* _global;                      // ȫ�ֵ���
};