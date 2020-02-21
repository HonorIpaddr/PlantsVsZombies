/**
 *Copyright (c) 2019 LZ.All Right Reserved
 *Author : LZ
 *Date: 2019.7.15
 *Emal: 2117610943@qq.com
 */

#include "AudioEngine.h"
#include "Based/LevelData.h"
#include "LoadingScene.h"
#include "tinyxml2/tinyxml2.h"

#define MYDEBUG 0

LoadingScene::LoadingScene():
	_loadFileNumbers(0),
	_textNumbers(0),
	_allFileNumbers(0),
	_loadingPrecent(0),
	_label(nullptr),
	_loadingBar(nullptr),
	_listener(nullptr),
	_global(Global::getInstance()),
	_director(Director::getInstance()),
	_files(FileUtils::getInstance())
{
}

LoadingScene::~LoadingScene()
{
}

Scene* LoadingScene::createLaodingScene()
{
	return LoadingScene::create();
}

bool LoadingScene::init()
{
	if (!Scene::init())
	{
		return false;
	}
	
	this->calculateFileNumbers();  /* �����ļ����� */
	this->setSystem();             /* ����ϵͳ���� */
	this->loadUserData();          /* �����û���Ϣ */
	this->showLoadingBackGround(); /* չʾ���ؽ��� */
	
	return true;
}

void LoadingScene::setSystem()
{
	/* ���ù�� */
	_director->getOpenGLView()->setCursor(_global->userInformation->getImagePath().find("cursor")->second, Point::ANCHOR_TOP_LEFT);

	/* ����ͼ�� */
	_director->getOpenGLView()->setIcon(_global->userInformation->getImagePath().find("PlantsVsZombies")->second);

}

void LoadingScene::loadUserData()
{
	auto userdefault = UserDefault::getInstance();

	_global->userInformation->setKillZombiesNumbers(userdefault->getIntegerForKey("KILLALLZOMBIES")); /* ɱ����ʬ�� */
	_global->userInformation->setUsePlantsNumbers(userdefault->getIntegerForKey("USEPLANTSNUMBERS")); /* ʹ��ֲ������ */
	_global->userInformation->setBreakThroughnumbers(userdefault->getIntegerForKey("BREAKTHROUGH"));  /* ����ʧ�ܸ��� */
	_global->userInformation->setUserCaveFileNumber(userdefault->getIntegerForKey("USERDATANUMBER")); /* �浵��� */
	_global->userInformation->setIsShowEggs(userdefault->getBoolForKey("ISBEGINSHOWEGGS"));           /* ��ʾ�ʵ� */
	_global->userInformation->setCoinNumbers(userdefault->getIntegerForKey("COINNUMBERS"));           /* ����� */
	_global->userInformation->setSoundEffectVolume(userdefault->getFloatForKey("SOUNDEFFECT"));       /* ��Ч */
	_global->userInformation->setBackGroundMusicVolume(userdefault->getFloatForKey("GLOBALMUSIC"));   /* ���� */

	/* ��ȡ�û��浵���� */
	for (int i = 0; i < 8; i++)
	{
		if (!userdefault->getStringForKey(_global->userInformation->getUserCaveFileNameKey(i).c_str()).size())
		{
			_global->userInformation->setUserCaveFileName(i, "δ�����浵");
		}
		else
		{
			_global->userInformation->setUserCaveFileName(i, userdefault->getStringForKey(_global->userInformation->getUserCaveFileNameKey(i).c_str()));
		}
	}

	/* �û����� */
	_global->userInformation->setUserName(_global->userInformation->getUserCaveFileName(_global->userInformation->getUserCaveFileNumber()));
	_global->userInformation->setIsUpdate(true);

	/* ��ʾ��Ϣ */
	switch (userdefault->getBoolForKey("SHOWINFORMATION"))
	{
	case true:
		_director->setDisplayStats(true);
		_global->userInformation->setIsShowInformation(cocos2d::ui::CheckBox::EventType::SELECTED);
		break;
	case false:
		_director->setDisplayStats(false);
		_global->userInformation->setIsShowInformation(cocos2d::ui::CheckBox::EventType::UNSELECTED);
		break;
	}

	/* �Ƿ�ȫ�� */
	switch (userdefault->getBoolForKey("SHOWFULLSCREEN"))
	{
	case true:
		((GLViewImpl*)_director->getOpenGLView())->setFullscreen();
		_global->userInformation->setIsSelectFullScreen(cocos2d::ui::CheckBox::EventType::SELECTED);
		break;
	case false:
		((GLViewImpl*)_director->getOpenGLView())->setWindowed(1280, 720);
		_global->userInformation->setIsSelectFullScreen(cocos2d::ui::CheckBox::EventType::UNSELECTED);
		break;
	}
	
	/* �Ƿ��֡�� */
	switch (userdefault->getBoolForKey("SHOWHIGHFPS"))
	{
	case true:
		_director->setAnimationInterval(1.0f / 60);
		_global->userInformation->setIsSelectHighFPS(cocos2d::ui::CheckBox::EventType::SELECTED);
		_global->userInformation->setFps(60);
		break;
	case false:
		_director->setAnimationInterval(1.0f / 45);
		_global->userInformation->setIsSelectHighFPS(cocos2d::ui::CheckBox::EventType::UNSELECTED);
		_global->userInformation->setFps(45);
		break;
	}

	/* �Ƿ�������ʾ */
	switch (userdefault->getBoolForKey("STRETCHINGSHOW"))
	{
	case true:
		_global->userInformation->setIsSelectStretchingShow(cocos2d::ui::CheckBox::EventType::SELECTED);
		_director->getOpenGLView()->setDesignResolutionSize(_director->getWinSize().width, _director->getWinSize().height, ResolutionPolicy::EXACT_FIT);
		break;
	case false:
		_global->userInformation->setIsSelectStretchingShow(cocos2d::ui::CheckBox::EventType::UNSELECTED);
		_director->getOpenGLView()->setDesignResolutionSize(_director->getWinSize().width, _director->getWinSize().height, ResolutionPolicy::SHOW_ALL);
		break;
	}

	/* �Ƿ�������� */
	switch (userdefault->getBoolForKey("CURSORHIDE"))
	{
	case true:
		_global->userInformation->setIsSelectCursorNotHide(cocos2d::ui::CheckBox::EventType::SELECTED);
		break;
	case false:
		_global->userInformation->setIsSelectCursorNotHide(cocos2d::ui::CheckBox::EventType::UNSELECTED);
		break;
	}
}

