/**
 *Copyright (c) 2020 LZ.All Right Reserved
 *Author : LZ
 *Date: 2020.2.11
 *Email: 2117610943@qq.com
 */

#include "GSData.h"
 #include "GSBackgroundLayer.h"
 #include "GSInformationLayer.h"
 #include "GSButtonLayer.h"
 #include "GSControlLayer.h"
 #include "GSAnimationLayer.h"
 
 //#include "Plants/Bullet.h"
// #include "Based/Car.h"

GSBackgroundLayer* backgroundLayerInformation;   // ������ʵ����
GSInformationLayer* informationLayerInformation; // ��Ϣ��ʵ����
GSButtonLayer* buttonLayerInformation;           // ��ť��ʵ����
GSControlLayer* controlLayerInformation;         // ���Ʋ�ʵ����
GSAnimationLayer* animationLayerInformation;     // ������ʾ����

multimap<int, Plants*> PlantsGroup;
list<Zombies*> ZombiesGroup;
list<Bullet*> BulletGroup;
list<Sun*> SunsGroup;
list<Car*> CarsGroup;
