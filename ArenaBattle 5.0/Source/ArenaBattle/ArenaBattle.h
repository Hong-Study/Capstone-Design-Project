// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EngineMinimal.h" // 원래 CoreMinial.h 로 넣어주지만 이것은 언리얼 오브젝트가 동작할 최소 기능만 선언된 헤더파일임

DECLARE_LOG_CATEGORY_EXTERN(ArenaBattle, Log, All)

#define ABLOG_CALLINFO (FString(__FUNCTION__) + TEXT("(") + FString::FromInt(__LINE__) + TEXT(")"))

#define ABLOG_S(Verbosity) UE_LOG(ArenaBattle, Verbosity, TEXT("%s"), *ABLOG_CALLINFO)

#define ABLOG(Verbosity,Format,...) UE_LOG(ArenaBattle,Verbosity,TEXT("%s%s"),*ABLOG_CALLINFO,*FString::Printf(Format,##__VA_ARGS__))

#define ABCHECK(Expr, ...){if(!(Expr)) {ABLOG(Error,TEXT("ASSERTION : %s"),TEXT("'"#Expr"'")); return __VA_ARGS__; } }