void LoadingScene::showLoadingBackGround()
{
	/* �������� */
	_global->changeBgMusic("mainmusic", true);
	
	/* ��ȡ���ڴ�С */
	auto const size = Director::getInstance()->getWinSize();

	/* �������� */
	_sprite[0] = Sprite::create(_global->userInformation->getImagePath().find("PopCap_Logo1")->second);
	_sprite[1] = Sprite::create(_global->userInformation->getImagePath().find("PopCap_Logo")->second);
	_sprite[7] = Sprite::create("resources/Text/txt/About.txt");
	_sprite[2] = Sprite::create(_global->userInformation->getImagePath().find("titlescreen")->second);

	/* Ϊ������������ */
	_sprite[0]->setName("0");
	_sprite[1]->setName("1");
	_sprite[7]->setName("7");
	
	/* ���þ����С */
	_sprite[0]->setScale(1.5f);
	_sprite[1]->setScale(1.5f);
	_sprite[2]->setContentSize(size);
	_sprite[7]->setScale(1.5f);

	/* ���þ����λ�� */
	_sprite[0]->setPosition(size / 2);
	_sprite[1]->setPosition(size / 2);
	_sprite[7]->setPosition(size / 2);
	_sprite[2]->setPosition(size / 2);

	/* ���þ���͸���� */
	_sprite[0]->setOpacity(0);
	_sprite[1]->setOpacity(0);
	_sprite[7]->setOpacity(0);
	_sprite[2]->setOpacity(0);

	/* �Ѿ�����ص������� */
	this->addChild(_sprite[0]);
	this->addChild(_sprite[1]);
	this->addChild(_sprite[7]);
	this->addChild(_sprite[2]);

	/* ���þ��鲻�ɼ� */
	_sprite[1]->setVisible(false);
	_sprite[7]->setVisible(false);
	_sprite[2]->setVisible(false);

	/* ���þ��鶯�� */
	_sprite[0]->runAction(Sequence::create(FadeIn::create(2.0f), FadeOut::create(2.0f), CallFuncN::create(CC_CALLBACK_1(LoadingScene::runLoGoCallBack, this, 1)), NULL));
}

