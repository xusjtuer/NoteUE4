// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "K2UObjectFunctionLibrary.generated.h"


//General Log
DECLARE_LOG_CATEGORY_EXTERN(LogUK2UObjectFunctionLibrary, Log, All);

/**
 *
 */
UCLASS()
class AWESOMEBLUEPRINTS_API UK2UObjectFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	/** Set UObject single property value by name */
	UFUNCTION(BlueprintCallable, CustomThunk, meta = (BlueprintInternalUseOnly = "true", CustomStructureParam = "Value", AutoCreateRefTerm = "Value"))
		static void SetSingelPropertyByName(UObject* Object, FName PropertyName, const int32& Value);
	static void Generic_SetSingelPropertyByName(UObject* OwnerObject, FName PropertyName, FProperty* ValueProperty, void* ValueAddr);

	DECLARE_FUNCTION(execSetSingelPropertyByName)
	{
		P_GET_OBJECT(UObject, OwnerObject);
		P_GET_PROPERTY(FNameProperty, PropertyName);

		Stack.StepCompiledIn<FStructProperty>(NULL);
		FProperty* ValueProp = CastField<FProperty>(Stack.MostRecentProperty);
		void* ValueAddr = Stack.MostRecentPropertyAddress;

		P_FINISH;

		P_NATIVE_BEGIN;
		UK2UObjectFunctionLibrary::Generic_SetSingelPropertyByName(OwnerObject, PropertyName, ValueProp, ValueAddr);
		P_NATIVE_END;
	}


public: // Get Property Value
	/** Get UObject single property value by name */
	UFUNCTION(BlueprintCallable, CustomThunk, meta = (BlueprintInternalUseOnly = "true", CustomStructureParam = "Value", AutoCreateRefTerm = "Value"))
		static bool GetSingelPropertyByName(UObject* Object, FName PropertyName, int32& Value);

	/** Get UObject Array property value by name */
	UFUNCTION(BlueprintCallable, CustomThunk, meta = (BlueprintInternalUseOnly = "true", ArrayParm = "Value", ArrayTypeDependentParams = "Value"))
		static bool GetArrayPropertyByName(UObject* Object, FName PropertyName, TArray<int32>& Value);

	/** Get UObject Map property value by name */
	UFUNCTION(BlueprintCallable, CustomThunk, meta = (BlueprintInternalUseOnly = "true", MapParam = "Value"))
		static bool GetMapPropertyByName(UObject* Object, FName PropertyName, TMap<int32, int32>& Value);

	/** Get UObject Set property value by name */
	UFUNCTION(BlueprintCallable, CustomThunk, meta = (BlueprintInternalUseOnly = "true", SetParam = "Value"))
		static bool GetSetPropertyByName(UObject* Object, FName PropertyName, TSet<int32>& Value);

	static bool Generic_GetPropertyByName(UObject* OwnerObject, FName PropertyName, FProperty* OutValueProperty, void* OutValueAddr);

	DECLARE_FUNCTION(execGetSingelPropertyByName)
	{
		P_GET_OBJECT(UObject, OwnerObject);
		P_GET_PROPERTY(FNameProperty, PropertyName);

		Stack.StepCompiledIn<FStructProperty>(NULL);
		FProperty* ValueProp = CastField<FProperty>(Stack.MostRecentProperty);
		void* ValueAddr = Stack.MostRecentPropertyAddress;

		P_FINISH;

		P_NATIVE_BEGIN;
		*(bool*)RESULT_PARAM = UK2UObjectFunctionLibrary::Generic_GetPropertyByName(OwnerObject, PropertyName, ValueProp, ValueAddr);
		P_NATIVE_END;
	}

	DECLARE_FUNCTION(execGetArrayPropertyByName)
	{
		P_GET_OBJECT(UObject, OwnerObject);
		P_GET_PROPERTY(FNameProperty, PropertyName);

		Stack.StepCompiledIn<FArrayProperty>(NULL);
		void* ValueAddr = Stack.MostRecentPropertyAddress;
		FArrayProperty* ValueProp = CastField<FArrayProperty>(Stack.MostRecentProperty);
		if (!ValueProp)
		{
			Stack.bArrayContextFailed = true;
			return;
		}
		P_FINISH;

		P_NATIVE_BEGIN;
		*(bool*)RESULT_PARAM = UK2UObjectFunctionLibrary::Generic_GetPropertyByName(OwnerObject, PropertyName, ValueProp, ValueAddr);
		P_NATIVE_END;
	}

	DECLARE_FUNCTION(execGetMapPropertyByName)
	{
		P_GET_OBJECT(UObject, OwnerObject);
		P_GET_PROPERTY(FNameProperty, PropertyName);

		Stack.StepCompiledIn<FMapProperty>(NULL);
		void* ValueAddr = Stack.MostRecentPropertyAddress;
		FMapProperty* ValueProp = CastField<FMapProperty>(Stack.MostRecentProperty);

		if (!ValueProp)
		{
			Stack.bArrayContextFailed = true;
			return;
		}
		P_FINISH;

		P_NATIVE_BEGIN;
		*(bool*)RESULT_PARAM = UK2UObjectFunctionLibrary::Generic_GetPropertyByName(OwnerObject, PropertyName, ValueProp, ValueAddr);
		P_NATIVE_END;
	}

	DECLARE_FUNCTION(execGetSetPropertyByName)
	{
		P_GET_OBJECT(UObject, OwnerObject);
		P_GET_PROPERTY(FNameProperty, PropertyName);

		Stack.StepCompiledIn<FSetProperty>(NULL);
		void* ValueAddr = Stack.MostRecentPropertyAddress;
		FSetProperty* ValueProp = CastField<FSetProperty>(Stack.MostRecentProperty);

		if (!ValueProp)
		{
			Stack.bArrayContextFailed = true;
			return;
		}
		P_FINISH;

		P_NATIVE_BEGIN;
		*(bool*)RESULT_PARAM = UK2UObjectFunctionLibrary::Generic_GetPropertyByName(OwnerObject, PropertyName, ValueProp, ValueAddr);
		P_NATIVE_END;
	}

};
