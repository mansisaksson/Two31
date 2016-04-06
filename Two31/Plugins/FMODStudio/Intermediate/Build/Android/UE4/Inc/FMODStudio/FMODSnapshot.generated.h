// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	C++ class header boilerplate exported from UnrealHeaderTool.
	This is automatically generated by the tools.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "ObjectBase.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS
#ifdef FMODSTUDIO_FMODSnapshot_generated_h
#error "FMODSnapshot.generated.h already included, missing '#pragma once' in FMODSnapshot.h"
#endif
#define FMODSTUDIO_FMODSnapshot_generated_h

#define dev_ue4_FMODStudio_Source_FMODStudio_Classes_FMODSnapshot_h_20_RPC_WRAPPERS
#define dev_ue4_FMODStudio_Source_FMODStudio_Classes_FMODSnapshot_h_20_RPC_WRAPPERS_NO_PURE_DECLS
#define dev_ue4_FMODStudio_Source_FMODStudio_Classes_FMODSnapshot_h_20_INCLASS_NO_PURE_DECLS \
	private: \
	static void StaticRegisterNativesUFMODSnapshot(); \
	friend FMODSTUDIO_API class UClass* Z_Construct_UClass_UFMODSnapshot(); \
	public: \
	DECLARE_CLASS(UFMODSnapshot, UFMODEvent, COMPILED_IN_FLAGS(0), 0, FMODStudio, NO_API) \
	DECLARE_SERIALIZER(UFMODSnapshot) \
	/** Indicates whether the class is compiled into the engine */    enum {IsIntrinsic=COMPILED_IN_INTRINSIC}; \
	virtual UObject* _getUObject() const override { return const_cast<UFMODSnapshot*>(this); }


#define dev_ue4_FMODStudio_Source_FMODStudio_Classes_FMODSnapshot_h_20_INCLASS \
	private: \
	static void StaticRegisterNativesUFMODSnapshot(); \
	friend FMODSTUDIO_API class UClass* Z_Construct_UClass_UFMODSnapshot(); \
	public: \
	DECLARE_CLASS(UFMODSnapshot, UFMODEvent, COMPILED_IN_FLAGS(0), 0, FMODStudio, NO_API) \
	DECLARE_SERIALIZER(UFMODSnapshot) \
	/** Indicates whether the class is compiled into the engine */    enum {IsIntrinsic=COMPILED_IN_INTRINSIC}; \
	virtual UObject* _getUObject() const override { return const_cast<UFMODSnapshot*>(this); }


#define dev_ue4_FMODStudio_Source_FMODStudio_Classes_FMODSnapshot_h_20_STANDARD_CONSTRUCTORS \
	/** Standard constructor, called after all reflected properties have been initialized */ \
	NO_API UFMODSnapshot(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get()); \
	DEFINE_DEFAULT_OBJECT_INITIALIZER_CONSTRUCTOR_CALL(UFMODSnapshot) \
	DECLARE_VTABLE_PTR_HELPER_CTOR(NO_API, UFMODSnapshot); \
DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(UFMODSnapshot); \
private: \
	/** Private copy-constructor, should never be used */ \
	NO_API UFMODSnapshot(const UFMODSnapshot& InCopy); \
public:


#define dev_ue4_FMODStudio_Source_FMODStudio_Classes_FMODSnapshot_h_20_ENHANCED_CONSTRUCTORS \
	/** Standard constructor, called after all reflected properties have been initialized */ \
	NO_API UFMODSnapshot(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get()) : Super(ObjectInitializer) { }; \
private: \
	/** Private copy-constructor, should never be used */ \
	NO_API UFMODSnapshot(const UFMODSnapshot& InCopy); \
public: \
	DECLARE_VTABLE_PTR_HELPER_CTOR(NO_API, UFMODSnapshot); \
DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(UFMODSnapshot); \
	DEFINE_DEFAULT_OBJECT_INITIALIZER_CONSTRUCTOR_CALL(UFMODSnapshot)


#define dev_ue4_FMODStudio_Source_FMODStudio_Classes_FMODSnapshot_h_17_PROLOG
#define dev_ue4_FMODStudio_Source_FMODStudio_Classes_FMODSnapshot_h_20_GENERATED_BODY_LEGACY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	dev_ue4_FMODStudio_Source_FMODStudio_Classes_FMODSnapshot_h_20_RPC_WRAPPERS \
	dev_ue4_FMODStudio_Source_FMODStudio_Classes_FMODSnapshot_h_20_INCLASS \
	dev_ue4_FMODStudio_Source_FMODStudio_Classes_FMODSnapshot_h_20_STANDARD_CONSTRUCTORS \
public: \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


#define dev_ue4_FMODStudio_Source_FMODStudio_Classes_FMODSnapshot_h_20_GENERATED_BODY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	dev_ue4_FMODStudio_Source_FMODStudio_Classes_FMODSnapshot_h_20_RPC_WRAPPERS_NO_PURE_DECLS \
	dev_ue4_FMODStudio_Source_FMODStudio_Classes_FMODSnapshot_h_20_INCLASS_NO_PURE_DECLS \
	dev_ue4_FMODStudio_Source_FMODStudio_Classes_FMODSnapshot_h_20_ENHANCED_CONSTRUCTORS \
static_assert(false, "Unknown access specifier for GENERATED_BODY() macro in class FMODSnapshot."); \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


#undef CURRENT_FILE_ID
#define CURRENT_FILE_ID dev_ue4_FMODStudio_Source_FMODStudio_Classes_FMODSnapshot_h


PRAGMA_ENABLE_DEPRECATION_WARNINGS