void LoadingScene::runLoGoCallBack(Node* node, const int &ID)
{
	switch (ID)
	{
	case 1:
		this->removeChildByName("0"); /* �ӳ������Ƴ�����Ϊ0�ĺ��� */
		_sprite[1]->setVisible(true);  /* ���þ���1�ɼ� */
		_sprite[1]->runAction(Sequence::create(FadeIn::create(2.0f), FadeOut::create(2.0f), CallFuncN::create(CC_CALLBACK_1(LoadingScene::runLoGoCallBack, this, 5)), NULL));
		break;
	case 2:
		this->removeChildByName("7"); /* �ӳ������Ƴ�����Ϊ7�ĺ��� */
		_sprite[2]->setVisible(true);  /* ���þ���2�ɼ� */
		_sprite[2]->runAction(Sequence::create(FadeIn::create(2.0f),CallFuncN::create(CC_CALLBACK_1(LoadingScene::runLoGoCallBack, this, 3)), NULL));
		break;
	case 3:
		this->showTileAndLoadingBar(); /* չʾ����ͽ����� */
		break;
	case 4:
		this->beginLoadingImageAndMusic(); /* ��ʼ����ͼƬ������ */
		break;
	case 5:
		this->removeChildByName("1"); /* �ӳ������Ƴ�����Ϊ1�ĺ��� */
		_sprite[7]->setVisible(true);  /* ���þ���7�ɼ� */
		_sprite[7]->runAction(Sequence::create(FadeIn::create(2.0f), FadeOut::create(4.0f), CallFuncN::create(CC_CALLBACK_1(LoadingScene::runLoGoCallBack, this, 2)), NULL));
		break;
	}
}

void LoadingScene::showTileAndLoadingBar()
{
	/* ��ȡ���ڴ�С */
    auto const size = _director->getWinSize();

	/* �������� */
	_sprite[3] = Sprite::create(_global->userInformation->getImagePath().find("PvZ_Logo")->second);
	_sprite[4] = Sprite::create(_global->userInformation->getImagePath().find("LoadBar_dirt")->second);
	_sprite[5] = Sprite::create(_global->userInformation->getImagePath().find("SodRollCap")->second);
	_sprite[6] = Sprite::create(_global->userInformation->getImagePath().find("spark")->second);

	/* �����ü��ڵ� */
	auto clippingNode = ClippingNode::create();

	/* ��ȡ����������Ĵ�С */
	Size const SpriteSize = _sprite[3]->getContentSize();

	/* Ϊ�������ó�ʼλ�� */
	_sprite[4]->setPosition(Vec2(size.width / 2, -100));
	_sprite[5]->setPosition(Vec2(10, 95));
	_sprite[6]->setPosition(Vec2(-SpriteSize.width, 0));
	clippingNode->setPosition(Vec2(size.width / 2 + 10, 1100));

	/* Ϊ�������ô�С */
	_sprite[3]->setScale(2.0f);
	_sprite[3]->setScaleX(2.5f);
	_sprite[4]->setScale(2.0f);
	_sprite[6]->setScale(2.0f);

	/* �þ����˶����� */
	_sprite[4]->runAction(Sequence::create(MoveTo::create(0.5f, Vec2(size.width / 2, 150)),CallFuncN::create(CC_CALLBACK_1(LoadingScene::runLoGoCallBack, this, 4)), NULL));
	_sprite[6]->runAction(RepeatForever::create(Sequence::create(MoveTo::create(1.0f, Vec2(SpriteSize.width, 0)), DelayTime::create(2.0f), CallFunc::create([=]() {_sprite[6]->setPosition(Vec2(-SpriteSize.width, 0)); }), NULL)));
    clippingNode->runAction(MoveTo::create(0.5f, Vec2(size.width / 2, 900)));

	/* �ӵ������� */
	this->addChild(_sprite[4]);
	this->addChild(clippingNode);
	_sprite[4]->addChild(_sprite[5],1);
	
	/* ���òü��ڵ�Ļ������� */
	clippingNode->setAlphaThreshold(0.01f);    //����alphaբֵ
	clippingNode->setContentSize(SpriteSize);  //���óߴ��С
	clippingNode->setStencil(_sprite[3]);      //����ģ��stencil
	clippingNode->addChild(_sprite[3]);        //����ӱ���,����ȫ��ʾ����,��Ϊ��ģ��һ����С
	clippingNode->addChild(_sprite[6]);        //�ᱻ�ü�

	/* �����˵���ǩ */
	_label = MenuItemLabel::create(Label::createWithTTF("������......", "resources/fonts/fzse_gbk.ttf", 20), CC_CALLBACK_1(LoadingScene::beginGameCallBack, this));
	_label->setColor(Color3B::YELLOW);
	_label->setEnabled(false);
	
	/* �����˵� */
	auto menu = Menu::create(_label, NULL);
	menu->setPosition(150, 55);
	_sprite[4]->addChild(menu);
}

