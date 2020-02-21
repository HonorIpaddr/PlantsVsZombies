/**
 *Copyright (c) 2020 LZ.All Right Reserved
 *Author : LZ
 *Date: 2020.2.6
 *Email: 2117610943@qq.com
 */

#pragma once
#include "BattlePlants.h"

class PotatoMine :public BattlePlants
{
public:
	static PotatoMine* create(Node* node = nullptr);
	Sprite* createPlantImage() override;
	void createPlantAnimation() override;
	void determineRelativePositionPlantsAndZombies() override;

CC_CONSTRUCTOR_ACCESS:
	PotatoMine(Node* node = nullptr);
	~PotatoMine();

private:
	void plantExplode() override;
	void zombieEatPlant(Zombies* zombie) override;
	void createListener();
	void explodeHurtZombies();
	bool getZombieIsInExplodeRange(Zombies* zombie);
	
private:
	bool _isReady;                 // �Ƿ�׼����
	bool _isExplodeFinished;       // �Ƿ�ը���
	bool _isBeginExplode;          // �Ƿ�ʼ��ը
	bool _isCanKillZombies;        // �Ƿ����ɱ�ֽ�ʬ
	bool _isKillZombiesFinished;   // �Ƿ�ɱ����ʬ����
	float _breakGround;            // ����ʱ��
};