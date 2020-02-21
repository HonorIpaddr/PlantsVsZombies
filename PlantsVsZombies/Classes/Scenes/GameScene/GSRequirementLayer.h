/**
 *Copyright (c) 2020 LZ.All Right Reserved
 *Author : LZ
 *Date: 2020.2.19
 *Email: 2117610943@qq.com
 */

#pragma once
#include "cocos2d.h"
#include "Based/UserWinRequirement.h"

using namespace cocos2d;

class GSRequirementLayer :public Layer
{
public:
    CREATE_FUNC(GSRequirementLayer);
    static Layer* addLayer();
    bool init();
    void keyboardControl();

CC_CONSTRUCTOR_ACCESS:
    GSRequirementLayer() : _requirement(nullptr) {}
    ~GSRequirementLayer() { delete _requirement; }

private:
    void showRequirement();

private:
    UserWinRequirement* _requirement;
};

/**
 * �ո������ͣ/�ָ�����
 */
extern int pressKeySpace;