// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "JsonObjectConverter.h"
#include "MiscAlgorithm.generated.h"

/** Declare General Log Category, header file .h */
DECLARE_LOG_CATEGORY_EXTERN(LogMiscAlgorithm, Log, All);

// Expand Enum As Execs
UENUM()
enum class EEvaluateArray : uint8
{
	/** Array length > 0. */
	IsValid,
	/** Array length == 0. */
	IsNotValid
};


/**
 *
 */
UCLASS()
class ALGORITHMSYSTEM_API UMiscAlgorithm : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()


public: // wildcard

	/*
	 *Filter an array based on filter function of object.
	 *
	 *@param	Object		The owner of function
	 *@param	FilterBy	Filter function name, this custom function with 2 parameters,
							1 input (Type same as array member), 1 return named "ReturnValue"(bool)
	 *@param	TargetArray	 The array to filter from
	 *@return	An array containing only those members which meet the filterBy condition.
	*/
	UFUNCTION(BlueprintCallable, CustomThunk, meta = (DisplayName = "Filter Array", CompactNodeTitle = "Filter", ArrayParm = "TargetArray,FilteredArray", ArrayTypeDependentParams = "TargetArray,FilteredArray", AutoCreateRefTerm = "FilteredArray", DefaultToSelf = "Object", AdvancedDisplay = "Object"), Category = "Utilities|Array")
		static void Array_Filter(const UObject* Object, const FName FilterBy, const TArray<int32>& TargetArray, TArray<int32>& FilteredArray);
	static void GenericArray_Filter(UObject* Object, UFunction* FilterFunction, const UArrayProperty* ArrayProp, void* SrcArrayAddr, void* FilterArrayAddr);

	DECLARE_FUNCTION(execArray_Filter)
	{
		P_GET_OBJECT(UObject, OwnerObject);
		P_GET_PROPERTY(UNameProperty, FilterBy);

		//Find filter function
		UFunction* const FilterFunction = OwnerObject->FindFunction(FilterBy);
		// Fitler function must have two parameters(1 input / 1 output)
		if (!FilterFunction || (FilterFunction->NumParms != 2))
		{
			UE_LOG(LogTemp, Warning, TEXT("Tooltip -> Array_Filter -> Please check filter function %s "), *FilterBy.ToString());
			return;
		}

		// Get target array  address and ArrayProperty
		Stack.MostRecentProperty = nullptr;
		Stack.StepCompiledIn<UArrayProperty>(NULL);
		void* SrcArrayAddr = Stack.MostRecentPropertyAddress;
		UArrayProperty* SrcArrayProperty = Cast<UArrayProperty>(Stack.MostRecentProperty);
		if (!SrcArrayProperty)
		{
			Stack.bArrayContextFailed = true;
			return;
		}

		// Get filtered array address and arrayproperty
		Stack.MostRecentProperty = nullptr;
		Stack.StepCompiledIn<UArrayProperty>(NULL);
		void* FilterArrayAddr = Stack.MostRecentPropertyAddress;
		UArrayProperty* FilterArrayProperty = Cast<UArrayProperty>(Stack.MostRecentProperty);
		if (!FilterArrayProperty)
		{
			Stack.bArrayContextFailed = true;
			return;
		}

		P_FINISH;

		P_NATIVE_BEGIN;
		// ScrArrayProperty is equal to FilterArrayProperty
		GenericArray_Filter(OwnerObject, FilterFunction, SrcArrayProperty, SrcArrayAddr, FilterArrayAddr);
		P_NATIVE_END;
	}


	/*
	 *Determines if an aray is valid(length > 0) ?
	 *
	 *@param	TargetArray		The array to get the length > 0
	 *@return	True if length of array > 0, false otherwise.
	*/
	UFUNCTION(BlueprintPure, CustomThunk, meta = (DisplayName = "Is Valid Array", CompactNodeTitle = "VALID", ArrayParm = "TargetArray", Keywords = "num, size,valid", BlueprintThreadSafe), Category = "Utilities|Array")
		static bool Array_Valid(const TArray<int32>& TargetArray);
	static bool GenericArray_Valid(void* ArrayAddr, UArrayProperty* ArrayProperty);

	DECLARE_FUNCTION(execArray_Valid)
	{
		Stack.MostRecentProperty = nullptr;
		Stack.StepCompiledIn<UArrayProperty>(NULL);
		void* ArrayAddr = Stack.MostRecentPropertyAddress;
		UArrayProperty* ArrayProperty = Cast<UArrayProperty>(Stack.MostRecentProperty);
		if (!ArrayProperty)
		{
			Stack.bArrayContextFailed = true;
			return;
		}

		P_FINISH;

		P_NATIVE_BEGIN;
		*(bool*)RESULT_PARAM = GenericArray_Valid(ArrayAddr, ArrayProperty);
		P_NATIVE_END;
	}

	/*
	 *Determines if an aray is valid(length > 0) ?
	 *
	 *@param	TargetArray		The array to get the length > 0
	*/
	UFUNCTION(BlueprintCallable, CustomThunk, meta = (DisplayName = "Is Valid ?", ArrayParm = "TargetArray", Keywords = "num, size,valid", ExpandEnumAsExecs = "EvaluateArrayPIN", BlueprintThreadSafe), Category = "Utilities|Array")
		static void Array_Validv2(const TArray<int32>& TargetArray, EEvaluateArray& EvaluateArrayPIN);
	static void GenericArray_Validv2(void* ArrayAddr, UArrayProperty* ArrayProperty, EEvaluateArray& EvaluateArrayPIN);

	DECLARE_FUNCTION(execArray_Validv2)
	{
		Stack.MostRecentProperty = nullptr;
		Stack.StepCompiledIn<UArrayProperty>(NULL);
		void* ArrayAddr = Stack.MostRecentPropertyAddress;
		UArrayProperty* ArrayProperty = Cast<UArrayProperty>(Stack.MostRecentProperty);
		if (!ArrayProperty)
		{
			Stack.bArrayContextFailed = true;
			return;
		}

		P_GET_ENUM_REF(EEvaluateArray, EvaluateArrayPIN);

		P_FINISH;

		P_NATIVE_BEGIN;
		GenericArray_Validv2(ArrayAddr, ArrayProperty, EvaluateArrayPIN);
		P_NATIVE_END;
	}


	/**
		* Save any type of struct object to JSON format string, no struct type restriction
		*
		* @param	StructReference		The UStruct instance to read from
		* @return	JSON String		Json Object string to be filled in with data from the ustruct
	*/
	UFUNCTION(BlueprintPure, CustomThunk, meta = (CustomStructureParam = "StructReference", DisplayName = "Struct to JSON String"), Category = "File|Json")
		static void UStructToJsonObjectString(const int32& StructReference, FString& JSONString);
	//static void Generic_UStructToJsonObjectString(UStructProperty* StructProperty, void* StructPtr, FString& JSONString);

	/// Custom execFunciton thunk for function UStructToJsonObjectString.
	DECLARE_FUNCTION(execUStructToJsonObjectString)
	{
		//Get input wildcard single variable
		Stack.Step(Stack.Object, NULL);
		UStructProperty* StructProperty = ExactCast<UStructProperty>(Stack.MostRecentProperty);
		void* StructPtr = Stack.MostRecentPropertyAddress;

		//Get JsonString reference
		P_GET_PROPERTY_REF(UStrProperty, JSONString);

		P_FINISH;

		P_NATIVE_BEGIN;
		FJsonObjectConverter::UStructToJsonObjectString(StructProperty->Struct, StructPtr, JSONString, 0, 0);
		//Generic_UStructToJsonObjectString(StructProperty, StructPtr, JSONString);
		P_NATIVE_END;
	}

};
