/**
 *Copyright (c) 2019 LZ.All Right Reserved
 *Author : LZ
 *Date: 2019.8.12
 *Modify: 2020.2.3
 *Email: 2117610943@qq.com
 */

#include "AudioEngine.h"
#include "SPSSpriteLayer.h"
#include "spine/spine-cocos2dx.h"

using namespace spine;

SPSSpriteLayer::SPSSpriteLayer() :
	_seedChooser(Sprite::create()),
	_plantCardScrollView(ScrollView::create()),
	_plantCardTextScrollView(ScrollView::create()),
	_global(Global::getInstance()),
	_plantCardScrollViewPercent(0),
	_selectFinished(false)
{
}

SPSSpriteLayer::~SPSSpriteLayer()
{
}

bool SPSSpriteLayer::init()
{
	if (!Layer::init())return false;

	createSelectPlantsDialog();

	return true;
}

void SPSSpriteLayer::createSelectPlantsDialog()
{
	AudioEngine::setVolume(AudioEngine::play2d(_global->userInformation->getMusicPath().find("swing")->second), _global->userInformation->getSoundEffectVolume());

	alreadySelectPlantsDialog();

	alreadyHavePlantsDialog();

	createScrollview();

	createPlantsCards();

	showRandPlantsInformation();

	createMouseListener();
}

void SPSSpriteLayer::alreadySelectPlantsDialog()
{
	auto SeedBank = Scale9Sprite::create(_global->userInformation->getImagePath().find("SeedBank")->second);
	SeedBank->setPosition(Vec2(-200, 1080));
	SeedBank->setAnchorPoint(Vec2(0, 1));
	SeedBank->setCapInsets(Rect(0, 5, 87, 373));
	SeedBank->setContentSize(Size(217.5f, 980));
	SeedBank->runAction(MoveTo::create(0.2f, Vec2(0, 1080)));
	this->addChild(SeedBank);

	for (int i = 0; i < 9; ++i)
	{
		auto Seed = Sprite::create(_global->userInformation->getImagePath().find("SeedPacketSilhouette")->second);
		Seed->setContentSize(Size(180, 100));
		Seed->setPosition(Vec2(110, 905 - 103 * i));
		SeedBank->addChild(Seed);
	}
}

void SPSSpriteLayer::alreadyHavePlantsDialog()
{
	_seedChooser = Sprite::create(_global->userInformation->getImagePath().find("SeedChooser_Background")->second);
	_seedChooser->setPosition(Vec2(600, -600));
	_seedChooser->setContentSize(Size(780, 1080));
	_seedChooser->setName("_seedChooer");
	_seedChooser->runAction(Sequence::create(MoveTo::create(0.2f, Vec2(600, 540)), CallFunc::create([&]() {createBeginButton(); }), nullptr));
	this->addChild(_seedChooser);

	auto SelectBackground = Sprite::create(_global->userInformation->getImagePath().find("SelectBackground")->second);
	SelectBackground->setPosition(Vec2(190, 730));
	SelectBackground->setScaleY(0.96f);
	SelectBackground->setScaleX(1.14f);
	SelectBackground->setColor(Color3B(0, 255, 255));
	_seedChooser->addChild(SelectBackground);
	SelectBackground->runAction(RepeatForever::create(Sequence::create(FadeTo::create(0.2f, 150), FadeTo::create(0.2f, 50), nullptr)));

	auto State = SkeletonAnimation::createWithData(_global->userInformation->getAnimationData().find("Stage")->second);
	State->setPosition(Vec2(195, 720));
	State->setScale(3.5f);
	State->setAnimation(0, "Rotate", true);
	_seedChooser->addChild(State);

	auto SeedChooserText = ui::Text::create(_global->userInformation->getGameText().find("��ѡ�����ս��ֲ�")->second, "resources/fonts/fzse_gbk.ttf", 50);
	SeedChooserText->setColor(Color3B::YELLOW);
	SeedChooserText->setPosition(Vec2(400, 1000));
	_seedChooser->addChild(SeedChooserText);
}

