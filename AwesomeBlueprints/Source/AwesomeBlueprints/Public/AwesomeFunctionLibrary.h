// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Engine/DataTable.h"
#include "AwesomeFunctionLibrary.generated.h"


/**
 *
 */
UCLASS()
class AWESOMEBLUEPRINTS_API UAwesomeFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()


public:
	/** Get a Row from a DataTable given a RowName */
	UFUNCTION(BlueprintCallable, CustomThunk, meta = (CustomStructureParam = "RowData", BlueprintInternalUseOnly = "true"), Category = "DataTable")
		static void SetDataTableRowFromName(UDataTable* Table, FName RowName, const FTableRowBase& RowData);
	static bool Generic_SetDataTableRowFromName(const UDataTable* Table, FName RowName, void* OutRowPtr);

	/** Based on UDataTableFunctionLibrary::GetDataTableRow */
	DECLARE_FUNCTION(execSetDataTableRowFromName)
	{
		P_GET_OBJECT(UDataTable, DataTable);
		P_GET_PROPERTY(FNameProperty, RowName);

		//Get input wildcard single variable
		Stack.Step(Stack.Object, NULL);
		FStructProperty* StructProperty = CastField<FStructProperty>(Stack.MostRecentProperty);
		void* StructPtr = Stack.MostRecentPropertyAddress;

		//P_GET_UBOOL_REF(bSuccessful);
		P_FINISH;
		P_NATIVE_BEGIN;
		//Generic_SetDataTableRowFromName(DataTable, RowName, StructProperty, StructPtr, bSuccessful);
		P_NATIVE_END;
	}


	/** Generic sort array by property using quick sort algorithm.
	*
	*	@param	TargetArray	Target array to sort
	*	@param	PropertyName	Name is the variable to sort by for struct or object array. Otherwise, the parameter is ignored.
	*	@param	bAscending	If true, sort by ascending order.
	*/
	UFUNCTION(BlueprintCallable, CustomThunk, meta = (ArrayParm = "TargetArray", BlueprintInternalUseOnly = "true"), Category = "Utilities|Array")
		static void Array_SortV2(const TArray<int32>& TargetArray, FName PropertyName, bool bAscending);

	// generic quick sort array by property
	static void GenericArray_SortV2(void* TargetArray, FArrayProperty* ArrayProp, FName PropertyName, bool bAscending);

	// sort array by property
	DECLARE_FUNCTION(execArray_SortV2)
	{
		Stack.MostRecentProperty = nullptr;
		Stack.StepCompiledIn<FArrayProperty>(NULL);
		void* ArrayAddr = Stack.MostRecentPropertyAddress;
		FArrayProperty* ArrayProperty = CastField<FArrayProperty>(Stack.MostRecentProperty);
		if (!ArrayProperty)
		{
			Stack.bArrayContextFailed = true;
			return;
		}
		P_GET_PROPERTY(FNameProperty, PropertyName);
		P_GET_UBOOL(bAscending);
		P_FINISH;

		P_NATIVE_BEGIN;
		UAwesomeFunctionLibrary::GenericArray_SortV2(ArrayAddr, ArrayProperty, PropertyName, bAscending);
		P_NATIVE_END;
	}

private:
	// Low  --> Starting index,  High  --> Ending index
	static void QuickSort_RecursiveByProperty(FScriptArrayHelper& ArrayHelper, FProperty* InnerProp, FProperty* SortProp, int32 Low, int32 High, bool bAscending);
	// swapping items in place and partitioning the section of an array
	static int32 QuickSort_PartitionByProperty(FScriptArrayHelper& ArrayHelper, FProperty* InnerProp, FProperty* SortProp, int32 Low, int32 High, bool bAscending);
	// generic compare two element of array by property
	static bool GenericComparePropertyValue(FScriptArrayHelper& ArrayHelper, FProperty* InnerProp, FProperty* SortProp, int32 j, int32 High, bool bAscending);
	// internal function of GenericArray_Sort
	static bool GenericArray_SortCompare(const FProperty* LeftProperty, void* LeftValuePtr, const FProperty* RightProperty, void* RightValuePtr);
};
