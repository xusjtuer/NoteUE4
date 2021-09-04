// Copyright Epic Games, Inc. All Rights Reserved.
#include "K2BlueprintGraphPanelPinFactory.h"
#include "Engine/DataTable.h"
#include "Widgets/DeclarativeSyntaxSupport.h"
#include "EdGraphSchema_K2.h"
#include "K2Node_CallFunction.h"
#include "SGraphPin.h"
//#include "SGraphPinNameList.h"
//#include "K2Node_SetDataTableRow.h"
#include "K2Node_ArraySort.h"
#include "SGraphPinDataTableRowName.h"
#include "SGraphPinArraySortPropertyName.h"



TSharedPtr<class SGraphPin> FK2BlueprintGraphPanelPinFactory::CreatePin(class UEdGraphPin* InPin) const
{
	if (InPin->PinType.PinCategory == UEdGraphSchema_K2::PC_Name)
	{
		UObject* Outer = InPin->GetOuter();

		/*if (Outer->IsA(UK2Node_SetDataTableRow::StaticClass()))
		//{
		//	const UK2Node_SetDataTableRow* SetDataTableRowNode = CastChecked<UK2Node_SetDataTableRow>(Outer);
		//	// Create drop down combo boxes for DataTable and CurveTable RowName pins
		//	const UEdGraphPin* DataTablePin = SetDataTableRowNode->GetDataTablePin();
		//	if (DataTablePin)
		//	{
		//		if (DataTablePin->DefaultObject != nullptr && DataTablePin->LinkedTo.Num() == 0)
		//		{
		//			if (auto DataTable = Cast<UDataTable>(DataTablePin->DefaultObject))
		//			{
		//				return SNew(SGraphPinDataTableRowName, InPin, DataTable);
		//			}
		//		}
		//	}
		//}
		//else */if (Outer->IsA(UK2Node_ArraySort::StaticClass()))
		{
			UK2Node_ArraySort* ArraySortNode = CastChecked<UK2Node_ArraySort>(Outer);
			UEdGraphPin* ArrayPin = ArraySortNode->GetTargetArrayPin();
			if (ArrayPin)
			{
				if (ArrayPin->PinType.PinCategory == UEdGraphSchema_K2::PC_Object || ArrayPin->PinType.PinCategory == UEdGraphSchema_K2::PC_Struct)
				{
					TArray<FString> PropertyNames = ArraySortNode->GetPropertyNames();
					return SNew(SGraphPinArraySortPropertyName, ArraySortNode->GetPropertyNamePin(), PropertyNames);
				}
				else
				{
					UEdGraphPin* PropertyNamePin = ArraySortNode->GetPropertyNamePin();
					GetDefault<UEdGraphSchema_K2>()->SetPinAutogeneratedDefaultValueBasedOnType(PropertyNamePin);
				}
			}
		}
	}
	return nullptr;
}

