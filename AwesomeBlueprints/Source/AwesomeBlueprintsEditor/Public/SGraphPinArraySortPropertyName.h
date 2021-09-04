// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/DeclarativeSyntaxSupport.h"
#include "SGraphPinNameList.h"

/** Class Forward Declarations */
class UEdGraphPin;

class AWESOMEBLUEPRINTSEDITOR_API SGraphPinArraySortPropertyName : public SGraphPinNameList
{
public:
	SLATE_BEGIN_ARGS(SGraphPinArraySortPropertyName) {}
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs, UEdGraphPin* InGraphPinObj, const TArray<FString>& InNameList);

	SGraphPinArraySortPropertyName();
	virtual ~SGraphPinArraySortPropertyName();

protected:

	void RefreshNameList(const TArray<FString>& InNameList);

};
