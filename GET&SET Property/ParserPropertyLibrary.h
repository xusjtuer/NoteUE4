// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "ParserPropertyLibrary.generated.h"

// Declare General Log Category, header file .h
DECLARE_LOG_CATEGORY_EXTERN(LogParserPropertyLibrary, Log, All);


/**
 *
 */
UCLASS()
class ALGORITHMSYSTEM_API UParserPropertyLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()


public:
	/**
	 * Get or Set object PROPERTY value by property name
	 *
	 * @param  Object, object that owns this PROPERTY
	 * @param  PropertyName, property name
	 * @param  Value(return), save returned object property(Get Operation) as well as indicate property type
	 * @param  bSetter, If true, write Value to object property(Set operation). Otherwise, read object property and assign it to Value(Get operation)
	 */
	UFUNCTION(BlueprintCallable, CustomThunk, Category = "Utilities|Variables", meta = (CustomStructureParam = "Value", AutoCreateRefTerm = "Value", DisplayName = "GET/SET (Property)", CompactNodeTitle = "GET/SET"))
		static void AccessPropertyByName(UObject* Object, FName PropertyName, const int32& Value, bool bSetter = true);

	/**
	 * Get or Set object ARRAY value by property name
	 *
	 * @param  Object, object that owns this ARRAY
	 * @param  PropertyName, array property name
	 * @param  Value, save returned object array value(Get Operation) as well as indicate array type
	 * @param  bSetter, If true, write Value to object array(Set operation). Otherwise, read object array and assign it to Value(Get operation)
	 */
	UFUNCTION(BlueprintCallable, CustomThunk, Category = "Utilities|Array", meta = (ArrayParm = "Value", ArrayTypeDependentParams = "Value", DisplayName = "GET/SET (array)", CompactNodeTitle = "GET/SET"))
		static void AccessArrayByName(UObject* Object, FName PropertyName, const TArray<int32>& Value, bool bSetter = true);

	/**
	 * Get or Set object SET value by property name
	 *
	 * @param  Object, object that owns this SET
	 * @param  PropertyName, set property name
	 * @param  Value(return), save returned set value(Get Operation) as well as indicate set type
	 * @param  bSetter, If true, write Value to object set(Set operation). Otherwise, read object set and assign it to Value(Get operation)
	 */
	UFUNCTION(BlueprintCallable, CustomThunk, Category = "Utilities|Set", meta = (SetParam = "Value", DisplayName = "GET/SET (set)", CompactNodeTitle = "GET/SET"))
		static void AccessSetByName(UObject* Object, FName PropertyName, const TSet<int32>& Value, bool bSetter = true);

	/**
	 * Get or Set object MAP value by property name
	 *
	 * @param  Object, object that owns this MAP
	 * @param  PropertyName, map property name
	 * @param  Value(return), save returned map value(Get Operation) as well as indicate map type
	 * @param  bSetter, If true, write Value to object map(Set operation). Otherwise, read object map and assign it to Value(Get operation)
	 */
	UFUNCTION(BlueprintCallable, CustomThunk, Category = "Utilities|Map", meta = (MapParam = "Value", DisplayName = "GET/SET (map)", CompactNodeTitle = "GET/SET"))
		static void AccessMapByName(UObject* Object, FName PropertyName, const TMap<int32, int32>& Value, bool bSetter = true);

public: //exec function

	// get property
	DECLARE_FUNCTION(execAccessPropertyByName)
	{
		P_GET_OBJECT(UObject, OwnerObject);
		P_GET_PROPERTY(UNameProperty, PropertyName);

		Stack.StepCompiledIn<UStructProperty>(NULL);
		void* SrcPropertyAddr = Stack.MostRecentPropertyAddress;
		/// Reference: Plugins\Experimental\StructBox\Source\StructBox\Classes\StructBoxLibrary.h -> execSetStructInBox
		UProperty* SrcProperty = Cast<UProperty>(Stack.MostRecentProperty);

		P_GET_UBOOL(bSetter);
		P_FINISH;

		P_NATIVE_BEGIN;
		Generic_AccessPropertyByName(OwnerObject, PropertyName, SrcPropertyAddr, SrcProperty, bSetter);
		P_NATIVE_END;
	}

	// get array
	DECLARE_FUNCTION(execAccessArrayByName)
	{
		P_GET_OBJECT(UObject, OwnerObject);
		P_GET_PROPERTY(UNameProperty, ArrayPropertyName);

		Stack.StepCompiledIn<UArrayProperty>(NULL);
		void* SrcArrayAddr = Stack.MostRecentPropertyAddress;
		UArrayProperty* SrcArrayProperty = Cast<UArrayProperty>(Stack.MostRecentProperty);

		P_GET_UBOOL(bSetter);
		P_FINISH;

		P_NATIVE_BEGIN;
		Generic_AcessArrayByName(OwnerObject, ArrayPropertyName, SrcArrayAddr, SrcArrayProperty, bSetter);
		P_NATIVE_END;
	}

	// get set
	DECLARE_FUNCTION(execAccessSetByName)
	{
		P_GET_OBJECT(UObject, OwnerObject);
		P_GET_PROPERTY(UNameProperty, SetPropertyName);

		Stack.MostRecentProperty = nullptr;
		Stack.StepCompiledIn<USetProperty>(NULL);
		void* SetAddr = Stack.MostRecentPropertyAddress;
		USetProperty* SetProperty = Cast<USetProperty>(Stack.MostRecentProperty);
		if (!SetProperty)
		{
			Stack.bArrayContextFailed = true;
			return;
		}

		P_GET_UBOOL(bSetter);
		P_FINISH;

		P_NATIVE_BEGIN;
		Generic_AcessSetByName(OwnerObject, SetPropertyName, SetAddr, SetProperty, bSetter);
		P_NATIVE_END;
	}

	// get map
	DECLARE_FUNCTION(execAccessMapByName)
	{
		P_GET_OBJECT(UObject, OwnerObject);
		P_GET_PROPERTY(UNameProperty, MapPropertyName);

		Stack.MostRecentProperty = nullptr;
		Stack.StepCompiledIn<UMapProperty>(NULL);
		void* SrcMapAddr = Stack.MostRecentPropertyAddress;
		UMapProperty* SrcMapProperty = Cast<UMapProperty>(Stack.MostRecentProperty);
		if (!SrcMapProperty)
		{
			Stack.bArrayContextFailed = true;
			return;
		}

		P_GET_UBOOL(bSetter);
		P_FINISH;

		P_NATIVE_BEGIN;
		Generic_AcessMapByName(OwnerObject, MapPropertyName, SrcMapAddr, SrcMapProperty, bSetter);
		P_NATIVE_END;
	}

public: // Generic function

	// Get or set a UPROPERTY of UObject by property name
	static void Generic_AccessPropertyByName(UObject* OwnerObject, FName PropertyName, void* SrcPropertyAddr, UProperty* SrcProperty, bool bSetter = true);

	// Get or set a ARRAY Property of UObject by property name
	static void Generic_AcessArrayByName(UObject* OwnerObject, FName ArrayPropertyName, void* SrcArrayAddr, const UArrayProperty* SrcArrayProperty, bool bSetter = true);

	// Get or set a SET Property of UObject by property name
	static void Generic_AcessSetByName(UObject* OwnerObject, FName SetPropertyName, void* SrcSetAddr, const USetProperty* SrcSetProperty, bool bSetter = true);

	// Get or set a MAP Property of UObject by property name
	static void Generic_AcessMapByName(UObject* OwnerObject, FName MapPropertyName, void* SrcMapAddr, const UMapProperty* SrcMapProperty, bool bSetter = true);

};
