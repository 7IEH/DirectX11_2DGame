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
	static FMOD::System* g_pFMOD; // FMOD 관리자 클래스

public:
	FMOD::Sound* m_pSound;       // 로딩된 사운드 객체
	list<FMOD::Channel*>	m_listChannel;  // 사운드가 재생되고 있는 채널


public:
	// _iRoopCount : 0 (무한반복),  _fVolume : 0 ~ 1(Volume), _bOverlap : 소리 중첩 가능여부
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

