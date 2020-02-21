#include "car.h"

#include "Zombies/Zombies.h"
#include "Scenes/GameScene/GSData.h"

Car::Car(Node* node) :
	_carImage(nullptr),
	_position(Vec2::ZERO),
	_isLive(false),
	_tag(-1),
	_carState(1),
	_scale(0),
	_node(node),
	_global(Global::getInstance())
{
}

Car::~Car()
{
}

Car* Car::create(Node* node)
{
	Car* car = new (std::nothrow) Car(node);
	if (car && car->init())
	{
		car->autorelease();
		return car;
	}
	CC_SAFE_DELETE(car);
	return nullptr;
}

void Car::showCar()
{
	_carImage= Sprite::create(_global->userInformation->getImagePath().find("Car")->second);
	_carImage->setPosition(_position);
	_carImage->setLocalZOrder(getZOrder(_position.y));
	_carImage->setScale(_scale);
	_carImage->runAction(ScaleTo::create(0.2f, 0.8f));
	_node->addChild(_carImage);

	createCarShadow();
	createAppearSpecialEffect();
}

void Car::setPosition(const Vec2& position)
{
	_position = position;
}

void Car::setCarTag(const int tag)
{
	_tag = tag;
}

void Car::setParent(Node* node)
{
	_node = node;
}

void Car::setScale(const float& scale)
{
	_scale = scale;
}

void Car::setLive(bool isLive)
{
	_isLive = isLive;
}

bool Car::getLive() const
{
	return _isLive;
}

int Car::getCarTag() const
{
	return _carImage->getTag();
}

Sprite* Car::getCar() const
{
	return _carImage;
}

void Car::createCarListener()
{
	for (auto& zombie : ZombiesGroup)
	{
		if (zombie->getZombieIsEnterMap() && zombie->getZombieIsSurvive() &&         /* ��ʬ�����ͼ && ��ʬû������ */       
			getZombieIsSameLineWithCar(zombie) && getzombieIsEncounterCar(zombie))   /* ��ʬ��С����ͬһ�� && ��ʬ����С�� */
		{
			_isLive = true;

			zombie->setZombieDeath(true);
			zombie->setZombieVisible(false);
		}
	}

	carStartUp();
}

void Car::carStartUp()
{
	if (_isLive)
	{
		if (_carState == 1)
		{
			_carState = 2;
			AudioEngine::setVolume(AudioEngine::play2d(_global->userInformation->getMusicPath().find("lawnmower")->second), _global->userInformation->getSoundEffectVolume());
			_carImage->runAction(MoveBy::create(2.5f, Vec2(1900, 0)));
		}
	}
}

void Car::deleteCar(list<Car*>::iterator& car)
{
	if ((*car)->getCar()->getPositionX() > 2100)
	{
		(*car)->getCar()->removeFromParent();
		delete* car;
		*car = nullptr;
		CarsGroup.erase(car++);
	}
	else
	{
		++car;
	}
}

void Car::createCarShadow()
{
	auto shadow = Sprite::create(_global->userInformation->getImagePath().find("plantshadow")->second);
	shadow->setScaleX(4.0f);
	shadow->setScaleY(3.0f);
	shadow->setPosition(Vec2(120, 30));
	_carImage->addChild(shadow, -1);
}

void Car::createAppearSpecialEffect()
{
	auto fog = Sprite::create(_global->userInformation->getImagePath().find("AwardPickupGlow")->second);
	fog->setScale(4.0f);
	fog->setName("fog");
	fog->setPosition(Vec2(90, 30));
	_carImage->addChild(fog);
	fog->runAction(Sequence::create(DelayTime::create(0.5f), FadeOut::create(0.5f), nullptr));
}

bool Car::getZombieIsSameLineWithCar(Zombies* zombie) const
{
	return fabs((zombie->getZombieAnimation()->getPositionY() + 50) - _carImage->getPositionY()) <= 10 ? true : false;
}

bool Car::getzombieIsEncounterCar(Zombies* zombie) const
{
	return zombie->getZombieAnimation()->getPositionX() - 80 < _carImage->getPositionX() ? true : false;
}

int Car::getZOrder(const float& pos_y) const
{
	int const pos[5] = { 732,594,456,318,180 };
	for (int i = 0; i < 5; ++i)
	{
		if (pos[i] == pos_y)
		{
			return (i + 1) * 20 - 9;
		}
	}
	return 0;
}