// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "S1/S1GameModeBase.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeS1GameModeBase() {}
// Cross Module References
	S1_API UClass* Z_Construct_UClass_AS1GameModeBase_NoRegister();
	S1_API UClass* Z_Construct_UClass_AS1GameModeBase();
	ENGINE_API UClass* Z_Construct_UClass_AGameModeBase();
	UPackage* Z_Construct_UPackage__Script_S1();
// End Cross Module References
	void AS1GameModeBase::StaticRegisterNativesAS1GameModeBase()
	{
	}
	IMPLEMENT_CLASS_NO_AUTO_REGISTRATION(AS1GameModeBase);
	UClass* Z_Construct_UClass_AS1GameModeBase_NoRegister()
	{
		return AS1GameModeBase::StaticClass();
	}
	struct Z_Construct_UClass_AS1GameModeBase_Statics
	{
		static UObject* (*const DependentSingletons[])();
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[];
#endif
		static const FCppClassTypeInfoStatic StaticCppClassTypeInfo;
		static const UECodeGen_Private::FClassParams ClassParams;
	};
	UObject* (*const Z_Construct_UClass_AS1GameModeBase_Statics::DependentSingletons[])() = {
		(UObject* (*)())Z_Construct_UClass_AGameModeBase,
		(UObject* (*)())Z_Construct_UPackage__Script_S1,
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_AS1GameModeBase_Statics::Class_MetaDataParams[] = {
		{ "Comment", "/**\n * \n */" },
		{ "HideCategories", "Info Rendering MovementReplication Replication Actor Input Movement Collision Rendering HLOD WorldPartition DataLayers Transformation" },
		{ "IncludePath", "S1GameModeBase.h" },
		{ "ModuleRelativePath", "S1GameModeBase.h" },
		{ "ShowCategories", "Input|MouseInput Input|TouchInput" },
	};
#endif
	const FCppClassTypeInfoStatic Z_Construct_UClass_AS1GameModeBase_Statics::StaticCppClassTypeInfo = {
		TCppClassTypeTraits<AS1GameModeBase>::IsAbstract,
	};
	const UECodeGen_Private::FClassParams Z_Construct_UClass_AS1GameModeBase_Statics::ClassParams = {
		&AS1GameModeBase::StaticClass,
		"Game",
		&StaticCppClassTypeInfo,
		DependentSingletons,
		nullptr,
		nullptr,
		nullptr,
		UE_ARRAY_COUNT(DependentSingletons),
		0,
		0,
		0,
		0x009002ACu,
		METADATA_PARAMS(Z_Construct_UClass_AS1GameModeBase_Statics::Class_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UClass_AS1GameModeBase_Statics::Class_MetaDataParams))
	};
	UClass* Z_Construct_UClass_AS1GameModeBase()
	{
		if (!Z_Registration_Info_UClass_AS1GameModeBase.OuterSingleton)
		{
			UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_AS1GameModeBase.OuterSingleton, Z_Construct_UClass_AS1GameModeBase_Statics::ClassParams);
		}
		return Z_Registration_Info_UClass_AS1GameModeBase.OuterSingleton;
	}
	template<> S1_API UClass* StaticClass<AS1GameModeBase>()
	{
		return AS1GameModeBase::StaticClass();
	}
	DEFINE_VTABLE_PTR_HELPER_CTOR(AS1GameModeBase);
	struct Z_CompiledInDeferFile_FID_S1_Source_S1_S1GameModeBase_h_Statics
	{
		static const FClassRegisterCompiledInInfo ClassInfo[];
	};
	const FClassRegisterCompiledInInfo Z_CompiledInDeferFile_FID_S1_Source_S1_S1GameModeBase_h_Statics::ClassInfo[] = {
		{ Z_Construct_UClass_AS1GameModeBase, AS1GameModeBase::StaticClass, TEXT("AS1GameModeBase"), &Z_Registration_Info_UClass_AS1GameModeBase, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(AS1GameModeBase), 4169078009U) },
	};
	static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_S1_Source_S1_S1GameModeBase_h_3254610(TEXT("/Script/S1"),
		Z_CompiledInDeferFile_FID_S1_Source_S1_S1GameModeBase_h_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_S1_Source_S1_S1GameModeBase_h_Statics::ClassInfo),
		nullptr, 0,
		nullptr, 0);
PRAGMA_ENABLE_DEPRECATION_WARNINGS
