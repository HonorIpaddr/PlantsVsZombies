/**
 *Copyright (c) 2019 LZ.All Right Reserved
 *Author : LZ
 *Date: 2019.7.17
 *Modify: 2020.2.3
 *Email: 2117610943@qq.com
 */

#pragma once
#include "Based/Dialog.h"
#include "cocos2d.h"
#include "cocos-ext.h"
#include "Scenes/EasterEggsScene/GameEasterEggs.h"


#ifdef WIN32
#pragma execution_character_set("utf-8")
#endif

using namespace ui;
using namespace cocos2d;
using namespace cocos2d::experimental;
using namespace cocos2d::extension;

enum class OptionScene_CheckBox
{
	��ʾ��Ϣ = 1,
	ȫ��,
	��֡��,
	�������,
	������ʾ
};

enum class OptionScene_Slider
{
	���� = 1,
	��Ч
};

class OptionsMenu :public Dialog
{
public:
	CREATE_FUNC(OptionsMenu);
	virtual bool init() override;
	void addLayer(Node* node);

CC_CONSTRUCTOR_ACCESS:
	OptionsMenu();
	~OptionsMenu();
	
protected:
	virtual void createDialog();                                                                                      /* �����Ի��� */
	virtual CheckBox* createCheckBox(Vec2&, Vec2&, const std::string&, OptionScene_CheckBox, const std::string,
		const std::string, const bool IsNew = false);                                                                 /* ������ѡ�� */
	virtual ControlSlider* createSlider(Vec2&, Vec2&, const std::string&, OptionScene_Slider,const std::string,
		const std::string,const std::string, const bool IsNew = false);                                               /* ���������� */
	virtual ControlSlider* createSlider(Vec2&, Vec2&, const std::string&, OptionScene_Slider, Sprite*, Sprite*,
		Sprite*, const bool IsNew = false);                                                                           /* ���������� */
	virtual void deleteDialog() override;                                                                             /* ɾ���Ի��� */

private:
	void createButton();
	void backGroundMusicVolumeChangeCallBack(Ref* sender, Control::EventType type);                                  /* ���������������ڻص�����*/
	void soundEffectMusicVolumeChangeCallBack(Ref* sender, Control::EventType type);                                 /* ��Ч�����������ڻص�����*/

protected:
	Sprite* _option;         /* �Ի��� */
	Director* _director;

private:
	UserDefault* _userDefault;
};