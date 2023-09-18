// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/ObjectMacros.h"
#include "UObject/ScriptMacros.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS
#ifdef S1_S1GameInstance_generated_h
#error "S1GameInstance.generated.h already included, missing '#pragma once' in S1GameInstance.h"
#endif
#define S1_S1GameInstance_generated_h

#define FID_S1_Source_S1_S1GameInstance_h_16_SPARSE_DATA
#define FID_S1_Source_S1_S1GameInstance_h_16_RPC_WRAPPERS \
 \
	DECLARE_FUNCTION(execHandleRecvPackets); \
	DECLARE_FUNCTION(execDisconnectFromGameServer); \
	DECLARE_FUNCTION(execConnectToGameServer);


#define FID_S1_Source_S1_S1GameInstance_h_16_RPC_WRAPPERS_NO_PURE_DECLS \
 \
	DECLARE_FUNCTION(execHandleRecvPackets); \
	DECLARE_FUNCTION(execDisconnectFromGameServer); \
	DECLARE_FUNCTION(execConnectToGameServer);


#define FID_S1_Source_S1_S1GameInstance_h_16_INCLASS_NO_PURE_DECLS \
private: \
	static void StaticRegisterNativesUS1GameInstance(); \
	friend struct Z_Construct_UClass_US1GameInstance_Statics; \
public: \
	DECLARE_CLASS(US1GameInstance, UGameInstance, COMPILED_IN_FLAGS(0 | CLASS_Transient), CASTCLASS_None, TEXT("/Script/S1"), NO_API) \
	DECLARE_SERIALIZER(US1GameInstance)


#define FID_S1_Source_S1_S1GameInstance_h_16_INCLASS \
private: \
	static void StaticRegisterNativesUS1GameInstance(); \
	friend struct Z_Construct_UClass_US1GameInstance_Statics; \
public: \
	DECLARE_CLASS(US1GameInstance, UGameInstance, COMPILED_IN_FLAGS(0 | CLASS_Transient), CASTCLASS_None, TEXT("/Script/S1"), NO_API) \
	DECLARE_SERIALIZER(US1GameInstance)


#define FID_S1_Source_S1_S1GameInstance_h_16_STANDARD_CONSTRUCTORS \
	/** Standard constructor, called after all reflected properties have been initialized */ \
	NO_API US1GameInstance(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get()); \
	DEFINE_DEFAULT_OBJECT_INITIALIZER_CONSTRUCTOR_CALL(US1GameInstance) \
	DECLARE_VTABLE_PTR_HELPER_CTOR(NO_API, US1GameInstance); \
	DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(US1GameInstance); \
private: \
	/** Private move- and copy-constructors, should never be used */ \
	NO_API US1GameInstance(US1GameInstance&&); \
	NO_API US1GameInstance(const US1GameInstance&); \
public:


#define FID_S1_Source_S1_S1GameInstance_h_16_ENHANCED_CONSTRUCTORS \
	/** Standard constructor, called after all reflected properties have been initialized */ \
	NO_API US1GameInstance(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get()) : Super(ObjectInitializer) { }; \
private: \
	/** Private move- and copy-constructors, should never be used */ \
	NO_API US1GameInstance(US1GameInstance&&); \
	NO_API US1GameInstance(const US1GameInstance&); \
public: \
	DECLARE_VTABLE_PTR_HELPER_CTOR(NO_API, US1GameInstance); \
	DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(US1GameInstance); \
	DEFINE_DEFAULT_OBJECT_INITIALIZER_CONSTRUCTOR_CALL(US1GameInstance)


#define FID_S1_Source_S1_S1GameInstance_h_13_PROLOG
#define FID_S1_Source_S1_S1GameInstance_h_16_GENERATED_BODY_LEGACY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	FID_S1_Source_S1_S1GameInstance_h_16_SPARSE_DATA \
	FID_S1_Source_S1_S1GameInstance_h_16_RPC_WRAPPERS \
	FID_S1_Source_S1_S1GameInstance_h_16_INCLASS \
	FID_S1_Source_S1_S1GameInstance_h_16_STANDARD_CONSTRUCTORS \
public: \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


#define FID_S1_Source_S1_S1GameInstance_h_16_GENERATED_BODY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	FID_S1_Source_S1_S1GameInstance_h_16_SPARSE_DATA \
	FID_S1_Source_S1_S1GameInstance_h_16_RPC_WRAPPERS_NO_PURE_DECLS \
	FID_S1_Source_S1_S1GameInstance_h_16_INCLASS_NO_PURE_DECLS \
	FID_S1_Source_S1_S1GameInstance_h_16_ENHANCED_CONSTRUCTORS \
private: \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


template<> S1_API UClass* StaticClass<class US1GameInstance>();

#undef CURRENT_FILE_ID
#define CURRENT_FILE_ID FID_S1_Source_S1_S1GameInstance_h


PRAGMA_ENABLE_DEPRECATION_WARNINGS
