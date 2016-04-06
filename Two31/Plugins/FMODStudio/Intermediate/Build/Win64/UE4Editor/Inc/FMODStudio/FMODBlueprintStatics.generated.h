// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	C++ class header boilerplate exported from UnrealHeaderTool.
	This is automatically generated by the tools.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "ObjectBase.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS
struct FFMODEventInstance;
struct FTransform;
class UFMODBus;
class UObject;
class UFMODEvent;
class UFMODBank;
class UFMODAsset;
class USceneComponent;
struct FVector;
class UFMODAudioComponent;
#ifdef FMODSTUDIO_FMODBlueprintStatics_generated_h
#error "FMODBlueprintStatics.generated.h already included, missing '#pragma once' in FMODBlueprintStatics.h"
#endif
#define FMODSTUDIO_FMODBlueprintStatics_generated_h

#define Two31_Plugins_FMODStudio_Source_FMODStudio_Classes_FMODBlueprintStatics_h_26_GENERATED_BODY \
	friend FMODSTUDIO_API class UScriptStruct* Z_Construct_UScriptStruct_FFMODEventInstance(); \
	FMODSTUDIO_API static class UScriptStruct* StaticStruct();


#define Two31_Plugins_FMODStudio_Source_FMODStudio_Classes_FMODBlueprintStatics_h_35_RPC_WRAPPERS \
 \
	DECLARE_FUNCTION(execEventInstanceSetTransform) \
	{ \
		P_GET_STRUCT(FFMODEventInstance,Z_Param_EventInstance); \
		P_GET_STRUCT_REF(FTransform,Z_Param_Out_Location); \
		P_FINISH; \
		UFMODBlueprintStatics::EventInstanceSetTransform(Z_Param_EventInstance,Z_Param_Out_Location); \
	} \
 \
	DECLARE_FUNCTION(execEventInstanceTriggerCue) \
	{ \
		P_GET_STRUCT(FFMODEventInstance,Z_Param_EventInstance); \
		P_FINISH; \
		UFMODBlueprintStatics::EventInstanceTriggerCue(Z_Param_EventInstance); \
	} \
 \
	DECLARE_FUNCTION(execEventInstanceStop) \
	{ \
		P_GET_STRUCT(FFMODEventInstance,Z_Param_EventInstance); \
		P_FINISH; \
		UFMODBlueprintStatics::EventInstanceStop(Z_Param_EventInstance); \
	} \
 \
	DECLARE_FUNCTION(execEventInstancePlay) \
	{ \
		P_GET_STRUCT(FFMODEventInstance,Z_Param_EventInstance); \
		P_FINISH; \
		UFMODBlueprintStatics::EventInstancePlay(Z_Param_EventInstance); \
	} \
 \
	DECLARE_FUNCTION(execEventInstanceGetParameter) \
	{ \
		P_GET_STRUCT(FFMODEventInstance,Z_Param_EventInstance); \
		P_GET_PROPERTY(UNameProperty,Z_Param_Name); \
		P_FINISH; \
		*(float*)Z_Param__Result=UFMODBlueprintStatics::EventInstanceGetParameter(Z_Param_EventInstance,Z_Param_Name); \
	} \
 \
	DECLARE_FUNCTION(execEventInstanceSetParameter) \
	{ \
		P_GET_STRUCT(FFMODEventInstance,Z_Param_EventInstance); \
		P_GET_PROPERTY(UNameProperty,Z_Param_Name); \
		P_GET_PROPERTY(UFloatProperty,Z_Param_Value); \
		P_FINISH; \
		UFMODBlueprintStatics::EventInstanceSetParameter(Z_Param_EventInstance,Z_Param_Name,Z_Param_Value); \
	} \
 \
	DECLARE_FUNCTION(execEventInstanceSetPaused) \
	{ \
		P_GET_STRUCT(FFMODEventInstance,Z_Param_EventInstance); \
		P_GET_UBOOL(Z_Param_Paused); \
		P_FINISH; \
		UFMODBlueprintStatics::EventInstanceSetPaused(Z_Param_EventInstance,Z_Param_Paused); \
	} \
 \
	DECLARE_FUNCTION(execEventInstanceSetPitch) \
	{ \
		P_GET_STRUCT(FFMODEventInstance,Z_Param_EventInstance); \
		P_GET_PROPERTY(UFloatProperty,Z_Param_Pitch); \
		P_FINISH; \
		UFMODBlueprintStatics::EventInstanceSetPitch(Z_Param_EventInstance,Z_Param_Pitch); \
	} \
 \
	DECLARE_FUNCTION(execEventInstanceSetVolume) \
	{ \
		P_GET_STRUCT(FFMODEventInstance,Z_Param_EventInstance); \
		P_GET_PROPERTY(UFloatProperty,Z_Param_Volume); \
		P_FINISH; \
		UFMODBlueprintStatics::EventInstanceSetVolume(Z_Param_EventInstance,Z_Param_Volume); \
	} \
 \
	DECLARE_FUNCTION(execEventInstanceIsValid) \
	{ \
		P_GET_STRUCT(FFMODEventInstance,Z_Param_EventInstance); \
		P_FINISH; \
		*(bool*)Z_Param__Result=UFMODBlueprintStatics::EventInstanceIsValid(Z_Param_EventInstance); \
	} \
 \
	DECLARE_FUNCTION(execBusSetMute) \
	{ \
		P_GET_OBJECT(UFMODBus,Z_Param_Bus); \
		P_GET_UBOOL(Z_Param_bMute); \
		P_FINISH; \
		UFMODBlueprintStatics::BusSetMute(Z_Param_Bus,Z_Param_bMute); \
	} \
 \
	DECLARE_FUNCTION(execBusSetPaused) \
	{ \
		P_GET_OBJECT(UFMODBus,Z_Param_Bus); \
		P_GET_UBOOL(Z_Param_bPaused); \
		P_FINISH; \
		UFMODBlueprintStatics::BusSetPaused(Z_Param_Bus,Z_Param_bPaused); \
	} \
 \
	DECLARE_FUNCTION(execBusSetFaderLevel) \
	{ \
		P_GET_OBJECT(UFMODBus,Z_Param_Bus); \
		P_GET_PROPERTY(UFloatProperty,Z_Param_Level); \
		P_FINISH; \
		UFMODBlueprintStatics::BusSetFaderLevel(Z_Param_Bus,Z_Param_Level); \
	} \
 \
	DECLARE_FUNCTION(execFindEventInstances) \
	{ \
		P_GET_OBJECT(UObject,Z_Param_WorldContextObject); \
		P_GET_OBJECT(UFMODEvent,Z_Param_Event); \
		P_FINISH; \
		*(TArray<FFMODEventInstance>*)Z_Param__Result=UFMODBlueprintStatics::FindEventInstances(Z_Param_WorldContextObject,Z_Param_Event); \
	} \
 \
	DECLARE_FUNCTION(execUnloadEventSampleData) \
	{ \
		P_GET_OBJECT(UObject,Z_Param_WorldContextObject); \
		P_GET_OBJECT(UFMODEvent,Z_Param_Event); \
		P_FINISH; \
		UFMODBlueprintStatics::UnloadEventSampleData(Z_Param_WorldContextObject,Z_Param_Event); \
	} \
 \
	DECLARE_FUNCTION(execLoadEventSampleData) \
	{ \
		P_GET_OBJECT(UObject,Z_Param_WorldContextObject); \
		P_GET_OBJECT(UFMODEvent,Z_Param_Event); \
		P_FINISH; \
		UFMODBlueprintStatics::LoadEventSampleData(Z_Param_WorldContextObject,Z_Param_Event); \
	} \
 \
	DECLARE_FUNCTION(execUnloadBankSampleData) \
	{ \
		P_GET_OBJECT(UFMODBank,Z_Param_Bank); \
		P_FINISH; \
		UFMODBlueprintStatics::UnloadBankSampleData(Z_Param_Bank); \
	} \
 \
	DECLARE_FUNCTION(execLoadBankSampleData) \
	{ \
		P_GET_OBJECT(UFMODBank,Z_Param_Bank); \
		P_FINISH; \
		UFMODBlueprintStatics::LoadBankSampleData(Z_Param_Bank); \
	} \
 \
	DECLARE_FUNCTION(execUnloadBank) \
	{ \
		P_GET_OBJECT(UFMODBank,Z_Param_Bank); \
		P_FINISH; \
		UFMODBlueprintStatics::UnloadBank(Z_Param_Bank); \
	} \
 \
	DECLARE_FUNCTION(execLoadBank) \
	{ \
		P_GET_OBJECT(UFMODBank,Z_Param_Bank); \
		P_GET_UBOOL(Z_Param_bBlocking); \
		P_GET_UBOOL(Z_Param_bLoadSampleData); \
		P_FINISH; \
		UFMODBlueprintStatics::LoadBank(Z_Param_Bank,Z_Param_bBlocking,Z_Param_bLoadSampleData); \
	} \
 \
	DECLARE_FUNCTION(execFindEventByName) \
	{ \
		P_GET_PROPERTY(UStrProperty,Z_Param_Name); \
		P_FINISH; \
		*(UFMODEvent**)Z_Param__Result=UFMODBlueprintStatics::FindEventByName(Z_Param_Name); \
	} \
 \
	DECLARE_FUNCTION(execFindAssetByName) \
	{ \
		P_GET_PROPERTY(UStrProperty,Z_Param_Name); \
		P_FINISH; \
		*(UFMODAsset**)Z_Param__Result=UFMODBlueprintStatics::FindAssetByName(Z_Param_Name); \
	} \
 \
	DECLARE_FUNCTION(execPlayEventAttached) \
	{ \
		P_GET_OBJECT(UFMODEvent,Z_Param_Event); \
		P_GET_OBJECT(USceneComponent,Z_Param_AttachToComponent); \
		P_GET_PROPERTY(UNameProperty,Z_Param_AttachPointName); \
		P_GET_STRUCT(FVector,Z_Param_Location); \
		P_GET_PROPERTY(UByteProperty,Z_Param_LocationType); \
		P_GET_UBOOL(Z_Param_bStopWhenAttachedToDestroyed); \
		P_GET_UBOOL(Z_Param_bAutoPlay); \
		P_FINISH; \
		*(UFMODAudioComponent**)Z_Param__Result=UFMODBlueprintStatics::PlayEventAttached(Z_Param_Event,Z_Param_AttachToComponent,Z_Param_AttachPointName,Z_Param_Location,EAttachLocation::Type(Z_Param_LocationType),Z_Param_bStopWhenAttachedToDestroyed,Z_Param_bAutoPlay); \
	} \
 \
	DECLARE_FUNCTION(execPlayEventAtLocation) \
	{ \
		P_GET_OBJECT(UObject,Z_Param_WorldContextObject); \
		P_GET_OBJECT(UFMODEvent,Z_Param_Event); \
		P_GET_STRUCT_REF(FTransform,Z_Param_Out_Location); \
		P_GET_UBOOL(Z_Param_bAutoPlay); \
		P_FINISH; \
		*(FFMODEventInstance*)Z_Param__Result=UFMODBlueprintStatics::PlayEventAtLocation(Z_Param_WorldContextObject,Z_Param_Event,Z_Param_Out_Location,Z_Param_bAutoPlay); \
	} \
 \
	DECLARE_FUNCTION(execPlayEvent2D) \
	{ \
		P_GET_OBJECT(UObject,Z_Param_WorldContextObject); \
		P_GET_OBJECT(UFMODEvent,Z_Param_Event); \
		P_GET_UBOOL(Z_Param_bAutoPlay); \
		P_FINISH; \
		*(FFMODEventInstance*)Z_Param__Result=UFMODBlueprintStatics::PlayEvent2D(Z_Param_WorldContextObject,Z_Param_Event,Z_Param_bAutoPlay); \
	}


