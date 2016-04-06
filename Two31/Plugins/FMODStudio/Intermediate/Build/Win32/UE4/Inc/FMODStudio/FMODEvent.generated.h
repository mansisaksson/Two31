// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	C++ class header boilerplate exported from UnrealHeaderTool.
	This is automatically generated by the tools.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "ObjectBase.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS
#ifdef FMODSTUDIO_FMODEvent_generated_h
#error "FMODEvent.generated.h already included, missing '#pragma once' in FMODEvent.h"
#endif
#define FMODSTUDIO_FMODEvent_generated_h

#define dev_ue4_FMODStudio_Source_FMODStudio_Classes_FMODEvent_h_20_RPC_WRAPPERS
#define dev_ue4_FMODStudio_Source_FMODStudio_Classes_FMODEvent_h_20_RPC_WRAPPERS_NO_PURE_DECLS
#define dev_ue4_FMODStudio_Source_FMODStudio_Classes_FMODEvent_h_20_INCLASS_NO_PURE_DECLS \
	private: \
	static void StaticRegisterNativesUFMODEvent(); \
	friend FMODSTUDIO_API class UClass* Z_Construct_UClass_UFMODEvent(); \
	public: \
	DECLARE_CLASS(UFMODEvent, UFMODAsset, COMPILED_IN_FLAGS(0), 0, FMODStudio, NO_API) \
	DECLARE_SERIALIZER(UFMODEvent) \
	/** Indicates whether the class is compiled into the engine */    enum {IsIntrinsic=COMPILED_IN_INTRINSIC}; \
	virtual UObject* _getUObject() const override { return const_cast<UFMODEvent*>(this); }


#define dev_ue4_FMODStudio_Source_FMODStudio_Classes_FMODEvent_h_20_INCLASS \
	private: \
	static void StaticRegisterNativesUFMODEvent(); \
	friend FMODSTUDIO_API class UClass* Z_Construct_UClass_UFMODEvent(); \
	public: \
	DECLARE_CLASS(UFMODEvent, UFMODAsset, COMPILED_IN_FLAGS(0), 0, FMODStudio, NO_API) \
	DECLARE_SERIALIZER(UFMODEvent) \
	/** Indicates whether the class is compiled into the engine */    enum {IsIntrinsic=COMPILED_IN_INTRINSIC}; \
	virtual UObject* _getUObject() const override { return const_cast<UFMODEvent*>(this); }


#define dev_ue4_FMODStudio_Source_FMODStudio_Classes_FMODEvent_h_20_STANDARD_CONSTRUCTORS \
	/** Standard constructor, called after all reflected properties have been initialized */ \
	NO_API UFMODEvent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get()); \
	DEFINE_DEFAULT_OBJECT_INITIALIZER_CONSTRUCTOR_CALL(UFMODEvent) \
	DECLARE_VTABLE_PTR_HELPER_CTOR(NO_API, UFMODEvent); \
DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(UFMODEvent); \
private: \
	/** Private copy-constructor, should never be used */ \
	NO_API UFMODEvent(const UFMODEvent& InCopy); \
public:


#define dev_ue4_FMODStudio_Source_FMODStudio_Classes_FMODEvent_h_20_ENHANCED_CONSTRUCTORS \
	/** Standard constructor, called after all reflected properties have been initialized */ \
	NO_API UFMODEvent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get()) : Super(ObjectInitializer) { }; \
private: \
	/** Private copy-constructor, should never be used */ \
	NO_API UFMODEvent(const UFMODEvent& InCopy); \
public: \
	DECLARE_VTABLE_PTR_HELPER_CTOR(NO_API, UFMODEvent); \
DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(UFMODEvent); \
	DEFINE_DEFAULT_OBJECT_INITIALIZER_CONSTRUCTOR_CALL(UFMODEvent)


#define dev_ue4_FMODStudio_Source_FMODStudio_Classes_FMODEvent_h_17_PROLOG
#define dev_ue4_FMODStudio_Source_FMODStudio_Classes_FMODEvent_h_20_GENERATED_BODY_LEGACY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	dev_ue4_FMODStudio_Source_FMODStudio_Classes_FMODEvent_h_20_RPC_WRAPPERS \
	dev_ue4_FMODStudio_Source_FMODStudio_Classes_FMODEvent_h_20_INCLASS \
	dev_ue4_FMODStudio_Source_FMODStudio_Classes_FMODEvent_h_20_STANDARD_CONSTRUCTORS \
public: \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


#define dev_ue4_FMODStudio_Source_FMODStudio_Classes_FMODEvent_h_20_GENERATED_BODY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	dev_ue4_FMODStudio_Source_FMODStudio_Classes_FMODEvent_h_20_RPC_WRAPPERS_NO_PURE_DECLS \
	dev_ue4_FMODStudio_Source_FMODStudio_Classes_FMODEvent_h_20_INCLASS_NO_PURE_DECLS \
	dev_ue4_FMODStudio_Source_FMODStudio_Classes_FMODEvent_h_20_ENHANCED_CONSTRUCTORS \
static_assert(false, "Unknown access specifier for GENERATED_BODY() macro in class FMODEvent."); \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


#undef CURRENT_FILE_ID
#define CURRENT_FILE_ID dev_ue4_FMODStudio_Source_FMODStudio_Classes_FMODEvent_h


PRAGMA_ENABLE_DEPRECATION_WARNINGS
