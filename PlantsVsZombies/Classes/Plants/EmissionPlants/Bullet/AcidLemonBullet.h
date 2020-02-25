/**
 *Copyright (c) 2020 LZ.All Right Reserved
 *Author : LZ
 *Date: 2020.2.25
 *Email: 2117610943@qq.com
 */

#pragma once
#include "Bullet.h"

class AcidLemonBullet :public Bullet
{
public:
	/**
	 *�����ӵ�
	 */
	virtual void createBullet() override;

	/**
	 *�ӵ��뽩ʬ��ײ���
	 */
	virtual void bulletAndZombiesCollision() override;
	 
	/**
	 *�ӵ���ʼ��
	 */
	virtual void bulletInit() override;

	/**
	 *�����ӵ�Ӱ��
	 */
	virtual void createShadow() override;

CC_CONSTRUCTOR_ACCESS:
	AcidLemonBullet(Node* node = nullptr);
	~AcidLemonBullet();

private:
	void createAcidLemonBulletExplode();
	void setAttackForShield(Zombies* zombie);
};