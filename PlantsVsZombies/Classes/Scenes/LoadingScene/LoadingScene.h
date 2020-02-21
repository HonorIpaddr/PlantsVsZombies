/**
 *Copyright (c) 2019 LZ.All Right Reserved
 *Author : LZ
 *Date: 2019.7.15
 *Emal: 2117610943@qq.com
 */
#pragma once
#include "cocos2d.h"
#include "Scenes/MainMenuScene/MainMenu.h"
#include "ui/CocosGUI.h"
#include "Based/GlobalVariable.h"

#ifdef WIN32
#pragma execution_character_set("utf-8")
#endif

using namespace cocos2d;
using namespace cocos2d::ui;
using namespace cocos2d::experimental;

class LoadingScene :public Scene
{
public:
	static Scene* createLaodingScene();
	CREATE_FUNC(LoadingScene); 

CC_CONSTRUCTOR_ACCESS:
	LoadingScene();
	~LoadingScene();
	virtual bool init() override; 

private:
	int _textNumbers;                               /* �ı��� */
	int _loadFileNumbers;                           /* �ļ������� */
	int _allFileNumbers;                            /* �ļ�����(ͼƬ�����֣��������ı�) */
	bool _flowerVisible[5] = { false };             /* ���ػ����Ƿ�ɼ� */
	float _loadingPrecent;                          /* ���ؽ��� */
	Sprite* _sprite[8];                             /* ����ͼƬ */
	MenuItemLabel* _label;                          /* ���ֱ�ǩ */
	LoadingBar* _loadingBar;                        /* ������ */
	EventListenerTouchOneByOne* _listener;          /* ���ؼ��� */
	Global* _global;                                /* ȫ�ֱ������� */
	Director* _director;                            /* ���ݵ��� */
	FileUtils* _files;                              /* �ļ����� */

private:

	void update(float Time) override;                 /* ��ʱ�� */
	void setSystem();                                 /* ����ϵͳ���� */
	void calculateFileNumbers();                      /* �����ļ����� */

	void loadUserData();                              /* �����û����� */
	void loadingText();                               /* �������� */
	void loadingImage();                              /* ����ͼƬ */
	void loadingMusic();                              /* �������� */
	void loadingAnimation();                          /* ���ض��� */

	void showLoadingBackGround();                     /* չʾ���� */
	void showTileAndLoadingBar();                     /* չʾ������ */
	void showLoadingBarFlower(const int &ID);         /* չʾ�������ϵĻ� */
	void beginLoadingImageAndMusic();                 /* ��ʼ����ͼƬ������ */

	void runLoGoCallBack(Node* node,const int &ID);   /* չʾlogo�ص� */
	void loadingTextCallBack();                       /* �������ֻص� */
	void loadingImageCallBack(Ref* pSender);          /* ����ͼƬ�ص� */
	void loadingMusicCallBack(bool isSucceed);        /* �������ֻص� */
	void loadingAnimationCallBack();                  /* ���ض����ص� */
	void beginGameCallBack(Ref* pSender);             /* ��ʼ��Ϸ�ص� */

	int openResourcesPath(map<string, string>& Path, const std::string& xml, bool IsEncryption = false);  /* ����Դ·�� */
};