void SPSSpriteLayer::createScrollview()
{
	/* ֲ�￨�ƻ�����ͼ */
	_plantCardScrollView->setDirection(ui::ScrollView::Direction::VERTICAL);
	_plantCardScrollView->setContentSize(Size(800.0f, 420));
	_plantCardScrollView->setPosition(Vec2(0, 120));
	_plantCardScrollView->setInnerContainerSize(Size(800, 400));
	//_plantCardScrollView->setBounceEnabled(true);
	_plantCardScrollView->setScrollBarPositionFromCorner(Vec2(40, 0));
	_plantCardScrollView->setScrollBarWidth(7);
	_plantCardScrollView->setScrollBarAutoHideTime(2.0f);
	_plantCardScrollView->setScrollBarColor(Color3B::WHITE);
	//this->schedule(schedule_selector(SPSSpriteLayer::scrollViewUpdate), 0.1f);
	_seedChooser->addChild(_plantCardScrollView);

	/* ֲ�￨�ƽ��ܻ�����ͼ */
	_plantCardTextScrollView = ui::ScrollView::create();
	_plantCardTextScrollView->setDirection(ui::ScrollView::Direction::VERTICAL);
	_plantCardTextScrollView->setContentSize(Size(400.0f, 360.0f));
	_plantCardTextScrollView->setInnerContainerSize(Size(400, 620));
	_plantCardTextScrollView->setPosition(Vec2(360, 580));
	_plantCardTextScrollView->setBounceEnabled(true);
	_plantCardTextScrollView->setScrollBarPositionFromCorner(Vec2(10, 0));
	_plantCardTextScrollView->setScrollBarAutoHideTime(2.0f);
	_plantCardTextScrollView->setScrollBarWidth(7);
	_plantCardTextScrollView->setScrollBarColor(Color3B::WHITE);
	_seedChooser->addChild(_plantCardTextScrollView);
}

void SPSSpriteLayer::scrollViewUpdate(float Time)
{
	_plantCardScrollViewPercent = (_plantCardScrollView->getScrolledPercentVertical()) * 25;
}

void SPSSpriteLayer::createMouseListener()
{
	/* ��껬������ */
	auto mouse = EventListenerMouse::create();
	mouse->onMouseScroll = [&](Event* event)
	{
		auto e = (EventMouse*)event;
		auto cur = e->getLocationInView();
		auto a = e->getScrollY();
		cur.x -= 200;

		if (_plantCardScrollView->getBoundingBox().containsPoint(cur))
		{
			if (_plantCardRollingDistance + a >= 0 && _plantCardRollingDistance + a <= 50)
			{
				_plantCardRollingDistance += a;
			}
			if (_plantCardRollingDistance < 0)_plantCardRollingDistance = 0;
			if (_plantCardRollingDistance > 50)_plantCardRollingDistance = 50;
			_plantCardScrollView->jumpToPercentVertical(_plantCardRollingDistance * 2);
		}
	};
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(mouse, this);
}

void SPSSpriteLayer::createPlantsCards()
{
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			if (4 * i + j < 11)
			{
				createButtons(Vec2(110 + 185 * j, 490 - 105 * i), 4 * i + j);
			}
		}
	}
}

Button* SPSSpriteLayer::createButtons(const Vec2& vec2, const int& id)
{
	auto button = Button::create(_global->userInformation->getImagePath().find("SeedPacket_Larger")->second);
	button->setPosition(Vec2(vec2.x, _plantCardScrollView->getInnerContainerSize().height + vec2.y - 540));
	button->setTitleColor(Color3B::RED);
	button->setTitleFontSize(25);
	_plantCardScrollView->addChild(button);

	button->addTouchEventListener([=](Ref* sender, ui::Widget::TouchEventType type)
		{
			switch (type)
			{
			case ui::Widget::TouchEventType::BEGAN:
				AudioEngine::setVolume(AudioEngine::play2d(_global->userInformation->getMusicPath().find("tap")->second), _global->userInformation->getSoundEffectVolume());
				break;
			case ui::Widget::TouchEventType::ENDED:

				if (seedBankButton.size() > 3)/* 583 */
				{
					((Button*)_seedChooser->getChildByName("Begin"))->setEnabled(true);
					((Button*)_seedChooser->getChildByName("Begin"))->setTitleColor(Color3B::YELLOW);
				}
				if (seedBankButton.size() < 9)
				{
					button->setEnabled(false);
					button->setColor(Color3B(70, 70, 70));
					button->setCascadeColorEnabled(true);  /* ���ø��ڵ�Ӱ���ӽڵ� */

					createMoveButton(button, vec2, id);
				}

				createAnimationAndText(id);//����ֲ�ﶯ�� 
				break;
			}
		});

	/* ��ʾֲ����Ϣ */
	showPlantsInformation(button, id);

	return button;
}

