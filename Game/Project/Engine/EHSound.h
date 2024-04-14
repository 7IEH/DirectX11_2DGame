#pragma once
#include "EHAsset.h"

#include <fmod.h>
#include <fmod.hpp>
#include <fmod_codec.h>

#ifdef _DEBUG
#pragma comment(lib, "fmodL64_vc.lib")
#else
#pragma comment(lib, "fmod64_vc.lib")
#endif

class Sound :
    public Asset
{
public:
	static FMOD::System* g_pFMOD; // FMOD ������ Ŭ����

public:
	FMOD::Sound* m_pSound;       // �ε��� ���� ��ü
	list<FMOD::Channel*>	m_listChannel;  // ���尡 ����ǰ� �ִ� ä��


public:
	// _iRoopCount : 0 (���ѹݺ�),  _fVolume : 0 ~ 1(Volume), _bOverlap : �Ҹ� ��ø ���ɿ���
	int Play(int _iRoopCount, float _fVolume = 1.f, bool _bOverlap = false);
	void Stop();

	// 0 ~ 1
	void SetVolume(float _f, int _iChannelIdx);

	virtual HRESULT Load(const wstring& _strFilePath)override;

private:
	void RemoveChannel(FMOD::Channel* _pTargetChannel);
	friend FMOD_RESULT CHANNEL_CALLBACK(FMOD_CHANNELCONTROL* channelcontrol, FMOD_CHANNELCONTROL_TYPE controltype
		, FMOD_CHANNELCONTROL_CALLBACK_TYPE callbacktype
		, void* commanddata1, void* commanddata2);

public:
	virtual void UpdateData() {}
public:
    CLONE_DISABLE(Sound);
    Sound();
    virtual ~Sound();
};

