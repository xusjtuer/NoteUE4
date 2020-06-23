// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "BlueprintVariadicLibrary.generated.h"

/**
 *
 */
UCLASS()
class NOTE425_API UBlueprintVariadicLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()


public: // Wildcard property

	// Declare a function with an wildcard parameter.
	UFUNCTION(BlueprintCallable, CustomThunk, meta = (CustomStructureParam = "Value"), Category = "Utilities|Variadic")
		static void PropertyFunction1(const int32& Value);
	DECLARE_FUNCTION(execPropertyFunction1) {}

	// The variable marked by "CustomStructureParam" would be a placeholder.
	UFUNCTION(BlueprintCallable, CustomThunk, meta = (CustomStructureParam = "Value"), Category = "Utilities|Variadic")
		static void PropertyFunction2(UProperty* Value);
	DECLARE_FUNCTION(execPropertyFunction2) {}

	// Declare a function with multiple wildcard parameters.
	UFUNCTION(BlueprintCallable, CustomThunk, meta = (CustomStructureParam = "Value1,Value2,Value"), Category = "Utilities|Variadic")
		static void PropertyFunction3(const int32& Value1, const int32& Value2, int32& Value);
	DECLARE_FUNCTION(execPropertyFunction3) {}

	// Declare a function with incorrect delimiter        ---> Bad case <---
	UFUNCTION(BlueprintCallable, CustomThunk, meta = (CustomStructureParam = "Value1|Value2"), Category = "Utilities|Variadic")
		static void PropertyFunction4(const int32& Value1, const int32& Value2);
	DECLARE_FUNCTION(execPropertyFunction4) {}

	// Declare a function with one incorrect type  ---> Bad case <---
	UFUNCTION(BlueprintCallable, CustomThunk, meta = (CustomStructureParam = "Value"), Category = "Utilities|Variadic")
		static void PropertyFunction5(const TArray<int32>& Value);
	DECLARE_FUNCTION(execPropertyFunction5) {}


public: // Wildcard TArray

	// Declare a function with one wildcard array parameter.
	UFUNCTION(BlueprintCallable, CustomThunk, meta = (ArrayParm = "Array"), Category = "Utilities|Variadic")
		static void ArrayFunction1(const TArray<int32>& Array);
	DECLARE_FUNCTION(execArrayFunction1) {}

	// Declare a function with one wildcard array parameter with an type dependent parameter .
	UFUNCTION(BlueprintCallable, CustomThunk, meta = (ArrayParm = "Array", ArrayTypeDependentParams = "Item"), Category = "Utilities|Variadic")
		static void ArrayFunction2(const TArray<int32>& Array, const int32& Item);
	DECLARE_FUNCTION(execArrayFunction2) {}

	// Declare a function with multiple wildcard array parameters.
	UFUNCTION(BlueprintCallable, CustomThunk, meta = (ArrayParm = "Array1,Array2,Array3", ArrayTypeDependentParams = "Array1,Array2,Array3"), Category = "Utilities|Variadic")
		static void ArrayFunction3(const TArray<int32>& Array1, const TArray<int32>& Array2, TArray<int32>& Array3);
	DECLARE_FUNCTION(execArrayFunction3) {}

	// Declare a function with multiple wildcard array parameters
	UFUNCTION(BlueprintCallable, CustomThunk, meta = (ArrayParm = "Array1,Array2,Array3", ArrayTypeDependentParams = "Array1,Array3"), Category = "Utilities|Variadic")
		static void ArrayFunction4(const TArray<int32>& Array1, const TArray<int32>& Array2, TArray<int32>& Array3);
	DECLARE_FUNCTION(execArrayFunction4) {}

	// Declare a function with multiple wildcard array parameters. ---> Bad case <---
	UFUNCTION(BlueprintCallable, CustomThunk, meta = (ArrayParm = "Array1|Array2|Array3"), Category = "Utilities|Variadic")
		static void ArrayFunction5(const TArray<int32>& Array1, const TArray<int32>& Array2, TArray<int32>& Array3);
	DECLARE_FUNCTION(execArrayFunction5) {}