void SPSSpriteLayer::showRandPlantsInformation()
{
	preLoadText();

	/* ��ʼ���봴��������� */
	srand(time(nullptr));
	createAnimationAndText(rand() % 11);
}

void SPSSpriteLayer::preLoadText()
{
	for (int i = 0; i < 11; i++)
	{
		_animationText[i] = Text::create("", "resources/fonts/fzse_gbk.ttf", 30);
		_animationText[i]->setName("AnimationText");
		_animationText[i]->enableOutline(Color4B(0, 255, 255, 255));
		_animationText[i]->enableShadow(Color4B::GRAY, Size(4, -4), 1);
		switch (i)
		{
		case 0:
			_seedChooser->addChild(_animationText[i]);
			break;
		default:
			_plantCardTextScrollView->addChild(_animationText[i]);
			break;
		}
	}
}

Text* SPSSpriteLayer::showPlantsInformation(Button* button, const int id) const
{
	const string PlantsImageName[][5] =
	{
		{{"SunFlower_2"},{"50"},{"PlantsIcon4"}},{{"PeaShooter"},{"100"},{"PlantsIcon5"}},{{"Wallnut_body"},{"50"},{"PlantsIcon6"}},{{"CherryBomb"},{"150"},{"PlantsIcon7"}},
		{{"PotatoMine"},{"25"},{"PlantsIcon7"}},{{"Cabbage"},{"100"},{"PlantsIcon3"}},{{"Torchwood"},{"175"},{"PlantsIcon2"}},{{"Spikeweed"},{"100"},{"PlantsIcon12"}},
		{{"Garlic"},{"50"},{"PlantsIcon10"}},{{"FirePeaShooter"},{"200"},{"PlantsIcon2"}},{{"Jalapeno"},{"150"},{"PlantsIcon2"}}
	};
	Color3B color[] =
	{
		Color3B::YELLOW,Color3B::GREEN,Color3B(140,80,10),Color3B::RED,Color3B(250,75,10),Color3B(64,0,0),Color3B::RED,Color3B::BLACK,Color3B(128,0,128),Color3B::RED,
		Color3B::RED
	};

	/* ͼƬ */
	createPlantsImage(button, _global->userInformation->getImagePath().find(PlantsImageName[id][0])->second);

	auto PlantsLevelIamge = Sprite::create(_global->userInformation->getImagePath().find("PlantsLevel_Copper")->second);
	PlantsLevelIamge->setPosition(Vec2(155, 74));
	PlantsLevelIamge->setScale(0.65f);
	button->addChild(PlantsLevelIamge);

	auto Background = Sprite::create(_global->userInformation->getImagePath().find("PlantsIconBackground")->second);
	Background->setOpacity(0);
	button->addChild(Background);

	auto PlantsIconBackground = Sprite::create(_global->userInformation->getImagePath().find("PlantsIconBackground")->second);
	PlantsIconBackground->setPosition(Vec2(93, 150));
	PlantsIconBackground->setScale(0.28f);
	PlantsIconBackground->setColor(color[id]);
	Background->addChild(PlantsIconBackground);

	auto PlantsIcon = Sprite::create(_global->userInformation->getImagePath().find(PlantsImageName[id][2])->second);
	PlantsIcon->setPosition(Vec2(73, 75));
	PlantsIcon->setScale(1.2f);
	PlantsIconBackground->addChild(PlantsIcon);

	/* �ı� */
	auto PlantsNeedSuns = ui::Text::create();
	PlantsNeedSuns->setFontName("resources/fonts/arial.ttf");
	PlantsNeedSuns->setColor(Color3B::BLACK);
	PlantsNeedSuns->setAnchorPoint(Vec2(1, 0.5f));
	PlantsNeedSuns->setPosition(Vec2(175, 20));
	PlantsNeedSuns->setFontSize(30);
	PlantsNeedSuns->setString(PlantsImageName[id][1]);
	button->addChild(PlantsNeedSuns);

	auto PlantsLevelText = ui::Text::create();
	PlantsLevelText->setFontName("resources/fonts/GameFont.ttf");
	PlantsLevelText->setColor(Color3B::BLACK);
	PlantsLevelText->setAnchorPoint(Vec2(1, 0.5f));
	PlantsLevelText->setPosition(Vec2(177, 77));
	PlantsLevelText->setFontSize(20);
	PlantsLevelText->setString("LVL1");
	button->addChild(PlantsLevelText);

	return PlantsNeedSuns;
}

