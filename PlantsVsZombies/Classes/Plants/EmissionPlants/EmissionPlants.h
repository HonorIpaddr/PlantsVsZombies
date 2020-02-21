/**
 *Copyright (c) 2020 LZ.All Right Reserved
 *Author : LZ
 *Date: 2020.2.6
 *Email: 2117610943@qq.com
 */

#pragma once
#include "../Plants.h"

class Zombies;
class Bullet;

class EmissionPlants :public Plants
{
CC_CONSTRUCTOR_ACCESS:
    EmissionPlants();
    ~EmissionPlants();

protected:
    virtual void plantAttack(Zombies* zombie) = 0;

protected:
    bool _isChanged;           // �Ƿ�ı�Ϊ������̬
    Bullet* _bulletAnimation;  // �ӵ�����
};