void LoadingScene::beginLoadingImageAndMusic()
{
	/* ���������� */
	_loadingBar = LoadingBar::create(_global->userInformation->getImagePath().find("LoadBar_grass")->second);
	_loadingBar->setDirection(LoadingBar::Direction::LEFT); /* ���ü��ط��� */
	_loadingBar->setPosition(Vec2(150, 70));
	_sprite[4]->addChild(_loadingBar);

	/* ��ʱ�� */
	scheduleUpdate();

	loadingText(); /* �����ı� */
	
	loadingMusic();/* �������� */
	
	loadingAnimation(); /* ���ض��� */

	loadingImage();/* ����ͼƬ */	
}

void LoadingScene::update(float Time)
{
	if (_loadingPrecent <= 100) 
	{
		_loadingBar->setPercent(_loadingPrecent);              /* ���ü��ؽ��� */
		_sprite[5]->setScale(1 - _loadingPrecent / 170);       /* ���þ����С */
		_sprite[5]->setRotation(9 * _loadingPrecent);          /* ���þ���ת���� */
		_sprite[5]->setPosition(Vec2(10 + 290 / 100.0 * _loadingPrecent, 100 - _sprite[5]->getContentSize().height / 400 * _loadingPrecent));

		if (_loadingPrecent >= 20) showLoadingBarFlower(0);
		if (_loadingPrecent >= 40) showLoadingBarFlower(1);
		if (_loadingPrecent >= 60) showLoadingBarFlower(2);
		if (_loadingPrecent >= 80) showLoadingBarFlower(3);
		if (_loadingPrecent >= 100) showLoadingBarFlower(4);

	}
	if(_loadingPrecent >= 100) /* ���������� */
	{
		_label->setString(_global->userInformation->getGameText().find("�����ʼ")->second);  /* �������ñ�ǩ�������� */
		auto action = TintBy::create(0.5f, 0, 255, 255);
		_label->runAction(RepeatForever::create(Sequence::create(action,action->reverse(), nullptr)));
		_sprite[5]->setVisible(false);  /* ���þ���5Ϊ���ɼ� */
		_label->setEnabled(true);       /* ���ô������� */
	}
}

void LoadingScene::showLoadingBarFlower(const int &ID)
{
	srand(unsigned(time(nullptr)));
	if (!_flowerVisible[ID]) /* �������ID�ǲ��ɼ� */
	{
		/* ������ */
		auto flower = Sprite::create(_global->userInformation->getImagePath().find("sprout_petal")->second);
		flower->setScale(0.1f);
		flower->setPosition(Vec2(20 + (55 + rand() % 10)* ID, 85 + rand() % 10));
		flower->runAction(ScaleTo::create(0.2f, 0.5f));
		_sprite[4]->addChild(flower);
		if (ID == 4) /* ���ID==4 ������ʬͷ*/
		{
			AudioEngine::setVolume(AudioEngine::play2d(_global->userInformation->getMusicPath().find("loadingbar_zombie")->second, false), _global->userInformation->getSoundEffectVolume());
			flower->setTexture(_global->userInformation->getImagePath().find("ZombieHead")->second);
		}
		else  /* ���򴴽��� */
		{
			AudioEngine::setVolume(AudioEngine::play2d(_global->userInformation->getMusicPath().find("loadingbar_flower")->second, false), _global->userInformation->getSoundEffectVolume());
			flower->setRotation(rand() % 180);
		}
		
		_flowerVisible[ID] = true; /* ��ID����Ϊ�ɼ� */
	}
}

void LoadingScene::calculateFileNumbers()
{
#if MYDEBUG
	/* �ļ����� = �ı��� + ͼƬ�� + ������ + ������ */
	AllFileNumbers =
		openResourcesPath(_global->userInformation->getTextPath(), "resources/Text/TextPath.reanim.compiled", true) +
		openResourcesPath(_global->userInformation->getImagePath(), "resources/Text/ImagePath.reanim.compiled", true) +
		openResourcesPath(_global->userInformation->getMusicPath(), "resources/Text/MusicPath.reanim.compiled", true) +
		openResourcesPath_global->userInformation->getAnimationPath(), "resources/Text/AnimationPath.reanim.compiled", true);
#else
	_allFileNumbers =
		openResourcesPath(_global->userInformation->getTextPath(), "resources/Text/TextPath.xml", false) +
		openResourcesPath(_global->userInformation->getImagePath(), "resources/Text/ImagePath.xml", false) +
		openResourcesPath(_global->userInformation->getMusicPath(), "resources/Text/MusicPath.xml", false) +
		openResourcesPath(_global->userInformation->getAnimationPath(), "resources/Text/AnimationPath.xml", false);
#endif
}

