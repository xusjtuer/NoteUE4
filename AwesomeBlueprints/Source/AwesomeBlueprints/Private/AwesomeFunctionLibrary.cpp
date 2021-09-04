// Fill out your copyright notice in the Description page of Project Settings.


#include "AwesomeFunctionLibrary.h"
#include "Kismet/KismetArrayLibrary.h"


namespace AwesomeFunctionLibraryHelper
{
	template <class T> T* FindFProperty(const UStruct* Owner, FName FieldName)
	{
		// We know that a "none" field won't exist in this Struct
		if (FieldName.IsNone())
		{
			return nullptr;
		}

		// Search by comparing FNames (INTs), not strings
		for (TFieldIterator<T>It(Owner); It; ++It)
		{
			if ((It->GetFName() == FieldName) || (It->GetAuthoredName() == FieldName.ToString()))
			{
				return *It;
			}
		}
		// If we didn't find it, return no field
		return nullptr;
	}
}


void UAwesomeFunctionLibrary::SetDataTableRowFromName(UDataTable* Table, FName RowName, const FTableRowBase& RowData)
{
	// We should never hit this!  stubs to avoid NoExport on the class.
	check(0);
}

bool UAwesomeFunctionLibrary::Generic_SetDataTableRowFromName(const UDataTable* Table, FName RowName, void* OutRowPtr)
{
	bool bFoundRow = false;

	//if (OutRowPtr && Table)
	//{
	//	void* RowPtr = Table->FindRowUnchecked(RowName);

	//	if (RowPtr != nullptr)
	//	{
	//		const UScriptStruct* StructType = Table->GetRowStruct();

	//		if (StructType != nullptr)
	//		{
	//			StructType->CopyScriptStruct(OutRowPtr, RowPtr);
	//			bFoundRow = true;
	//		}
	//	}
	//}

	return bFoundRow;
}

void UAwesomeFunctionLibrary::Array_SortV2(const TArray<int32>& TargetArray, FName PropertyName, bool bAscending)
{
	// We should never hit this!  stubs to avoid NoExport on the class.
	check(0);
}

void UAwesomeFunctionLibrary::GenericArray_SortV2(void* TargetArray, FArrayProperty* ArrayProp, FName PropertyName, bool bAscending)
{
	if (!TargetArray)
	{
		return;
	}
	// Optimal
	UKismetArrayLibrary::GenericArray_Shuffle(TargetArray, ArrayProp);
	FScriptArrayHelper ArrayHelper(ArrayProp, TargetArray);

	FProperty* SortProp = nullptr;
	if (ArrayHelper.Num() < 2)
	{
		return;
	}
	else if (const FObjectProperty* ObjectProp = CastField<const FObjectProperty>(ArrayProp->Inner))
	{
		SortProp = FindFProperty<FProperty>(ObjectProp->PropertyClass, PropertyName);
	}
	else if (const FStructProperty* StructProp = CastField<const FStructProperty>(ArrayProp->Inner))
	{
		SortProp = AwesomeFunctionLibraryHelper::FindFProperty<FProperty>(StructProp->Struct, PropertyName);
	}
	else
	{
		SortProp = ArrayProp->Inner;
	}

	if (SortProp)
	{
		QuickSort_RecursiveByProperty(ArrayHelper, ArrayProp->Inner, SortProp, 0, ArrayHelper.Num() - 1, bAscending);
	}
}

void UAwesomeFunctionLibrary::QuickSort_RecursiveByProperty(FScriptArrayHelper& ArrayHelper, FProperty* InnerProp, FProperty* SortProp, int32 Low, int32 High, bool bAscending)
{
	if (Low < High)
	{
		int32 Pivot = QuickSort_PartitionByProperty(ArrayHelper, InnerProp, SortProp, Low, High, bAscending);

		QuickSort_RecursiveByProperty(ArrayHelper, InnerProp, SortProp, Low, Pivot - 1, bAscending);
		QuickSort_RecursiveByProperty(ArrayHelper, InnerProp, SortProp, Pivot + 1, High, bAscending);
	}
}


int32 UAwesomeFunctionLibrary::QuickSort_PartitionByProperty(FScriptArrayHelper& ArrayHelper, FProperty* InnerProp, FProperty* SortProp, int32 Low, int32 High, bool bAscending)
{
	int32 i = Low - 1;

	for (int32 j = Low; j < High; j++)
	{
		if (GenericComparePropertyValue(ArrayHelper, InnerProp, SortProp, j, High, bAscending))
		{
			i++;
			ArrayHelper.SwapValues(i, j);
		}
	}
	ArrayHelper.SwapValues(i + 1, High);
	return i + 1;
}

bool UAwesomeFunctionLibrary::GenericComparePropertyValue(FScriptArrayHelper& ArrayHelper, FProperty* InnerProp, FProperty* SortProp, int32 j, int32 High, bool bAscending)
{
	bool bResult = false;
	void* LeftValueAddr = nullptr;
	void* RightValueAddr = nullptr;
	if (const FObjectProperty* ObjectProp = CastField<const FObjectProperty>(InnerProp))
	{
		UObject* LeftObject = ObjectProp->GetObjectPropertyValue(ArrayHelper.GetRawPtr(j));
		UObject* RightObject = ObjectProp->GetObjectPropertyValue(ArrayHelper.GetRawPtr(High));

		LeftValueAddr = SortProp->ContainerPtrToValuePtr<void>(LeftObject);
		RightValueAddr = SortProp->ContainerPtrToValuePtr<void>(RightObject);
	}
	else
	{
		LeftValueAddr = SortProp->ContainerPtrToValuePtr<void>(ArrayHelper.GetRawPtr(j));
		RightValueAddr = SortProp->ContainerPtrToValuePtr<void>(ArrayHelper.GetRawPtr(High));
	}

	if (const FNumericProperty* NumericProp = CastField<const FNumericProperty>(SortProp))
	{
		if (NumericProp->IsFloatingPoint())
		{
			bResult = NumericProp->GetFloatingPointPropertyValue(LeftValueAddr) < NumericProp->GetFloatingPointPropertyValue(RightValueAddr);
		}
		else if (NumericProp->IsInteger())
		{
			bResult = NumericProp->GetSignedIntPropertyValue(LeftValueAddr) < NumericProp->GetSignedIntPropertyValue(RightValueAddr);
		}
	}
	else if (const FBoolProperty* BoolProp = CastField<const FBoolProperty>(SortProp))
	{
		bResult = !BoolProp->GetPropertyValue(LeftValueAddr) && BoolProp->GetPropertyValue(RightValueAddr);
	}
	else if (const FNameProperty* NameProp = CastField<const FNameProperty>(SortProp))
	{
		bResult = NameProp->GetPropertyValue(LeftValueAddr).ToString() < NameProp->GetPropertyValue(RightValueAddr).ToString();
	}
	else if (const FStrProperty* StringProp = CastField<const FStrProperty>(SortProp))
	{
		bResult = (StringProp->GetPropertyValue(LeftValueAddr) < StringProp->GetPropertyValue(RightValueAddr));
	}
	else if (const FTextProperty* TextProp = CastField<const FTextProperty>(SortProp))
	{
		bResult = (TextProp->GetPropertyValue(LeftValueAddr).ToString() < TextProp->GetPropertyValue(RightValueAddr).ToString());
	}

	return bResult == bAscending;
}

