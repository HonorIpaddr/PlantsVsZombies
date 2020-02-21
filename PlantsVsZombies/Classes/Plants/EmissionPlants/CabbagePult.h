/**
 *Copyright (c) 2020 LZ.All Right Reserved
 *Author : LZ
 *Date: 2020.2.6
 *Emal: 2117610943@qq.com
 */

#pragma once
#include "EmissionPlants.h"

class Bullet;

class CabbagePult :public EmissionPlants
{
public:
	static CabbagePult* create(Node* node = nullptr);
	Sprite* createPlantImage() override;
	void createPlantAnimation() override;
	void determineRelativePositionPlantsAndZombies() override;

CC_CONSTRUCTOR_ACCESS:
	CabbagePult(Node* node = nullptr);
	~CabbagePult();

private:
	void plantAttack(Zombies* zombie) override;
	void createCabbage();

private:
	bool _isCreateCabbage;    // �Ƿ񴴽��ݾ��Ĳ��ӵ�
	bool _isChanged;          // �Ƿ�ı�Ϊ������̬
	float _distance;          // ֲ��������Ľ�ʬ�ľ���
	Vec2 _zombiePostion;      // ��ʱ��ѡ�����Ľ�ʬ��λ��
	Zombies* _zombie;         // �洢��ʱ��ѡ�����Ľ�ʬ
	Bullet* _cabbage;         // ���Ĳ�
};