#define Two31_Plugins_FMODStudio_Source_FMODStudio_Classes_FMODBlueprintStatics_h_35_RPC_WRAPPERS_NO_PURE_DECLS \
 \
	DECLARE_FUNCTION(execEventInstanceSetTransform) \
	{ \
		P_GET_STRUCT(FFMODEventInstance,Z_Param_EventInstance); \
		P_GET_STRUCT_REF(FTransform,Z_Param_Out_Location); \
		P_FINISH; \
		UFMODBlueprintStatics::EventInstanceSetTransform(Z_Param_EventInstance,Z_Param_Out_Location); \
	} \
 \
	DECLARE_FUNCTION(execEventInstanceTriggerCue) \
	{ \
		P_GET_STRUCT(FFMODEventInstance,Z_Param_EventInstance); \
		P_FINISH; \
		UFMODBlueprintStatics::EventInstanceTriggerCue(Z_Param_EventInstance); \
	} \
 \
	DECLARE_FUNCTION(execEventInstanceStop) \
	{ \
		P_GET_STRUCT(FFMODEventInstance,Z_Param_EventInstance); \
		P_FINISH; \
		UFMODBlueprintStatics::EventInstanceStop(Z_Param_EventInstance); \
	} \
 \
	DECLARE_FUNCTION(execEventInstancePlay) \
	{ \
		P_GET_STRUCT(FFMODEventInstance,Z_Param_EventInstance); \
		P_FINISH; \
		UFMODBlueprintStatics::EventInstancePlay(Z_Param_EventInstance); \
	} \
 \
	DECLARE_FUNCTION(execEventInstanceGetParameter) \
	{ \
		P_GET_STRUCT(FFMODEventInstance,Z_Param_EventInstance); \
		P_GET_PROPERTY(UNameProperty,Z_Param_Name); \
		P_FINISH; \
		*(float*)Z_Param__Result=UFMODBlueprintStatics::EventInstanceGetParameter(Z_Param_EventInstance,Z_Param_Name); \
	} \
 \
	DECLARE_FUNCTION(execEventInstanceSetParameter) \
	{ \
		P_GET_STRUCT(FFMODEventInstance,Z_Param_EventInstance); \
		P_GET_PROPERTY(UNameProperty,Z_Param_Name); \
		P_GET_PROPERTY(UFloatProperty,Z_Param_Value); \
		P_FINISH; \
		UFMODBlueprintStatics::EventInstanceSetParameter(Z_Param_EventInstance,Z_Param_Name,Z_Param_Value); \
	} \
 \
	DECLARE_FUNCTION(execEventInstanceSetPaused) \
	{ \
		P_GET_STRUCT(FFMODEventInstance,Z_Param_EventInstance); \
		P_GET_UBOOL(Z_Param_Paused); \
		P_FINISH; \
		UFMODBlueprintStatics::EventInstanceSetPaused(Z_Param_EventInstance,Z_Param_Paused); \
	} \
 \
	DECLARE_FUNCTION(execEventInstanceSetPitch) \
	{ \
		P_GET_STRUCT(FFMODEventInstance,Z_Param_EventInstance); \
		P_GET_PROPERTY(UFloatProperty,Z_Param_Pitch); \
		P_FINISH; \
		UFMODBlueprintStatics::EventInstanceSetPitch(Z_Param_EventInstance,Z_Param_Pitch); \
	} \
 \
	DECLARE_FUNCTION(execEventInstanceSetVolume) \
	{ \
		P_GET_STRUCT(FFMODEventInstance,Z_Param_EventInstance); \
		P_GET_PROPERTY(UFloatProperty,Z_Param_Volume); \
		P_FINISH; \
		UFMODBlueprintStatics::EventInstanceSetVolume(Z_Param_EventInstance,Z_Param_Volume); \
	} \
 \
	DECLARE_FUNCTION(execEventInstanceIsValid) \
	{ \
		P_GET_STRUCT(FFMODEventInstance,Z_Param_EventInstance); \
		P_FINISH; \
		*(bool*)Z_Param__Result=UFMODBlueprintStatics::EventInstanceIsValid(Z_Param_EventInstance); \
	} \
 \
	DECLARE_FUNCTION(execBusSetMute) \
	{ \
		P_GET_OBJECT(UFMODBus,Z_Param_Bus); \
		P_GET_UBOOL(Z_Param_bMute); \
		P_FINISH; \
		UFMODBlueprintStatics::BusSetMute(Z_Param_Bus,Z_Param_bMute); \
	} \
 \
	DECLARE_FUNCTION(execBusSetPaused) \
	{ \
		P_GET_OBJECT(UFMODBus,Z_Param_Bus); \
		P_GET_UBOOL(Z_Param_bPaused); \
		P_FINISH; \
		UFMODBlueprintStatics::BusSetPaused(Z_Param_Bus,Z_Param_bPaused); \
	} \
 \
	DECLARE_FUNCTION(execBusSetFaderLevel) \
	{ \
		P_GET_OBJECT(UFMODBus,Z_Param_Bus); \
		P_GET_PROPERTY(UFloatProperty,Z_Param_Level); \
		P_FINISH; \
		UFMODBlueprintStatics::BusSetFaderLevel(Z_Param_Bus,Z_Param_Level); \
	} \
 \
	DECLARE_FUNCTION(execFindEventInstances) \
	{ \
		P_GET_OBJECT(UObject,Z_Param_WorldContextObject); \
		P_GET_OBJECT(UFMODEvent,Z_Param_Event); \
		P_FINISH; \
		*(TArray<FFMODEventInstance>*)Z_Param__Result=UFMODBlueprintStatics::FindEventInstances(Z_Param_WorldContextObject,Z_Param_Event); \
	} \
 \
	DECLARE_FUNCTION(execUnloadEventSampleData) \
	{ \
		P_GET_OBJECT(UObject,Z_Param_WorldContextObject); \
		P_GET_OBJECT(UFMODEvent,Z_Param_Event); \
		P_FINISH; \
		UFMODBlueprintStatics::UnloadEventSampleData(Z_Param_WorldContextObject,Z_Param_Event); \
	} \
 \
	DECLARE_FUNCTION(execLoadEventSampleData) \
	{ \
		P_GET_OBJECT(UObject,Z_Param_WorldContextObject); \
		P_GET_OBJECT(UFMODEvent,Z_Param_Event); \
		P_FINISH; \
		UFMODBlueprintStatics::LoadEventSampleData(Z_Param_WorldContextObject,Z_Param_Event); \
	} \
 \
	DECLARE_FUNCTION(execUnloadBankSampleData) \
	{ \
		P_GET_OBJECT(UFMODBank,Z_Param_Bank); \
		P_FINISH; \
		UFMODBlueprintStatics::UnloadBankSampleData(Z_Param_Bank); \
	} \
 \
	DECLARE_FUNCTION(execLoadBankSampleData) \
	{ \
		P_GET_OBJECT(UFMODBank,Z_Param_Bank); \
		P_FINISH; \
		UFMODBlueprintStatics::LoadBankSampleData(Z_Param_Bank); \
	} \
 \
	DECLARE_FUNCTION(execUnloadBank) \
	{ \
		P_GET_OBJECT(UFMODBank,Z_Param_Bank); \
		P_FINISH; \
		UFMODBlueprintStatics::UnloadBank(Z_Param_Bank); \
	} \
 \
	DECLARE_FUNCTION(execLoadBank) \
	{ \
		P_GET_OBJECT(UFMODBank,Z_Param_Bank); \
		P_GET_UBOOL(Z_Param_bBlocking); \
		P_GET_UBOOL(Z_Param_bLoadSampleData); \
		P_FINISH; \
		UFMODBlueprintStatics::LoadBank(Z_Param_Bank,Z_Param_bBlocking,Z_Param_bLoadSampleData); \
	} \
 \
	DECLARE_FUNCTION(execFindEventByName) \
	{ \
		P_GET_PROPERTY(UStrProperty,Z_Param_Name); \
		P_FINISH; \
		*(UFMODEvent**)Z_Param__Result=UFMODBlueprintStatics::FindEventByName(Z_Param_Name); \
	} \
 \
	DECLARE_FUNCTION(execFindAssetByName) \
	{ \
		P_GET_PROPERTY(UStrProperty,Z_Param_Name); \
		P_FINISH; \
		*(UFMODAsset**)Z_Param__Result=UFMODBlueprintStatics::FindAssetByName(Z_Param_Name); \
	} \
 \
	DECLARE_FUNCTION(execPlayEventAttached) \
	{ \
		P_GET_OBJECT(UFMODEvent,Z_Param_Event); \
		P_GET_OBJECT(USceneComponent,Z_Param_AttachToComponent); \
		P_GET_PROPERTY(UNameProperty,Z_Param_AttachPointName); \
		P_GET_STRUCT(FVector,Z_Param_Location); \
		P_GET_PROPERTY(UByteProperty,Z_Param_LocationType); \
		P_GET_UBOOL(Z_Param_bStopWhenAttachedToDestroyed); \
		P_GET_UBOOL(Z_Param_bAutoPlay); \
		P_FINISH; \
		*(UFMODAudioComponent**)Z_Param__Result=UFMODBlueprintStatics::PlayEventAttached(Z_Param_Event,Z_Param_AttachToComponent,Z_Param_AttachPointName,Z_Param_Location,EAttachLocation::Type(Z_Param_LocationType),Z_Param_bStopWhenAttachedToDestroyed,Z_Param_bAutoPlay); \
	} \
 \
	DECLARE_FUNCTION(execPlayEventAtLocation) \
	{ \
		P_GET_OBJECT(UObject,Z_Param_WorldContextObject); \
		P_GET_OBJECT(UFMODEvent,Z_Param_Event); \
		P_GET_STRUCT_REF(FTransform,Z_Param_Out_Location); \
		P_GET_UBOOL(Z_Param_bAutoPlay); \
		P_FINISH; \
		*(FFMODEventInstance*)Z_Param__Result=UFMODBlueprintStatics::PlayEventAtLocation(Z_Param_WorldContextObject,Z_Param_Event,Z_Param_Out_Location,Z_Param_bAutoPlay); \
	} \
 \
	DECLARE_FUNCTION(execPlayEvent2D) \
	{ \
		P_GET_OBJECT(UObject,Z_Param_WorldContextObject); \
		P_GET_OBJECT(UFMODEvent,Z_Param_Event); \
		P_GET_UBOOL(Z_Param_bAutoPlay); \
		P_FINISH; \
		*(FFMODEventInstance*)Z_Param__Result=UFMODBlueprintStatics::PlayEvent2D(Z_Param_WorldContextObject,Z_Param_Event,Z_Param_bAutoPlay); \
	}


