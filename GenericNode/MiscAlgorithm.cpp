// Fill out your copyright notice in the Description page of Project Settings.

#include "MiscAlgorithm.h"
#include "Kismet/KismetArrayLibrary.h"


/** Declare General Log Category, source file .cpp */
DEFINE_LOG_CATEGORY(LogMiscAlgorithm);


void UMiscAlgorithm::GenericArray_Filter(UObject* Object, UFunction* FilterFunction, const UArrayProperty* ArrayProp, void* SrcArrayAddr, void* FilterArrayAddr)
{
	//check input parameters
	if (!Object || !FilterFunction || !SrcArrayAddr)
	{
		return;
	}
	// filter function return property
	UBoolProperty* ReturnProp = Cast<UBoolProperty>(FilterFunction->GetReturnProperty());
	if (!ReturnProp)
	{
		/// The return Property of filter function must be bool and named "ReturnValue"
		UE_LOG(LogTemp, Warning, TEXT("Tooltip -> GenericArray_Filter -> Pleas check return value of filter function.(Type:bool, Name:ReturnValue)"));
		return;
	}

	// Get function parameter list
	TArray<UProperty*> ParamterList;
	for (TFieldIterator<UProperty> It(FilterFunction); It; ++It)
	{
		UProperty* FuncParameter = *It;
		/// Get filter function parameters
		ParamterList.Emplace(FuncParameter);
	}
	/// Make sure the first input parameters of filter function is same to array inner
	if (!ParamterList[0]->SameType(ArrayProp->Inner))
	{
		/// The  property of 1st input parameter of filter function must be same as array member
		UE_LOG(LogTemp, Warning, TEXT("Tooltip -> GenericArray_Filter -> Pleas check input parameter of filter function.(Type is same as array member)"));
		return;
	}

	FScriptArrayHelper ArrayHelper(ArrayProp, SrcArrayAddr);
	FScriptArrayHelper FilterArray(ArrayProp, FilterArrayAddr);

	UProperty* InnerProp = ArrayProp->Inner;
	const int32 PropertySize = InnerProp->ElementSize * InnerProp->ArrayDim;
	// filter function parameters address, 1 input parameter(array item) and 1 return parameter (bool)
	uint8* FilterParamsAddr = (uint8*)FMemory::Malloc(PropertySize + 1);
	for (int32 i = 0; i < ArrayHelper.Num(); i++)
	{
		FMemory::Memzero(FilterParamsAddr, PropertySize + 1);
		// get array member and assign value to filter function input parameter
		InnerProp->CopyCompleteValueFromScriptVM(FilterParamsAddr, ArrayHelper.GetRawPtr(i));
		//process filter function
		Object->ProcessEvent(FilterFunction, FilterParamsAddr);
		if (ReturnProp && ReturnProp->GetPropertyValue(FilterParamsAddr + PropertySize))
		{
			// add item to filter array
			UKismetArrayLibrary::GenericArray_Add(FilterArrayAddr, ArrayProp, ArrayHelper.GetRawPtr(i));
		}
	}
	// relesed memory
	FMemory::Free(FilterParamsAddr);
}

bool UMiscAlgorithm::GenericArray_Valid(void* ArrayAddr, UArrayProperty* ArrayProperty)
{
	return (UKismetArrayLibrary::GenericArray_Length(ArrayAddr, ArrayProperty) > 0);
}


void UMiscAlgorithm::GenericArray_Validv2(void* ArrayAddr, UArrayProperty* ArrayProperty, EEvaluateArray& EvaluateArrayPIN)
{
	// Determine whether the array length is greater than 0
	bool bValidArray = UKismetArrayLibrary::GenericArray_Length(ArrayAddr, ArrayProperty) > 0;

	if (bValidArray == true)
	{
		// Exec pin is IsValid pin
		EvaluateArrayPIN = EEvaluateArray::IsValid;
	}
	else
	{
		// Exec pin is IsNotValid pin
		EvaluateArrayPIN = EEvaluateArray::IsNotValid;
	}
}


//void UMiscAlgorithm::Generic_UStructToJsonObjectString(UStructProperty* StructProperty, void* StructPtr, FString& JSONString)
//{
//	FJsonObjectConverter::UStructToJsonObjectString(StructProperty->Struct, StructPtr, JSONString, 0, 0);
//}