public: // wildcard TMap

	// Declare a function with one wildcard array parameter.
	UFUNCTION(BlueprintCallable, CustomThunk, meta = (MapParam = "TargetMap"), Category = "Utilities|Variadic")
		static void MapFunciton1(const TMap<int32, int32>& TargetMap);
	DECLARE_FUNCTION(execMapFunciton1) {}

	// Declare a function with one wildcard map parameter with type dependent parameters.
	UFUNCTION(BlueprintCallable, CustomThunk, meta = (MapParam = "TargetMap", MapKeyParam = "Key", MapValueParam = "Value", AutoCreateRefTerm = "Key, Value"), Category = "Utilities|Variadic")
		static void MapFunciton2(const TMap<int32, int32>& TargetMap, const int32& Key, const int32& Value);
	DECLARE_FUNCTION(execMapFunciton2) {}

	//UNSUPPORTED ---> Declare a function with multiple wildcard map parameters.
	UFUNCTION(BlueprintCallable, CustomThunk, meta = (MapParam = "TargetMap,SourceMap,ReturnMap"), Category = "Utilities|Variadic")
		static void MapFunciton3(const TMap<int32, int32>& TargetMap, const TMap<int32, int32>& SourceMap, TMap<int32, int32>& ReturnMap);
	DECLARE_FUNCTION(execMapFunciton3) {}

	//UNSUPPORTED ---> Declare a function with multiple wildcard map parameters.
	UFUNCTION(BlueprintCallable, CustomThunk, meta = (MapParam = "TargetMap|SourceMap|ReturnMap"), Category = "Utilities|Variadic")
		static void MapFunciton4(const TMap<int32, int32>& TargetMap, const TMap<int32, int32>& SourceMap, TMap<int32, int32>& ReturnMap);
	DECLARE_FUNCTION(execMapFunciton4) {}


public:// wildcard Set

	// Declare a function with one wildcard Set parameter.
	UFUNCTION(BlueprintCallable, CustomThunk, meta = (SetParam = "TargetSet"), Category = "Utilities|Variadic")
		static void SetFunciton1(const TSet<int32>& TargetSet);
	DECLARE_FUNCTION(execSetFunciton1) {}

	// Declare a function with one wildcard Set parameter with type independent parameter.
	UFUNCTION(BlueprintCallable, CustomThunk, meta = (SetParam = "TargetSet,NewItem"), Category = "Utilities|Variadic")
		static void SetFunciton2(const TSet<int32>& TargetSet, const int32& NewItem);
	DECLARE_FUNCTION(execSetFunciton2) {}

	// Declare a function with one wildcard Set parameter with type dependent parameter.
	UFUNCTION(BlueprintCallable, CustomThunk, meta = (SetParam = "TargetSet|NewItem"), Category = "Utilities|Variadic")
		static void SetFunciton3(const TSet<int32>& TargetSet, const int32& NewItem);
	DECLARE_FUNCTION(execSetFunciton3) {}

	// Declare a function with one wildcard Set parameter and type dependent array parameter
	UFUNCTION(BlueprintCallable, CustomThunk, meta = (SetParam = "TargetSet|NewItems"), Category = "Utilities|Variadic")
		static void SetFunciton4(const TSet<int32>& TargetSet, const TArray<int32>& NewItems);
	DECLARE_FUNCTION(execSetFunciton4) {}

	// Declare a function with multiple wildcard Set parameters v1
	UFUNCTION(BlueprintCallable, CustomThunk, meta = (SetParam = "Set1|Set2,Set3"), Category = "Utilities|Variadic")
		static void SetFunciton5(const TSet<int32>& Set1, const TSet<int32>& Set2, TSet<int32>& Set3);
	DECLARE_FUNCTION(execSetFunciton5) {}

	// Declare a function with multiple wildcard Set parameters v2
	UFUNCTION(BlueprintCallable, CustomThunk, meta = (SetParam = "Set1|Set2|Set3"), Category = "Utilities|Variadic")
		static void SetFunciton6(const TSet<int32>& Set1, const TSet<int32>& Set2, TSet<int32>& Set3);
	DECLARE_FUNCTION(execSetFunciton6) {}




};