#define Two31_Plugins_FMODStudio_Source_FMODStudio_Classes_FMODBlueprintStatics_h_35_INCLASS_NO_PURE_DECLS \
	private: \
	static void StaticRegisterNativesUFMODBlueprintStatics(); \
	friend FMODSTUDIO_API class UClass* Z_Construct_UClass_UFMODBlueprintStatics(); \
	public: \
	DECLARE_CLASS(UFMODBlueprintStatics, UBlueprintFunctionLibrary, COMPILED_IN_FLAGS(0), 0, FMODStudio, NO_API) \
	DECLARE_SERIALIZER(UFMODBlueprintStatics) \
	/** Indicates whether the class is compiled into the engine */    enum {IsIntrinsic=COMPILED_IN_INTRINSIC}; \
	virtual UObject* _getUObject() const override { return const_cast<UFMODBlueprintStatics*>(this); }


#define Two31_Plugins_FMODStudio_Source_FMODStudio_Classes_FMODBlueprintStatics_h_35_INCLASS \
	private: \
	static void StaticRegisterNativesUFMODBlueprintStatics(); \
	friend FMODSTUDIO_API class UClass* Z_Construct_UClass_UFMODBlueprintStatics(); \
	public: \
	DECLARE_CLASS(UFMODBlueprintStatics, UBlueprintFunctionLibrary, COMPILED_IN_FLAGS(0), 0, FMODStudio, NO_API) \
	DECLARE_SERIALIZER(UFMODBlueprintStatics) \
	/** Indicates whether the class is compiled into the engine */    enum {IsIntrinsic=COMPILED_IN_INTRINSIC}; \
	virtual UObject* _getUObject() const override { return const_cast<UFMODBlueprintStatics*>(this); }


