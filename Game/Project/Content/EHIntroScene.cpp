#include "pch.h"
#include "EHIntroLevel.h"

#include "EHGameObject.h"

#include "EHAssetMgr.h"
#include "EHThreadMgr.h"
#include "EHImGUIMgr.h"

#include "EHMesh.h"
#include "EHSprite.h"
#include "EHMaterial.h" 

#include "EHBehaviour.h"
#include "EHCollisionMgr.h"
#include "EHTimeMgr.h"
#include "EHKeyMgr.h"
#include "EHLevelMgr.h"
#include "EHPathMgr.h"

#include "EHDevice.h"
#include "EHRenderMgr.h"

IntroLevel::IntroLevel()
	: m_PlayTime(0.f)
{
}

IntroLevel::~IntroLevel()
{

	
}

void IntroLevel::Awake()
{
	/*MFStartup(MF_VERSION, MFSTARTUP_FULL);

	MFCreateSourceResolver(&m_pSourceRevolver);*/
}

void IntroLevel::Start()
{

}

void IntroLevel::Update()
{
	
}

void IntroLevel::FixedUpdate()
{

}

void IntroLevel::LateUpdate()
{
}