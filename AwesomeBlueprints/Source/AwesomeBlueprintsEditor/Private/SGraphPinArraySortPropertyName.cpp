// Copyright Epic Games, Inc. All Rights Reserved.

#include "SGraphPinArraySortPropertyName.h"

void SGraphPinArraySortPropertyName::Construct(const FArguments& InArgs, UEdGraphPin* InGraphPinObj, const TArray<FString>& InNameList)
{
	RefreshNameList(InNameList);
	SGraphPinNameList::Construct(SGraphPinNameList::FArguments(), InGraphPinObj, NameList);
}

SGraphPinArraySortPropertyName::SGraphPinArraySortPropertyName()
{}

SGraphPinArraySortPropertyName::~SGraphPinArraySortPropertyName()
{}


void SGraphPinArraySortPropertyName::RefreshNameList(const TArray<FString>& InNameList)
{
	NameList.Empty();
	for (FString PropName : InNameList)
	{
		TSharedPtr<FName> NamePropertyItem = MakeShareable(new FName(PropName));
		NameList.Add(NamePropertyItem);
	}
}
