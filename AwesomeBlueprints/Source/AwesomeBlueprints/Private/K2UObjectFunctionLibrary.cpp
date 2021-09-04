// Fill out your copyright notice in the Description page of Project Settings.


#include "K2UObjectFunctionLibrary.h"

//General Log
DEFINE_LOG_CATEGORY(LogUK2UObjectFunctionLibrary);


void UK2UObjectFunctionLibrary::SetSingelPropertyByName(UObject* Object, FName PropertyName, const int32& Value)
{
	// We should never hit these!  They're stubs to avoid NoExport on the class.
	check(0);
}

void UK2UObjectFunctionLibrary::Generic_SetSingelPropertyByName(UObject* OwnerObject, FName PropertyName, FProperty* ValueProperty, void* ValueAddr)
{
	if (OwnerObject != NULL)
	{
		FProperty* FoundProp = FindFProperty<FProperty>(OwnerObject->GetClass(), PropertyName);

		if (FoundProp && (FoundProp->SameType(ValueProperty)))
		{
			void* DestValueAddr = FoundProp->ContainerPtrToValuePtr<void>(OwnerObject);
			FoundProp->CopySingleValue(DestValueAddr, ValueAddr);
			return;
		}
	}
	UE_LOG(LogUK2UObjectFunctionLibrary, Warning, TEXT("Time:%s, Function:%s, Falid to set %s property value of %s object.")
		, *FString(__TIME__)
		, *FString(__FUNCTION__)
		, *PropertyName.ToString()
		, OwnerObject ? *OwnerObject->GetName() : TEXT("NULL")
	);

}

bool UK2UObjectFunctionLibrary::GetSingelPropertyByName(UObject* Object, FName PropertyName, int32& Value)
{
	// We should never hit these!  They're stubs to avoid NoExport on the class.  Call the Generic* equivalent instead
	check(0);
	return false;
}

bool UK2UObjectFunctionLibrary::GetArrayPropertyByName(UObject* Object, FName PropertyName, TArray<int32>& Value)
{
	// We should never hit these!  They're stubs to avoid NoExport on the class.  Call the Generic* equivalent instead
	check(0);
	return false;
}

bool UK2UObjectFunctionLibrary::GetMapPropertyByName(UObject* Object, FName PropertyName, TMap<int32, int32>& Value)
{
	// We should never hit these!  They're stubs to avoid NoExport on the class.  Call the Generic* equivalent instead
	check(0);
	return false;
}

bool UK2UObjectFunctionLibrary::GetSetPropertyByName(UObject* Object, FName PropertyName, TSet<int32>& Value)
{
	// We should never hit these!  They're stubs to avoid NoExport on the class.  Call the Generic* equivalent instead
	check(0);
	return false;
}


bool UK2UObjectFunctionLibrary::Generic_GetPropertyByName(UObject* OwnerObject, FName PropertyName, FProperty* OutValueProperty, void* OutValueAddr)
{
	if (OwnerObject != NULL)
	{
		FProperty* FoundProp = FindFProperty<FProperty>(OwnerObject->GetClass(), PropertyName);

		if ((FoundProp != NULL) && (FoundProp->SameType(OutValueProperty)))
		{
			void* SrcValueAddr = FoundProp->ContainerPtrToValuePtr<void>(OwnerObject);

			if (FArrayProperty* ArrayProperty = CastField<FArrayProperty>(FoundProp))
			{
				ArrayProperty->CopyValuesInternal(OutValueAddr, SrcValueAddr, 1);
			}
			else if (FSetProperty* SetProperty = CastField<FSetProperty>(FoundProp))
			{
				ArrayProperty->CopyValuesInternal(OutValueAddr, SrcValueAddr, 1);
			}
			else if (FMapProperty* MapProperty = CastField<FMapProperty>(FoundProp))
			{
				MapProperty->CopyValuesInternal(OutValueAddr, SrcValueAddr, 1);
			}
			else
			{
				FoundProp->CopySingleValue(OutValueAddr, SrcValueAddr);
			}
			return true;
		}
	}
	UE_LOG(LogUK2UObjectFunctionLibrary, Warning, TEXT("Time:%s, Function:%s, Falid to get %s property value from %s object.")
		, *FString(__TIME__)
		, *FString(__FUNCTION__)
		, *PropertyName.ToString()
		, OwnerObject ? *OwnerObject->GetName() : TEXT("NULL")
	);
	return false;
}
