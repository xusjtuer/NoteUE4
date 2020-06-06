// Fill out your copyright notice in the Description page of Project Settings.

#include "ParserPropertyLibrary.h"
#include "Kismet/KismetSystemLibrary.h"


//Declare General Log Category, source file .cpp
DEFINE_LOG_CATEGORY(LogParserPropertyLibrary);

void UParserPropertyLibrary::Generic_AccessPropertyByName(UObject* OwnerObject, FName PropertyName, void* SrcPropertyAddr, UProperty* SrcProperty, bool bSetter /*= true*/)
{
	if (OwnerObject != NULL)
	{
		UProperty* FoundProp = FindField<UProperty>(OwnerObject->GetClass(), PropertyName);

		if ((FoundProp != NULL) && (FoundProp->SameType(SrcProperty)))
		{
			void* Dest = FoundProp->ContainerPtrToValuePtr<void>(OwnerObject);
			if (bSetter == true)
			{
				FoundProp->CopySingleValue(Dest, SrcPropertyAddr);
			}
			else
			{
				FoundProp->CopySingleValue(SrcPropertyAddr, Dest);
			}
			return;
		}
	}
	UE_LOG(LogParserPropertyLibrary, Warning, TEXT("UParserPropertyLibrary::Generic_AccessPropertyByName: Failed to find %s variable from %s object"), *PropertyName.ToString(), *UKismetSystemLibrary::GetDisplayName(OwnerObject));
}

void UParserPropertyLibrary::Generic_AcessArrayByName(UObject* OwnerObject, FName ArrayPropertyName, void* SrcArrayAddr, const UArrayProperty* SrcArrayProperty, bool bSetter /*= true*/)
{
	if (OwnerObject != NULL)
	{
		UArrayProperty* ArrayProp = FindField<UArrayProperty>(OwnerObject->GetClass(), ArrayPropertyName);

		if (ArrayProp != NULL && (ArrayProp->SameType(SrcArrayProperty)))
		{
			void* Dest = ArrayProp->ContainerPtrToValuePtr<void>(OwnerObject);
			if (bSetter == true)
			{
				ArrayProp->CopyValuesInternal(Dest, SrcArrayAddr, 1);
			}
			else
			{
				ArrayProp->CopyValuesInternal(SrcArrayAddr, Dest, 1);
			}
			return;
		}
	}
	UE_LOG(LogParserPropertyLibrary, Warning, TEXT("UParserPropertyLibrary::Generic_AcessArrayByName: Failed to find %s array from %s object"), *ArrayPropertyName.ToString(), *UKismetSystemLibrary::GetDisplayName(OwnerObject));
}


void UParserPropertyLibrary::Generic_AcessSetByName(UObject* OwnerObject, FName SetPropertyName, void* SrcSetAddr, const USetProperty* SrcSetProperty, bool bSetter /*= true*/)
{
	if (OwnerObject)
	{
		USetProperty* SetProp = FindField<USetProperty>(OwnerObject->GetClass(), SetPropertyName);

		if (SetProp != NULL && SetProp->SameType(SrcSetProperty))
		{
			void* Dest = SetProp->ContainerPtrToValuePtr<void>(OwnerObject);

			if (bSetter == true)
			{
				SetProp->CopyValuesInternal(Dest, SrcSetAddr, 1);
			}
			else
			{
				SetProp->CopyValuesInternal(SrcSetAddr, Dest, 1);
			}
			return;
		}
	}
	UE_LOG(LogParserPropertyLibrary, Warning, TEXT("UParserPropertyLibrary::Generic_AcessSetByName: Failed to find %s set from %s object"), *SetPropertyName.ToString(), *UKismetSystemLibrary::GetDisplayName(OwnerObject));
}


void UParserPropertyLibrary::Generic_AcessMapByName(UObject* OwnerObject, FName MapPropertyName, void* SrcMapAddr, const UMapProperty* SrcMapProperty, bool bSetter /*= true*/)
{

	if (OwnerObject)
	{
		UMapProperty* MapProp = FindField<UMapProperty>(OwnerObject->GetClass(), MapPropertyName);

		if (MapProp != NULL && MapProp->SameType(SrcMapProperty))
		{
			void* Dest = MapProp->ContainerPtrToValuePtr<void>(OwnerObject);

			if (bSetter == true)
			{
				MapProp->CopyValuesInternal(Dest, SrcMapAddr, 1);
			}
			else
			{
				MapProp->CopyValuesInternal(SrcMapAddr, Dest, 1);
			}
			return;
		}
	}
	UE_LOG(LogParserPropertyLibrary, Warning, TEXT("UParserPropertyLibrary::Generic_AcessMapByName: Failed to find %s map from %s object"), *MapPropertyName.ToString(), *UKismetSystemLibrary::GetDisplayName(OwnerObject));
}