#define Two31_Plugins_FMODStudio_Source_FMODStudio_Classes_FMODBlueprintStatics_h_35_STANDARD_CONSTRUCTORS \
	/** Standard constructor, called after all reflected properties have been initialized */ \
	NO_API UFMODBlueprintStatics(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get()); \
	DEFINE_DEFAULT_OBJECT_INITIALIZER_CONSTRUCTOR_CALL(UFMODBlueprintStatics) \
	DECLARE_VTABLE_PTR_HELPER_CTOR(NO_API, UFMODBlueprintStatics); \
DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(UFMODBlueprintStatics); \
private: \
	/** Private copy-constructor, should never be used */ \
	NO_API UFMODBlueprintStatics(const UFMODBlueprintStatics& InCopy); \
public:


#define Two31_Plugins_FMODStudio_Source_FMODStudio_Classes_FMODBlueprintStatics_h_35_ENHANCED_CONSTRUCTORS \
	/** Standard constructor, called after all reflected properties have been initialized */ \
	NO_API UFMODBlueprintStatics(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get()) : Super(ObjectInitializer) { }; \
private: \
	/** Private copy-constructor, should never be used */ \
	NO_API UFMODBlueprintStatics(const UFMODBlueprintStatics& InCopy); \
public: \
	DECLARE_VTABLE_PTR_HELPER_CTOR(NO_API, UFMODBlueprintStatics); \
DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(UFMODBlueprintStatics); \
	DEFINE_DEFAULT_OBJECT_INITIALIZER_CONSTRUCTOR_CALL(UFMODBlueprintStatics)


#define Two31_Plugins_FMODStudio_Source_FMODStudio_Classes_FMODBlueprintStatics_h_32_PROLOG
#define Two31_Plugins_FMODStudio_Source_FMODStudio_Classes_FMODBlueprintStatics_h_35_GENERATED_BODY_LEGACY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	Two31_Plugins_FMODStudio_Source_FMODStudio_Classes_FMODBlueprintStatics_h_35_RPC_WRAPPERS \
	Two31_Plugins_FMODStudio_Source_FMODStudio_Classes_FMODBlueprintStatics_h_35_INCLASS \
	Two31_Plugins_FMODStudio_Source_FMODStudio_Classes_FMODBlueprintStatics_h_35_STANDARD_CONSTRUCTORS \
public: \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


#define Two31_Plugins_FMODStudio_Source_FMODStudio_Classes_FMODBlueprintStatics_h_35_GENERATED_BODY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	Two31_Plugins_FMODStudio_Source_FMODStudio_Classes_FMODBlueprintStatics_h_35_RPC_WRAPPERS_NO_PURE_DECLS \
	Two31_Plugins_FMODStudio_Source_FMODStudio_Classes_FMODBlueprintStatics_h_35_INCLASS_NO_PURE_DECLS \
	Two31_Plugins_FMODStudio_Source_FMODStudio_Classes_FMODBlueprintStatics_h_35_ENHANCED_CONSTRUCTORS \
static_assert(false, "Unknown access specifier for GENERATED_BODY() macro in class FMODBlueprintStatics."); \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


#undef CURRENT_FILE_ID
#define CURRENT_FILE_ID Two31_Plugins_FMODStudio_Source_FMODStudio_Classes_FMODBlueprintStatics_h


PRAGMA_ENABLE_DEPRECATION_WARNINGS