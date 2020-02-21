#pragma once
#include "cocos2d.h"
#include "Based/GlobalVariable.h"

#ifdef WIN32
#pragma execution_character_set("utf-8")
#endif

using namespace cocos2d;
using namespace cocos2d::experimental;

class Dialog :public Layer
{
public:

	/**
	 *����������
	 */
	virtual void setMouseListener(EventListenerMouse* listener);

protected:
	/**
	 *���ر���
	 */                    
	virtual void createBackGroundImage();

	/**
	 *������������ 
	 */           
	virtual EventListenerTouchOneByOne* createTouchtListener(Sprite* sprite);

	/**
	 *������ǩ
	 */
	virtual Label* label(const std::string &name, const float& fontsize, Vec2 &vec2 = Vec2(0, 0), 
		const float& space = 0, const Color3B& color = Color3B::GREEN, const float& scale = 1);

	/**
	 *������ť�ϵı�ǩ 
	 */
	virtual void createLabel(Sprite* sprite, MenuItemImage* MenuItem, const std::string &name, 
		Vec2 &vec2, float& fontsize, const float& space = 0, const Color3B& color = Color3B::GREEN);

	/**
	 *�������β�
	 */
	virtual void createShieldLayer(Node* node);

	/**
	 *ɾ����
	 */
	virtual void deleteDialog() { _dialogNode->removeFromParent(); }

	/**
	 *�����������Ƿ����
	 */
	virtual void setMouseListenerEnable(bool isEnable = true);

CC_CONSTRUCTOR_ACCESS:
	Dialog();
	~Dialog();

protected:
	Global* _global;
	Node* _dialogNode;
	EventListenerMouse* _mouseListener;

private:
	Vec2 _phasePosition; /* ���λ�� */
	EventListenerTouchOneByOne* _shieldListener;
};