void SPSSpriteLayer::createPlantsImage(Button* button, const std::string& resource, const float& scale) const
{
	auto image = Sprite::create(resource);
	image->setScale(scale);
	image->setPosition(Vec2(55, 50));
	button->addChild(image);
}

void SPSSpriteLayer::createMoveButton(Button* button, const Vec2& vec2, const int& id)
{
	/* �����ƶ����� */
	auto MoveCard = ui::Button::create(_global->userInformation->getImagePath().find("SeedPacket_Larger")->second);
	MoveCard->setPosition(Vec2(vec2.x + 212, vec2.y + _plantCardScrollViewPercent));
	MoveCard->setTitleColor(Color3B::RED);
	MoveCard->setTitleFontSize(25);
	MoveCard->setTag(id);
	MoveCard->runAction(MoveTo::create(0.2f, Vec2(110, 1008 - 103 * seedBankButton.size())));
	this->addChild(MoveCard);

	showPlantsInformation(MoveCard, id); //��ʾ��Ϣ

	/* �洢���������� */
	UserSelectCard seed_bank_button;
	seed_bank_button.cardbutton = MoveCard;
	seed_bank_button.cardTag = id;
	seedBankButton.push_back(seed_bank_button);
	
	float PlantCardScrollViewPercentLast = _plantCardScrollViewPercent;
	MoveCard->addTouchEventListener([=](Ref* sender, ui::Widget::TouchEventType type)
		{
			switch (type)
			{
			case ui::Widget::TouchEventType::BEGAN:
				AudioEngine::setVolume(AudioEngine::play2d(_global->userInformation->getMusicPath().find("tap2")->second), _global->userInformation->getSoundEffectVolume());
				break;
			case ui::Widget::TouchEventType::ENDED:
				createAnimationAndText(id);  //����ֲ�ﶯ��
				sortPlantsCard(id);          //��ֲ�￨����������
				_plantCardScrollView->scrollToPercentVertical(PlantCardScrollViewPercentLast / 25.0f, 0.5f, true);//��������ʼλ��
				_plantCardRollingDistance = PlantCardScrollViewPercentLast / 25.0f;
				MoveCard->runAction(Sequence::create(MoveTo::create(0.2f, Vec2(vec2.x + 212, vec2.y + PlantCardScrollViewPercentLast)), CallFuncN::create(CC_CALLBACK_1(SPSSpriteLayer::removePlantsCardCallBack, this, id, button)), nullptr));//ֲ�￨��ȡ��ѡ��
				break;
			}
		});
}

void SPSSpriteLayer::sortPlantsCard(const int& id)
{
	/* ɾ��ȡ��ѡ��Ŀ��� */
	for (auto& card = seedBankButton.begin(); card != seedBankButton.end();)
	{
		if (card->cardTag == id)
		{
			card = seedBankButton.erase(card);
		}
		else
		{
			++card;
		}
	}

	/* Ϊֲ�￨����������λ�� */
	int i = -1;
	for (auto& card : seedBankButton)
	{
		card.cardbutton->runAction(MoveTo::create(0.2f, Vec2(110, 1008 - 103 * ++i)));
	}

	if (seedBankButton.size() <= 4)
	{
		((Button*)_seedChooser->getChildByName("Begin"))->setEnabled(false);
		((Button*)_seedChooser->getChildByName("Begin"))->setTitleColor(Color3B::WHITE);
	}
}


void SPSSpriteLayer::removePlantsCardCallBack(Node* node, const int& id, Button* button)
{
	/* ɾ���ƶ����ƾ��� */
	this->removeChildByTag(id);

	/* ���ÿ��ƾ�������ٴ�ѡ�� */
	button->setEnabled(true);
	button->setColor(Color3B::WHITE);
}

