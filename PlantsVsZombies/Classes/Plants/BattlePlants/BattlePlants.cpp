/**
 *Copyright (c) 2020 LZ.All Right Reserved
 *Author : LZ
 *Date: 2020.2.17
 *Email: 2117610943@qq.com
 */

#include "BattlePlants.h"

#include "Zombies/Zombies.h"

BattlePlants::BattlePlants()
{
}

BattlePlants::~BattlePlants()
{
}

void BattlePlants::hurtZombies(Zombies* zombie)
{
	if (zombie->getZombieCurrentBodyShieldVolume() < _combatEffecttiveness) /* �����ǰ���廤��Ѫ��С�ڱ�ը�˺� */
	{
		if (zombie->getZombieCurrentBodyShieldVolume() + zombie->getZombieCurrentHeadShieldVolume() < _combatEffecttiveness) /* �����ǰ���廤�ܼ�ͷ������Ѫ��С�ڱ�ը�˺� */
		{
			if (zombie->getZombieCurrentBodyShieldVolume() + zombie->getZombieCurrentHeadShieldVolume() + zombie->getZombieCurrentBloodVolume() <= _combatEffecttiveness) /* �����ʬ����Ѫ��С�ڱ�ը�˺�����ʬ������ */
			{
				/* ��ʬ���� */
				zombie->setZombieCurrentBloodVolume(0);
				zombie->setZombieCurrentHeadShieldVolume(0);
				zombie->setZombieCurrentBodyShieldVolume(0);
			}
			else
			{
				/* ���㽩ʬ����Ѫ�� */
				zombie->setZombieCurrentBloodVolume(zombie->getZombieCurrentBodyShieldVolume() + zombie->getZombieCurrentHeadShieldVolume() + zombie->getZombieCurrentBloodVolume() - _combatEffecttiveness);
				zombie->setZombieCurrentHeadShieldVolume(0);
				zombie->setZombieCurrentBodyShieldVolume(0);
			}
		}
		else
		{
			/* ���㽩ʬ����ʣ��Ѫ�� */
			zombie->setZombieCurrentHeadShieldVolume(zombie->getZombieCurrentBodyShieldVolume() + zombie->getZombieCurrentHeadShieldVolume() - _combatEffecttiveness);
			zombie->setZombieCurrentBodyShieldVolume(0);
		}
	}
	else
	{
		zombie->setZombieCurrentBodyShieldVolume(zombie->getZombieCurrentBodyShieldVolume() - _combatEffecttiveness);
	}
}