int LoadingScene::openResourcesPath(map<string, string>& Path, const std::string& xml, bool IsEncryption)
{
	tinyxml2::XMLDocument* doc = new tinyxml2::XMLDocument();

	if (IsEncryption)
	{
		std::string textpath = _files->getStringFromFile(xml);

		char Encryption[200000] = { 0 }, Password[200000] = { 0 };
		strcpy(Encryption, textpath.c_str());

		OpenLevelData::getInstance()->decrypt(Encryption, Password);

		/* ����Դ·�� */
		doc->Parse(Password);
	}
	else
	{
		/* ����Դ·�� */
		doc->Parse(_files->getStringFromFile(xml).c_str());
	}

	auto root = doc->RootElement();
	for (auto e = root->FirstChildElement(); e; e = e->NextSiblingElement())
	{
		for (auto at = e->FirstAttribute(); at; at = at->Next())
		{
			Path.insert(pair<string, string>(at->Name(), at->Value()));
		}
	}
	return Path.size();
}

void LoadingScene::loadingText()
{
	/* ѭ�������ı� */
	for (auto& i : _global->userInformation->getTextPath())
	{
		ValueVector Text = _files->getValueVectorFromFile(i.second);
		for (auto& file : Text)
		{
			auto file_map = file.asValueMap();
			_global->userInformation->getGameText().insert(pair<string, string>(file_map.at("id").asString(), file_map.at("info").asString()));
		}
		/* ���ػص� */
		this->loadingTextCallBack();
	}
}

void LoadingScene::loadingImage()
{
    /* ѭ������ͼƬ */
	for (auto& i : _global->userInformation->getImagePath())
	{
		_director->getTextureCache()->addImageAsync(i.second, CC_CALLBACK_1(LoadingScene::loadingImageCallBack, this));
	}
}

void LoadingScene::loadingMusic()
{
	/* ѭ���������� */
	for (auto& i : _global->userInformation->getMusicPath())
	{
		AudioEngine::preload(i.second, CC_CALLBACK_1(LoadingScene::loadingMusicCallBack, this));
	}
}

void LoadingScene::loadingAnimation()
{
	/* ѭ�����ض��� */
	for (auto& i : _global->userInformation->getAnimationPath())
	{
		/* ��ʱ�洢�ļ����� */
		char JsonName[100], AtlasName[100];

		/* ת�� */
		sprintf(JsonName, "resources/Animations/Json/%s.json", (i.second).c_str());
		sprintf(AtlasName, "resources/Animations/Atlas/%s.atlas", (i.second).c_str());

		/* ���� */
		spSkeletonJson* json = spSkeletonJson_createWithLoader((spAttachmentLoader*)Cocos2dAttachmentLoader_create(spAtlas_createFromFile(AtlasName, nullptr)));
		auto skeletonData = spSkeletonJson_readSkeletonDataFile(json, JsonName);
		spSkeletonJson_dispose(json);

		/* �Ѽ��ص��Ķ�������map��*/
		_global->userInformation->getAnimationData().insert(pair<string, spSkeletonData*>(i.second, skeletonData));

		/* ���лص� */
		this->loadingAnimationCallBack();
	}
}

void LoadingScene::loadingTextCallBack()
{
	_loadFileNumbers++;     /* �ļ�����һ */
	_loadingPrecent = ((_loadFileNumbers * 1.0f) / _allFileNumbers) * 100;  /* ������صİٷֱ� */
}

void LoadingScene::loadingImageCallBack(Ref* pSender)
{
	_loadFileNumbers++;     /* �ļ�����һ */
	_loadingPrecent = ((_loadFileNumbers * 1.0f) / _allFileNumbers) * 100;  /* ������صİٷֱ� */
}

void LoadingScene::loadingMusicCallBack(bool isSucceed)
{
	if (isSucceed)/* ������سɹ� */
	{
		_loadFileNumbers++;     /* �ļ�����һ */
		_loadingPrecent = ((_loadFileNumbers * 1.0f) / _allFileNumbers) * 100;  /* ������صİٷֱ� */
	}
}

void LoadingScene::loadingAnimationCallBack()
{
	_loadFileNumbers++;     /* �ļ�����һ */
	_loadingPrecent = ((_loadFileNumbers * 1.0f) / _allFileNumbers) * 100;  /* ������صİٷֱ� */
}

void LoadingScene::beginGameCallBack(Ref* pSender)
{
	AudioEngine::setVolume(AudioEngine::play2d(_global->userInformation->getMusicPath().find("buttonclick")->second), _global->userInformation->getSoundEffectVolume());
	_director->replaceScene(MainMenu::createScene()); /* �л����� */
}