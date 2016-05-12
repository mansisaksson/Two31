//! @file SubstanceUtility.cpp
//! @brief Implementation of the SubstanceUtility blueprint utility class
//! @author Antoine Gonzalez - Allegorithmic
//! @copyright Allegorithmic. All rights reserved.

#include "SubstanceCorePrivatePCH.h"
#include "SubstanceUtility.h"
#include "SubstanceFGraph.h"
#include "SubstanceFPackage.h"
#include "SubstanceTexture2D.h"
#include "SubstanceGraphInstance.h"
#include "SubstanceInstanceFactory.h"


#include "Materials/MaterialExpressionTextureSample.h"

DEFINE_LOG_CATEGORY_STATIC(LogSbsBP, Warning, All);

USubstanceUtility::USubstanceUtility(class FObjectInitializer const & PCIP) : Super(PCIP)
{
}


TArray<class USubstanceGraphInstance*> USubstanceUtility::GetSubstances(class UMaterialInterface* MaterialInterface)
{
	TArray<class USubstanceGraphInstance*> Substances;

	if (!MaterialInterface)
	{
		return Substances;
	}

	UMaterial* Material = MaterialInterface->GetMaterial(); 

	for (int32 ExpressionIndex = Material->Expressions.Num() - 1; ExpressionIndex >= 0; ExpressionIndex--)
	{
		UMaterialExpressionTextureSample* Expression = Cast<UMaterialExpressionTextureSample>(Material->Expressions[ExpressionIndex]);

		if (Expression)
		{
			USubstanceTexture2D* SubstanceTexture = Cast<USubstanceTexture2D>(Expression->Texture);

			if (SubstanceTexture && SubstanceTexture->ParentInstance)
			{
				Substances.AddUnique(SubstanceTexture->ParentInstance);
			}
		}
	}

	return Substances;
}


TArray<class USubstanceTexture2D*> USubstanceUtility::GetSubstanceTextures(class USubstanceGraphInstance* GraphInstance)
{
	TArray<class USubstanceTexture2D*> SubstanceTextures;

	if (!GraphInstance)
	{
		return SubstanceTextures;
	}

	for (uint32 Idx = 0; Idx<GraphInstance->Instance->Outputs.size(); ++Idx)
	{
		output_inst_t* OutputInstance = &GraphInstance->Instance->Outputs[Idx];

		if (OutputInstance->bIsEnabled)
		{
			SubstanceTextures.Add(*OutputInstance->Texture.get());
		}
	}
	return SubstanceTextures;
}



float USubstanceUtility::GetSubstanceLoadingProgress()
{
	return Substance::Helpers::GetSubstanceLoadingProgress();
}


USubstanceGraphInstance* USubstanceUtility::CreateGraphInstance(UObject* WorldContextObject, USubstanceInstanceFactory* Factory, int32 GraphDescIndex, FString InstanceName)
{	
	check(WorldContextObject);
	USubstanceGraphInstance* GraphInstance = NULL;

	if (Factory && Factory->SubstancePackage && GraphDescIndex < Factory->SubstancePackage->Graphs.Num())
	{
		if (Factory->GetGenerationMode() == SGM_Baked)
		{
			UE_LOG(LogSbsBP, Warning, TEXT("Cannot create Graph Instance for Instance Factory %s, GenerationMode value not set to OnLoadAsync or OnLoadSync!"),
				*Factory->GetName());
			return GraphInstance;
		}

		GraphInstance = NewObject<USubstanceGraphInstance>(
			WorldContextObject ? WorldContextObject : GetTransientPackage(),
			*InstanceName);

		Factory->SubstancePackage->Graphs[GraphDescIndex]->Instantiate(
			GraphInstance, 
			false/*bCreateOutputs*/,
			true /*bSubscribeInstance*/,
			true /*bDynamicInstance*/);

		GraphInstance->Instance->bIsFreezed = false;
	}

	return GraphInstance;
}	


USubstanceGraphInstance* USubstanceUtility::DuplicateGraphInstance(UObject* WorldContextObject, USubstanceGraphInstance* GraphInstance)
{
	check(WorldContextObject);

	USubstanceGraphInstance* NewGraphInstance = NULL;

	if (GraphInstance && GraphInstance->Parent && GraphInstance->Parent->SubstancePackage)
	{
		int idx = 0;
		for (auto itGraph = GraphInstance->Parent->SubstancePackage->Graphs.itfront(); itGraph; ++itGraph, ++idx)
		{
			if (GraphInstance->Instance->Desc == *itGraph)
			{
				break;
			}
		}

		NewGraphInstance = CreateGraphInstance(WorldContextObject, GraphInstance->Parent, idx);

		CopyInputParameters(GraphInstance, NewGraphInstance);
	}

	return NewGraphInstance;
}


