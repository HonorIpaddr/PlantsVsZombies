/**
 *Copyright (c) 2020 LZ.All Right Reserved
 *Author : LZ
 *Date: 2020.2.6
 *Email: 2117610943@qq.com
 */

#pragma once
#include "Plants/Plants.h"

class Bullet;

class Torchwood :public Plants
{
public:
	static Torchwood* create(Node* node = nullptr);
	Sprite* createPlantImage() override;
	void createPlantAnimation() override;
	void determineRelativePositionPlantsAndZombies() override;

CC_CONSTRUCTOR_ACCESS:
	Torchwood(Node* node = nullptr);
	~Torchwood();

private:
	void judgeTorchwoodAndPeaPosition();                   // �㶹������׮λ���ж�
	bool getPeaIsSameLineWithTorchwood(Bullet* bullet);    // ��ȡ�㶹������׮�Ƿ���ͬһ��
	bool getPeaIsEncounterTorchwood(Bullet* bullet);       // ��ȡ�㶹������׮�Ƿ�����
	bool getBulletIsPea(Bullet* bullet);                   // ��ȡ�ӵ��Ƿ����㶹
};