#include "pch.h"
#include "EHAsset.h"

Asset::Asset(ASSET_TYPE _type)
	:m_Type(_type)
	, m_RefCount(0)
{
}

Asset::Asset(const Asset& _origin)
	: Entity(_origin)
	, m_Type(_origin.m_Type)
	, m_RefCount(0)
{
}

Asset::~Asset()
{
}
