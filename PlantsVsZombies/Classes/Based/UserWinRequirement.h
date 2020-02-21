#pragma once
#include "cocos2d.h"
#include "Based/GlobalVariable.h"

#ifdef WIN32
#pragma execution_character_set("utf-8")
#endif

using namespace cocos2d;
using namespace cocos2d::ui;
using namespace cocos2d::experimental;

enum class GameTypes;

class UserWinRequirement :public Node
{
public:
	void createDialogBox(GameTypes finishedid);
	void setParent(Node* node);
	void setButtonVisible(bool visible);
	void setDialogOpacity(const int opacity);
	void setListenerEnable(bool enable);
	void setShowDialogAction();
	void setDelectDialogAction();
	void setLayerColorOpacity(Color3B& color, GLubyte& opacity);
	void deleteDialog();
	Sprite* getDialog() const;

CC_CONSTRUCTOR_ACCESS:
	UserWinRequirement(Node* node = nullptr);
	~UserWinRequirement();

private:
	void showRequirement(GameTypes finishedid);                         /* ��ʾҪ�� */
	void showText(const string& text, const int& ID, Color3B color = Color3B::BLACK);
	void createListener();
	void createShieldLayer();

private:
	Node* _node;
	Layer* _layer;
	Global* _global;
	Button* _continuesGame;
	Scale9Sprite* _levelObjiectives;
	EventListenerTouchOneByOne* _listener;
	EventListenerTouchOneByOne* _shieldListener;
	Vec2 _phasePosition;                                                 /* ���λ�� */

	Vec2 _textPosition[4][4] =                                           /* ����λ�� */
	{
		{Vec2(200,270),},
		{Vec2(200,220),Vec2(200,320),},
		{Vec2(200,180),Vec2(200,270),Vec2(200,360)},
		{Vec2(200,160),Vec2(200,230),Vec2(200,300),Vec2(200,370)}
	};
};