void USubstanceUtility::CreateInstanceOutputs(UObject* WorldContextObject, USubstanceGraphInstance* GraphInstance, TArray<int32> OutputIndices)
{
	if (!GraphInstance || !GraphInstance->Instance)
	{
		return;
	}

	for (auto IdxIt = OutputIndices.CreateConstIterator(); IdxIt; ++IdxIt)
	{
		if (*IdxIt >= GraphInstance->Instance->Outputs.Num())
		{
			UE_LOG(LogSbsBP, Warning, TEXT("Cannot create Output for %s, index %d out of range!"),
				*GraphInstance->GetName(), *IdxIt);
			continue;
		}

		output_inst_t* OutputInstance = &GraphInstance->Instance->Outputs[*IdxIt];
		USubstanceTexture2D** ptr = OutputInstance->Texture.get();

		if (ptr && *ptr == NULL)
		{
			Substance::Helpers::CreateSubstanceTexture2D(
				OutputInstance, 
				true,
				FString() /*name does not matter for dynamic instances*/, 
				WorldContextObject ? WorldContextObject : GetTransientPackage());
			OutputInstance->bIsEnabled = true;
			OutputInstance->bIsDirty = true;
		}
		else
		{
			UE_LOG(LogSbsBP, Warning, TEXT("Cannot create Output for %s, index %d already created!"),
				*GraphInstance->GetName(), *IdxIt);
			continue;
		}
	}

	// upload some place holder content in the texture to make the texture usable
	Substance::Helpers::CreatePlaceHolderMips(GraphInstance->Instance);
}


void USubstanceUtility::CopyInputParameters(USubstanceGraphInstance* SourceGraphInstance, USubstanceGraphInstance* DestGraphInstance)
{
	if (!SourceGraphInstance ||
		!SourceGraphInstance->Instance ||
		!DestGraphInstance ||
		!DestGraphInstance->Instance)
	{
		return;
	}

	Substance::Helpers::CopyInstance(SourceGraphInstance->Instance, DestGraphInstance->Instance, false);
}


void USubstanceUtility::ResetInputParameters(USubstanceGraphInstance* GraphInstance)
{
	if (!GraphInstance ||
		!GraphInstance->Instance)
	{
		return;
	}

	Substance::Helpers::ResetToDefault(GraphInstance->Instance);
}


int SizeToPow2(ESubstanceTextureSize res)
{
	switch (res)
	{
	case ERL_16:
		return 4;
	case ERL_32:
		return 5;
	case ERL_64:
		return 6;
	case ERL_128:
		return 7;
	case ERL_256:
		return 8;
	case ERL_512:
		return 9;
	case ERL_1024:
		return 10;
	case ERL_2048:
		return 11;
	default:
		return 8;
	}
}


void USubstanceUtility::SetGraphInstanceOutputSize(USubstanceGraphInstance* GraphInstance, ESubstanceTextureSize Width, ESubstanceTextureSize Height)
{
	if (!GraphInstance || !GraphInstance->Instance)
	{
		return;
	}

	TArray<int32> OutputSize;
	OutputSize.Add(SizeToPow2(Width));
	OutputSize.Add(SizeToPow2(Height));

	GraphInstance->SetInputInt(TEXT("$outputsize"), OutputSize);
}


void USubstanceUtility::SetGraphInstanceOutputSizeInt(USubstanceGraphInstance* GraphInstance, int32 Width, int32 Height)
{
	if (!GraphInstance || !GraphInstance->Instance)
	{
		return;
	}

	Width = FMath::Clamp(Width, 16, 2048);
	Height = FMath::Clamp(Height, 16, 2048);

	Width = FMath::Log2(FMath::Pow(2, (int32)FMath::Log2(Width)));
	Height = FMath::Log2(FMath::Pow(2, (int32)FMath::Log2(Height)));

	TArray<int32> OutputSize;
	OutputSize.Add(Width);
	OutputSize.Add(Height);

	GraphInstance->SetInputInt(TEXT("$outputsize"), OutputSize);
}


void USubstanceUtility::SyncRendering(USubstanceGraphInstance* GraphInstance)
{
	if (!GraphInstance || !GraphInstance->Instance)
	{
		return;
	}

	Substance::Helpers::RenderSync(GraphInstance->Instance);
	Substance::List<graph_inst_t*> Instances;
	Instances.AddUnique(GraphInstance->Instance);
	Substance::Helpers::UpdateTextures(Instances);
}


void USubstanceUtility::AsyncRendering(USubstanceGraphInstance* GraphInstance)
{
	if (!GraphInstance || !GraphInstance->Instance)
	{
		return;
	}

	Substance::Helpers::RenderAsync(GraphInstance->Instance);
}
