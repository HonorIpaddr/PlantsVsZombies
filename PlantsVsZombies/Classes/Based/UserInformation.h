/**
 *Copyright (c) 2020 LZ.All Right Reserved
 *Author : LZ
 *Date: 2020.2.20
 *Email: 2117610943@qq.com
 */

#pragma once
#include <list>

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "spine/spine-cocos2dx.h"

using namespace std;
using namespace cocos2d;
using namespace cocos2d::ui;

/* �������� */
enum class WorldName
{
    Mordern = 0,
    DarkAges,
    NeonMixtapeTour,
    FrostibiteCaves
};

/* �û�ѡ��Ŀ��ƴ洢 */
struct UserSelectCard
{
    Button* cardbutton;     //���ư�ť
    int cardTag;            //���Ƶı��
};

/* ����ؿ� */
struct WorldData
{
    WorldData() 
        : levels(1),
        isBeginShowEggs(false),
        isReadWoldInformation(false)
    {}
    int levels;                      // ��ǰ���統ǰ��߹ؿ���
    bool isBeginShowEggs;            
    bool isReadWoldInformation;      // �Ƿ��ȡ������Ϣ
};

class UserInformation
{
public:
    vector<UserSelectCard>& getUserSelectCrads();
    vector<WorldData*>& getUserSelectWorldData();
    CheckBox::EventType getIsShowInformation() const;
    CheckBox::EventType getIsSelectHighFPS() const;
    CheckBox::EventType getIsSelectFullScreen() const;
    CheckBox::EventType getIsSelectCursorNotHide() const;
    CheckBox::EventType getIsSelectStretchingShow() const;
    map<string, spSkeletonData*>& getAnimationData();
    map<string, string>& getGameText();
    map<string, string>& getImagePath();
    map<string, string>& getMusicPath();
    map<string, string>& getAnimationPath();
    map<string, string>& getTextPath();
    WorldName getSelectWorldName() const;
    string& getUserName();
    string& getUserCaveFileName(const int id);
    string& getUserCaveFileNameKey(const int id);
    string& getSystemCaveFileName(const int id);
    string& getSystemDifCaveFileName(const int id);
    list<int>& getBackgroundMusic();
    float getSoundEffectVolume() const;
    float getBackGroundMusicVolume() const;
    int getUserCaveFileNumber() const;
    int& getKillZombiesNumbers();
    int& getUsePlantsNumbers();
    int& getBreakThroughnumbers();
    int getSunNumbers() const;
    int getCoinNumbers() const;
    int getFps() const;
    int getCurrentPlayLevels() const;
    int getCurrentPlayWorldTag() const;
    bool getMainToWorld() const;
    bool getIsUpdate() const;
    bool getIsShowEggs() const;
    Sprite* getBackground() const;
    string& getCurrentPlayWorldName();
    int getGameDifficulty() const;
    void setGameDifficulty(const int difficulty);
    void setCurrentPlayWorldName(const string& worldName);
    void setUserSelectCrads(vector<UserSelectCard>& card);
    void setSelectWorldName(WorldName name);
    void setBackground(Sprite* background);
    void setIsShowEggs(const bool isShow);
    void setUserName(const string& name);
    void setUserCaveFileName(const int id, const string& name);
    void setSoundEffectVolume(const float volume);
    void setBackGroundMusicVolume(const float volume);
    void setUserCaveFileNumber(const int number);
    void setKillZombiesNumbers(const int number);
    void setUsePlantsNumbers(const int number);
    void setBreakThroughnumbers(const int number);
    void setSunNumbers(const int number);
    void setCoinNumbers(const int number);
    void setFps(const int fps);
    void setCurrentPlayLevels(const int level);
    void setCurrentPlayWorldTag(const int tag);
    void setMainToWorld(const bool isMainToWorld);
    void setIsUpdate(const bool isUpdate);
    void setIsSelectStretchingShow(CheckBox::EventType isSelect);
    void setIsSelectCursorNotHide(CheckBox::EventType isSelect);
    void setIsSelectFullScreen(CheckBox::EventType isSelect);
    void setIsSelectHighFPS(CheckBox::EventType isSelect);
    void setIsShowInformation(CheckBox::EventType isShow);

CC_CONSTRUCTOR_ACCESS:
    UserInformation();
    ~UserInformation();

private:

    vector<UserSelectCard> _userSelectCard;              // �û�һ�δ�����ѡֲ��
    vector<WorldData*> _userSelectWorldData;             // �û���ѡ������Ϸ����

    WorldName _selectWorldName;                          // ��ѡ����

    CheckBox::EventType _showInformation;                // ��ʾfps
    CheckBox::EventType _highFPS;                        // ��֡��
    CheckBox::EventType _fullScreen;                     // ȫ��
    CheckBox::EventType _cursorNotHide;                  // �������
    CheckBox::EventType _stretchingShow;                 // ������ʾ

    map<string, spSkeletonData*> _animationData;         // �����洢
    map<string, string> _gameText;                       // ��Ϸ�ı�
    map<string, string> _imagesPath;                     // ͼƬ·��
    map<string, string> _musicsPath;                     // ����·�� 
    map<string, string> _animationPath;                  // ����·��
    map<string, string> _textPath;                       // �ı�·��

    bool _isUpdate;                                      // �����Ƿ����
    string _userName;                                    // �û�����
    string _userCaveFileName[8];                         // �û��Լ�����浵���ƣ���Ҫ������ʾ��
    string _userCaveFileNameKey[8];                      // �û��浵key
    string _systemCaveFileName[8];                       // ϵͳ����浵���ƣ����ڴ�ȡ����ֹ�û������Ϊ��)
    string _systemCaveFileName_difficult[8];             // ϵͳ�������Ѵ浵���ƣ����ڴ�ȡ����ֹ�û������Ϊ��)
    
    float _soundEffectVolume;                            // ��Ч
    float _backGroundMusicVolume;                        // ����
    list<int>_backGroundMusic;

    int _userCaveFileNumber;                             // �û���Ϸ�浵��� 

    int _killZombiesNumbers;                             // ɱ����ʬ���� 
    int _usePlantsNumbers;                               // ʹ��ֲ������ 
    int _breakThroughnumbers;                            // ����ʧ�ܴ���
    
    int _sunNumbers;                                     // ������ 
    int _coinNumbers;                                    // �����
    int _nowFps;                                         // ��ǰfps
    bool _mainToWorld;                                   // ������˵�������
    bool _isShowEggs;                                    // �Ƿ���ʾ�ʵ�

    int _currentPlayLevels;                              // ��ǰ����
    int _gameDifficulty;                                 // ��Ϸ�Ѷ�
    int _currentPlayWorldTag;                            // ��ǰ������ 
    Sprite* _background;                                 // ��ǰ�����ͼ 
    string _currentPlayWorldName;                        // ��ǰ��������
};