/**
 *Copyright (c) 2019 LZ.All Right Reserved
 *Author : LZ
 *Date: 2019.6.25
 *Email: 2117610943@qq.com
 */

#include "GameEasterEggs.h"

#include "Scenes/MainMenuScene/QuitScene.h"

GameEasterEggs::GameEasterEggs():
	_eggstext(nullptr)
,   _global(Global::getInstance())
,   _audioId(-1)
,   _arcSpeed(CC_DEGREES_TO_RADIANS(20))
,   _radius(500.0f)
,   _accAngle(0.f)
,   _sprite2(nullptr)
,   _swim(nullptr)
,   _hurt(nullptr)
,   _elapseTransTime(0.f)
,   _moveAction(nullptr)
,  _fileName("resources/Animations/Animation3D/tortoise.c3b")
{
	auto listener = EventListenerTouchAllAtOnce::create();
	listener->onTouchesEnded = CC_CALLBACK_2(GameEasterEggs::onTouchesEnded, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}

GameEasterEggs::~GameEasterEggs()
{
	CC_SAFE_RELEASE(_moveAction);
	CC_SAFE_RELEASE(_hurt);
	CC_SAFE_RELEASE(_swim);
}

Scene* GameEasterEggs::createScene()
{
	return GameEasterEggs::create();
}

bool GameEasterEggs::init()
{
	if (!Scene::init())return false;

	addtortoiseRotate();
	addtortoiseStraight();
	createEggText();
	musicCallBack();
	schedule(schedule_selector(GameEasterEggs::rotateUpdate));
	schedule(schedule_selector(GameEasterEggs::straightUpdate));

	return true;
}

void GameEasterEggs::addtortoiseRotate()
{
	auto sprite = Sprite3D::create(_fileName);
	sprite->setScale(0.7f);
	auto s = Director::getInstance()->getWinSize();
	sprite->setPosition(Vec2(s.width / 2.f + _radius * cosf(_accAngle), s.height / 2.f + _radius * sinf(_accAngle)));
	addChild(sprite);
	_sprite = sprite;
	auto animation = Animation3D::create(_fileName);
	if (animation)
	{
		auto animate = Animate3D::create(animation, 0.f, 1.933f);
		sprite->runAction(RepeatForever::create(animate));
		sprite->runAction(RepeatForever::create(Sequence::create(ScaleTo::create(20.f, 0.2f), ScaleTo::create(10.f, 0.7f), nullptr)));
	}
}

void GameEasterEggs::addtortoiseStraight()
{
	auto sprite = Sprite3D::create(_fileName);
	sprite->setScale(0.35f);
	auto s = Director::getInstance()->getWinSize();
	sprite->setPosition(Vec2(s.width * 4.f / 5.f, s.height / 6.f));
	addChild(sprite);
	_sprite2 = sprite;
	auto animation = Animation3D::create(_fileName);
	if (animation)
	{
		auto animate = Animate3D::create(animation, 0.f, 1.933f);
		_swim = RepeatForever::create(animate);
		sprite->runAction(_swim);

		_swim->retain();
		_hurt = Animate3D::create(animation, 1.933f, 2.8f);
		_hurt->retain();
		_state = State::SWIMMING;
	}

	_moveAction = MoveTo::create(4.f, Vec2(s.width / 5.f, s.height / 6.f));
	_moveAction->retain();
	auto seq = Sequence::create(_moveAction, CallFunc::create(CC_CALLBACK_0(GameEasterEggs::reachEndCallBack, this)), nullptr);
	seq->setTag(100);
	sprite->runAction(seq);
	sprite->runAction(RepeatForever::create(Sequence::create(ScaleTo::create(15.f, 0.55f), ScaleTo::create(8.f, 0.2f), nullptr)));
}

void GameEasterEggs::rotateUpdate(float delta)
{
	_accAngle += delta * _arcSpeed;
	const float pi = static_cast<float>(M_PI);
	if (_accAngle >= 2 * pi)
		_accAngle -= 2 * pi;

	auto s = Director::getInstance()->getWinSize();
	_sprite->setPosition(Vec2(s.width / 2.f + _radius * cosf(_accAngle), s.height / 2.f + _radius * sinf(_accAngle)));

	Quaternion quat;
	Quaternion::createFromAxisAngle(Vec3(0.f, 0.f, 1.f), _accAngle - pi * 0.5f, &quat);
	_sprite->setRotationQuat(quat);
}

void GameEasterEggs::straightUpdate(float dt)
{
	if (_state == State::HURT_TO_SWIMMING)
	{
		_elapseTransTime += dt;

		if (_elapseTransTime >= Animate3D::getTransitionTime())
		{
			_sprite2->stopAction(_hurt);
			_state = State::SWIMMING;
		}
	}
	else if (_state == State::SWIMMING_TO_HURT)
	{
		_elapseTransTime += dt;
		if (_elapseTransTime >= Animate3D::getTransitionTime())
		{
			_sprite2->stopAction(_swim);
			_state = State::HURT;
		}
	}
}

void GameEasterEggs::onTouchesEnded(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event* event)
{
	for (auto touch : touches)
	{
		auto location = touch->getLocation();

		if (_sprite2)
		{
			float len = (_sprite2->getPosition() - location).length();
			if (len < 40)
			{
				//hurt the tortoise
				if (_state == State::SWIMMING)
				{
					_elapseTransTime = 0.0f;
					_state = State::SWIMMING_TO_HURT;
					_sprite2->stopAction(_hurt);
					_sprite2->runAction(_hurt);
					auto delay = DelayTime::create(_hurt->getDuration() - Animate3D::getTransitionTime());
					auto seq = Sequence::create(delay, CallFunc::create(CC_CALLBACK_0(GameEasterEggs::renewCallBack, this)), nullptr);
					seq->setTag(101);
					_sprite2->runAction(seq);
				}
				return;
			}
		}
	}
}

void GameEasterEggs::reachEndCallBack()
{
	auto s = Director::getInstance()->getWinSize();
	_sprite2->stopActionByTag(100);
	auto inverse = MoveTo::create(4.f, Vec2(s.width - _sprite2->getPositionX(), s.height / 6.f));
	inverse->retain();
	_moveAction->release();
	_moveAction = inverse;
	auto rot = RotateBy::create(1.f, Vec3(0.f, 180.f, 0.f));
	auto seq = Sequence::create(rot, _moveAction, CallFunc::create(CC_CALLBACK_0(GameEasterEggs::reachEndCallBack, this)), nullptr);
	seq->setTag(100);
	_sprite2->runAction(seq);
}

void GameEasterEggs::renewCallBack()
{
	//rerun swim action
	_sprite2->runAction(_swim);
	_state = State::HURT_TO_SWIMMING;
	_elapseTransTime = 0.0f;
}

void GameEasterEggs::createEggText()
{
	_audioId = _global->changeBgMusic("ZombiesWinEggs", false);
	AudioEngine::setVolume(_audioId, 1.0f);

	auto sumTime = QuitMenu::getSumRunTime();
	int hour = sumTime / 3600;
	int min = (sumTime - hour * 3600) / 60;
	int sec = sumTime - hour * 3600 - min * 60;

	/* ���ֲʵ� */
	string buff = { "      �������������ף����ͨ���˴˲��԰�����йؿ�����л���ܹ�����ʱ������ע����Ϸ��\
���ǵĹ�ע���ҿ�����Ϸ���������������Ϊ���ǵĹ�ע����ʹ�������㹻������ʹ�Ҽ��������ȥ��������Ϸ���ĸ��á�\n\
      ֲ���ս��ʬ�����Ϸ������ϲ������Ϸ֮һ���ܾ���ǰ�Ҿ�ϣ���Լ��ܹ���������һ��ͬ����Ϸ�������Ǹ�ʱ���������ޣ�\
ֱ��ȥ����٣�2018�꣩�ҿ�ʼ������������˴�Լһ�����¡����������������ޣ�����������Ϸ����û�п����ԡ���󲻵ò�������������\
�����Ҳ�û�������ķ�����ʱ��һ�꣨2019�꣩������Ҵ��¿�ʼ����ֲ���ս��ʬ�������������µ�ѧϰ�뿪����������������һ������\
�����ԵĲ��԰档��Ϸ������һ�����Ѷ��Ҽ����Ĺ��̣��ڼ��������������⣬�й�������Ҳ�й��˷ܡ������Ҽ����������\n\
      �´ο��ܵĸ���������ֲ���Լ���ʬ���࣬�����̵�ϵͳ�Լ�ͼ��ϵͳ����һ���µ����磬������Ϸ���࣬����С��Ϸ�ȵȡ�\n \
      Ŀǰ��7�ֽ�ʬ��11��ֲ�ֲ��ͽ�ʬ�Ķ������Ǳ������ģ��������������������ޣ���Щ��ʬ��ֲ�ﶯ������ʵ�֣�\
�����������������Ը������ҵ��˿���˽���ң���������spine��������Ĺ��������������ĸ����ǲ���ʱ�ģ����ܻ����������qq��2117610943\n\n" };

	auto Eggstext = Label::createWithTTF("", "resources/fonts/GameFont.ttf", 36);
	Eggstext->setString(buff + "��Ϸ�����о��� " + std::to_string(_global->userInformation->getBreakThroughnumbers()) +
		" �δ���ʧ�ܣ�\n��Ϸ������ʹ�� " + std::to_string(_global->userInformation->getUsePlantsNumbers()) +
		" ��ֲ����з�����\n��Ϸ�����й��� " + std::to_string(_global->userInformation->getKillZombiesNumbers()) + " ����ʬ�����ˣ�\n" +
		UserDefault::getInstance()->getStringForKey("FIRSTRUNTIME") + " �����һ�����д���Ϸ��ʱ�䣡\n" +
		"��ĿǰΪֹ���Ѿ���Ϸ�� " + to_string(hour) + "Сʱ " + to_string(min) + "���� " + to_string(sec) + "�룡");
	Eggstext->setPosition(Vec2(Director::getInstance()->getWinSize().width / 2.0f, -430));
	Eggstext->setColor(Color3B(0, 255, 255));
	Eggstext->setMaxLineWidth(1700);
	addChild(Eggstext);

	Eggstext->runAction(Sequence::create(MoveTo::create(25.0f, Director::getInstance()->getWinSize() / 2.0f),
		CallFunc::create([=]()
			{
				auto text = Label::createWithTTF("�������������ֲ������", "resources/fonts/GameFont.ttf", 36);
				text->setPosition(Vec2(Director::getInstance()->getWinSize().width / 2.0f, 70));
				text->setColor(Color3B::WHITE);
				text->setName("Text");
				this->addChild(text);

				this->runAction(RepeatForever::create(Sequence::create(
					CallFunc::create([=]()
						{
							static int I = 0;
							if (I % 3 == 0 || I % 5 == 0 || I % 7 == 0)I++;
							else I += 2;
							auto size = Eggstext->getStringLength();
							for (int i = 0; i < size; i++)
							{
								auto letter = Eggstext->getLetter(i);
								if (letter)
								{
									letter->runAction(Sequence::create(DelayTime::create(0.05f * i), 
										Spawn::create(JumpBy::create(0.7f, Vec2(0, 0), 50, 1), RotateBy::create(0.7f, 360), 
											Sequence::create(TintTo::create(0.35f, Color3B::RED), TintTo::create(0.35f, I % 2 == 0 ? Color3B::GREEN : Color3B(0, 255, 255))), nullptr), nullptr));
								}
							}
						}), DelayTime::create(Eggstext->getStringLength() * 0.05f / 3.0f), nullptr)));

				this->runAction(RepeatForever::create(Sequence::create(
					CallFunc::create([=]()
						{
							auto size1 = text->getStringLength();
							for (int i = 0; i < size1; i++)
							{
								auto letter = text->getLetter(i);
								if (letter)
								{
									letter->runAction(Sequence::create(DelayTime::create(0.1f * i), JumpBy::create(0.5f, Vec2(0, 0), 30, 1), nullptr));
								}
							}
						}), DelayTime::create(text->getStringLength() * 0.1f), nullptr)));

			}), nullptr));
}

void GameEasterEggs::musicCallBack()
{
	AudioEngine::setFinishCallback(_audioId, [=](int i, string name)
		{
			this->getChildByName("Text")->setVisible(false);

			_global->changeBgMusic("mainmusic", true);
			
			/* ������ť */
			auto button = ui::Button::create(_global->userInformation->getImagePath().find("SeedChooser_Button2")->second, _global->userInformation->getImagePath().find("SeedChooser_Button2_Glow")->second);
			button->setTitleText(_global->userInformation->getGameText().find("�˳�")->second);
			button->setTitleColor(Color3B(0, 255, 255));
			button->setTitleFontSize(18);
			button->setScale(2.0f);
			button->setPosition(Vec2(Director::getInstance()->getWinSize().width / 2, 70));
			this->addChild(button);
			button->addTouchEventListener([&](Ref* sender, ui::Widget::TouchEventType type)
				{
					switch (type)
					{
					case ui::Widget::TouchEventType::BEGAN:
						AudioEngine::setVolume(AudioEngine::play2d(_global->userInformation->getMusicPath().find("tap2")->second), _global->userInformation->getSoundEffectVolume());
						break;
					case ui::Widget::TouchEventType::ENDED:
						Director::getInstance()->popScene();
						break;
					}
				});
		});
}
