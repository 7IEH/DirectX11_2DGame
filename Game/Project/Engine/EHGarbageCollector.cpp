#include "pch.h"
#include "EHGarbageCollector.h"

#include "EHEntity.h"

GarbageCollector::GarbageCollector()
{

}

GarbageCollector::~GarbageCollector()
{
	ReleaseVector(m_ExitObject);
}

void GarbageCollector::Update()
{
	if(m_ExitObject.size()>=100)
	{ 
		ReleaseVector(m_ExitObject);
	}
}