void SPSSpriteLayer::createBeginButton()
{
	auto button = Button::create(_global->userInformation->getImagePath().find("SeedChooser_Button_Disabled")->second, _global->userInformation->getImagePath().find("SeedChooser_Button")->second);
	button->setTitleText(_global->userInformation->getGameText().find("��ʼս���ɣ�")->second);
	button->setTitleColor(Color3B::WHITE);
	button->setTitleFontName("resources/fonts/fzse_gbk.ttf");
	button->setTitleFontSize(20);
	button->setPosition(Vec2(390, 70));
	button->setScale(2.0f);
	button->setEnabled(false);
	button->setName("Begin");
	button->addTouchEventListener([=](Ref* sender, ui::Widget::TouchEventType type)
		{
			switch (type)
			{
			case ui::Widget::TouchEventType::BEGAN:
				AudioEngine::setVolume(AudioEngine::play2d(_global->userInformation->getMusicPath().find("tap")->second), _global->userInformation->getSoundEffectVolume());
				break;
			case ui::Widget::TouchEventType::ENDED:
				AudioEngine::setVolume(AudioEngine::play2d(_global->userInformation->getMusicPath().find("swing")->second), _global->userInformation->getSoundEffectVolume());
				/* ���ð�ť������ */
				for (auto& card : seedBankButton)
				{
					card.cardbutton->setEnabled(false);
					card.cardbutton->setColor(Color3B::GRAY);
					card.cardbutton->setCascadeColorEnabled(true);
				}
				if (seedBankButton.size() > 4)
				{
					_seedChooser->runAction(Sequence::create(MoveTo::create(0.35f, Vec2(605, -600)), CallFuncN::create(CC_CALLBACK_1(SPSSpriteLayer::selectPlantsCallBack, this)), nullptr));
				}
				break;
			}
		});
	_seedChooser->addChild(button);
}

void SPSSpriteLayer::createPlantsAnimation(const std::string& filepath, const std::string& AnimationName, const std::string& skin, Vec2& vec2, const float& scale)
{
	/* �������� */
	auto iter = _global->userInformation->getAnimationData().find(filepath);
	if (iter != _global->userInformation->getAnimationData().end())/* ��������ҵ� */
	{
		_plantsAnimation = SkeletonAnimation::createWithData(iter->second);
		_plantsAnimation->setPosition(vec2);
		_plantsAnimation->setScale(scale);
		_plantsAnimation->setSkin(skin);
		_plantsAnimation->setAnimation(0, AnimationName, true);
		_plantsAnimation->setName("PlantsAnimation");
	}
}

void SPSSpriteLayer::createPlantsText(const int& ID, const std::string& name, const Vec2& vec2, const float& fontsize, Color3B color, bool AnchorPoint)
{
	_animationText[ID]->setString(name);
	_animationText[ID]->setFontSize(fontsize);;
	_animationText[ID]->setColor(color);
	_plantCardTextScrollView->scrollToTop(0.1f, true);
	AnchorPoint ? _animationText[ID]->setAnchorPoint(Vec2(0.5f, 0.5f)) : _animationText[ID]->setAnchorPoint(Vec2(0, 1));
	switch (ID)
	{
	case 0:
		_animationText[ID]->setPosition(vec2);
		break;
	default:
		_animationText[ID]->setPosition(Vec2(vec2.x - _plantCardTextScrollView->getInnerContainerSize().width + 40, vec2.y + _plantCardTextScrollView->getInnerContainerSize().height - 1000));
		break;
	}
}

