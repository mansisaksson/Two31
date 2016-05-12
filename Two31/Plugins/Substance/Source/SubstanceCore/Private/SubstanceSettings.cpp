// Copyright 2014 Allegorithmic All Rights Reserved.

#include "SubstanceCorePrivatePCH.h"
#include "SubstanceInstanceFactory.h"
#include "SubstanceSettings.h"

USubstanceSettings::USubstanceSettings(const FObjectInitializer& PCIP)
	: Super(PCIP)
	, MemoryBudgetMb(512)
	, CPUCores(32)
	, AsyncLoadMipClip(3)
{
}