void SPSSpriteLayer::createAnimationAndText(const int& id)
{
	_seedChooser->removeChildByName("PlantsAnimation");
	for (int i = 0; i < 10; i++)_animationText[i]->setString("");

	switch (id)
	{
	case 0:
		_plantCardTextScrollView->setInnerContainerSize(Size(400, 430));
		this->createPlantsAnimation("SunFlower_2", "SunFlower_Normal", "SunFlower_Normal", Vec2(200, 610), 1.5f);
		this->createPlantsText(0, _global->userInformation->getGameText().find("SUNFLOWER_1")->second, Vec2(190, 910), 50);
		this->createPlantsText(1, _global->userInformation->getGameText().find("SUNFLOWER_2")->second, Vec2(360, 1000), 30, Color3B::YELLOW, false);
		this->createPlantsText(2, _global->userInformation->getGameText().find("SUNFLOWER_3")->second, Vec2(440, 1000), 30, Color3B::RED, false);
		this->createPlantsText(3, _global->userInformation->getGameText().find("SUNFLOWER_4")->second, Vec2(360, 830), 30, Color3B::YELLOW, false);
		break;
	case 1:
		_plantCardTextScrollView->setInnerContainerSize(Size(400, 610));
		this->createPlantsAnimation("PeaShooter", "PeaShooter_Normal", "", Vec2(200, 610), 1.8f);
		this->createPlantsText(0, _global->userInformation->getGameText().find("PEASHOOTER_1")->second, Vec2(190, 910), 50);
		this->createPlantsText(2, _global->userInformation->getGameText().find("PEASHOOTER_2")->second, Vec2(360, 1000), 30, Color3B::YELLOW, false);
		this->createPlantsText(3, _global->userInformation->getGameText().find("PEASHOOTER_3")->second, Vec2(440, 1000), 30, Color3B::RED, false);
		this->createPlantsText(1, _global->userInformation->getGameText().find("PEASHOOTER_4")->second, Vec2(360, 830), 30, Color3B::YELLOW, false);

		break;
	case 2:
		_plantCardTextScrollView->setInnerContainerSize(Size(400, 460));
		this->createPlantsAnimation("WallNut", "WallNut_Nornal", "WallNut_Normal", Vec2(200, 610), 1.5f);
		this->createPlantsText(0, _global->userInformation->getGameText().find("WALLNUT_1")->second, Vec2(190, 910), 50);
		this->createPlantsText(2, _global->userInformation->getGameText().find("WALLNUT_2")->second, Vec2(360, 1000), 30, Color3B::YELLOW, false);
		this->createPlantsText(3, _global->userInformation->getGameText().find("WALLNUT_3")->second, Vec2(440, 1000), 30, Color3B::RED, false);
		this->createPlantsText(1, _global->userInformation->getGameText().find("WALLNUT_4")->second, Vec2(360, 870), 30, Color3B::YELLOW, false);
		break;
	case 3:
		_plantCardTextScrollView->setInnerContainerSize(Size(400, 650));
		this->createPlantsAnimation("CherryBomb", "CherryBomb_Normal", "", Vec2(200, 610), 1.8f);
		this->createPlantsText(0, _global->userInformation->getGameText().find("CHERRYBOMB_1")->second, Vec2(190, 910), 50);
		this->createPlantsText(2, _global->userInformation->getGameText().find("CHERRYBOMB_2")->second, Vec2(360, 1000), 30, Color3B::YELLOW, false);
		this->createPlantsText(3, _global->userInformation->getGameText().find("CHERRYBOMB_3")->second, Vec2(440, 1000), 30, Color3B::RED, false);
		this->createPlantsText(1, _global->userInformation->getGameText().find("CHERRYBOMB_4")->second, Vec2(360, 870), 30, Color3B::YELLOW, false);
		break;
	case 4:
		_plantCardTextScrollView->setInnerContainerSize(Size(400, 650));
		this->createPlantsAnimation("PotatoMine", "PotatoMine_Normal", "", Vec2(200, 610), 1.8f);
		this->createPlantsText(0, _global->userInformation->getGameText().find("POTATOMINE_1")->second, Vec2(190, 910), 50);
		this->createPlantsText(2, _global->userInformation->getGameText().find("POTATOMINE_2")->second, Vec2(360, 1000), 30, Color3B::YELLOW, false);
		this->createPlantsText(3, _global->userInformation->getGameText().find("POTATOMINE_3")->second, Vec2(440, 1000), 30, Color3B::RED, false);
		this->createPlantsText(1, _global->userInformation->getGameText().find("POTATOMINE_4")->second, Vec2(360, 870), 30, Color3B::YELLOW, false);
		break;
	case 5:
		_plantCardTextScrollView->setInnerContainerSize(Size(400, 430));
		this->createPlantsAnimation("Cabbage", "Cabbage_Normal", "", Vec2(200, 610), 1.3f);
		this->createPlantsText(0, _global->userInformation->getGameText().find("CABBAGE_1")->second, Vec2(190, 910), 50);
		this->createPlantsText(2, _global->userInformation->getGameText().find("CABBAGE_2")->second, Vec2(360, 1000), 30, Color3B::YELLOW, false);
		this->createPlantsText(3, _global->userInformation->getGameText().find("CABBAGE_3")->second, Vec2(440, 1000), 30, Color3B::RED, false);
		this->createPlantsText(1, _global->userInformation->getGameText().find("CABBAGE_4")->second, Vec2(360, 870), 30, Color3B::YELLOW, false);
		break;
	case 6:
		_plantCardTextScrollView->setInnerContainerSize(Size(400, 540));
		this->createPlantsAnimation("Torchwood", "animation", "", Vec2(200, 610), 2.3f);
		this->createPlantsText(0, _global->userInformation->getGameText().find("TORCHWOOD_1")->second, Vec2(190, 910), 50);
		this->createPlantsText(2, _global->userInformation->getGameText().find("TORCHWOOD_2")->second, Vec2(360, 1000), 30, Color3B::YELLOW, false);
		this->createPlantsText(3, _global->userInformation->getGameText().find("TORCHWOOD_3")->second, Vec2(440, 1000), 30, Color3B::RED, false);
		this->createPlantsText(1, _global->userInformation->getGameText().find("TORCHWOOD_4")->second, Vec2(360, 870), 30, Color3B::YELLOW, false);
		break;
	case 7:
		_plantCardTextScrollView->setInnerContainerSize(Size(400, 500));
		this->createPlantsAnimation("Spikeweed", "Spikeweed_Normal", "", Vec2(200, 610), 1.5f);
		this->createPlantsText(0, _global->userInformation->getGameText().find("SPIKEWEED_1")->second, Vec2(190, 910), 50);
		this->createPlantsText(2, _global->userInformation->getGameText().find("SPIKEWEED_2")->second, Vec2(360, 1000), 30, Color3B::YELLOW, false);
		this->createPlantsText(3, _global->userInformation->getGameText().find("SPIKEWEED_3")->second, Vec2(440, 1000), 30, Color3B::RED, false);
		this->createPlantsText(1, _global->userInformation->getGameText().find("SPIKEWEED_4")->second, Vec2(360, 870), 30, Color3B::YELLOW, false);
		break;
	case 8:
		_plantCardTextScrollView->setInnerContainerSize(Size(400, 535));
		this->createPlantsAnimation("Garlic", "Garlic_Normal", "", Vec2(200, 610), 1.5f);
		this->createPlantsText(0, _global->userInformation->getGameText().find("GARLIC_1")->second, Vec2(190, 910), 50);
		this->createPlantsText(2, _global->userInformation->getGameText().find("GARLIC_2")->second, Vec2(360, 1000), 30, Color3B::YELLOW, false);
		this->createPlantsText(3, _global->userInformation->getGameText().find("GARLIC_3")->second, Vec2(440, 1000), 30, Color3B::RED, false);
		this->createPlantsText(1, _global->userInformation->getGameText().find("GARLIC_4")->second, Vec2(360, 870), 30, Color3B::YELLOW, false);
		break;
	case 9:
		_plantCardTextScrollView->setInnerContainerSize(Size(400, 470));
		this->createPlantsAnimation("FirePeaShooter", "FirePeaShooter_Normal", "", Vec2(200, 610), 1.8f);
		this->createPlantsText(0, _global->userInformation->getGameText().find("FIREPEASHOOTER_1")->second, Vec2(190, 910), 50);
		this->createPlantsText(2, _global->userInformation->getGameText().find("FIREPEASHOOTER_2")->second, Vec2(360, 1000), 30, Color3B::YELLOW, false);
		this->createPlantsText(3, _global->userInformation->getGameText().find("FIREPEASHOOTER_3")->second, Vec2(440, 1000), 30, Color3B::RED, false);
		this->createPlantsText(1, _global->userInformation->getGameText().find("FIREPEASHOOTER_4")->second, Vec2(360, 870), 30, Color3B::YELLOW, false);
		break;
	case 10:
		_plantCardTextScrollView->setInnerContainerSize(Size(400, 470));
		this->createPlantsAnimation("Jalapeno", "Jalapeno_Normal", "", Vec2(200, 610), 1.5f);
		this->createPlantsText(0, _global->userInformation->getGameText().find("FIREPEASHOOTER_1")->second, Vec2(190, 910), 50);
		this->createPlantsText(2, _global->userInformation->getGameText().find("FIREPEASHOOTER_2")->second, Vec2(360, 1000), 30, Color3B::YELLOW, false);
		this->createPlantsText(3, _global->userInformation->getGameText().find("FIREPEASHOOTER_3")->second, Vec2(440, 1000), 30, Color3B::RED, false);
		this->createPlantsText(1, _global->userInformation->getGameText().find("FIREPEASHOOTER_4")->second, Vec2(360, 870), 30, Color3B::YELLOW, false);
		break;
	default:
		break;
	}
	_seedChooser->addChild(_plantsAnimation);
}