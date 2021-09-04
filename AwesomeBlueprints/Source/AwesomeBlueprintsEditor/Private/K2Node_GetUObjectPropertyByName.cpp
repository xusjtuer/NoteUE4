// Fill out your copyright notice in the Description page of Project Settings.

#include "K2Node_GetUObjectPropertyByName.h"
#include "EdGraphSchema_K2.h"
#include "K2Node_CallFunction.h"
#include "Kismet2/BlueprintEditorUtils.h"
#include "KismetCompiler.h"
#include "BlueprintNodeSpawner.h"
#include "EditorCategoryUtils.h"
#include "BlueprintActionDatabaseRegistrar.h"
#include "K2UObjectFunctionLibrary.h"
#include "K2Node_IfThenElse.h"


#define LOCTEXT_NAMESPACE "K2Node_GetUObjectPropertyByName"

namespace GetPropertyHelper
{
	const FName ObjectPinName = "Object";
	const FName PropertyNamePinName = "PropertyName";
	const FName GetValueFailed = "Failed";
	const FName OutValuePinName = "Value";
}

UK2Node_GetUObjectPropertyByName::UK2Node_GetUObjectPropertyByName(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	NodeTooltip = LOCTEXT("NodeTooltip", "Get UObject property value by name");
}


void UK2Node_GetUObjectPropertyByName::AllocateDefaultPins()
{
	const UEdGraphSchema_K2* K2Schema = GetDefault<UEdGraphSchema_K2>();

	// Add execution pins
	CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Exec, UEdGraphSchema_K2::PN_Execute);

	CreatePin(EGPD_Output, UEdGraphSchema_K2::PC_Exec, UEdGraphSchema_K2::PN_Then);
	CreatePin(EGPD_Output, UEdGraphSchema_K2::PC_Exec, GetPropertyHelper::GetValueFailed);

	// Add Target Object pin
	UEdGraphPin* ObjectPin = CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Object, UObject::StaticClass(), GetPropertyHelper::ObjectPinName);
	SetPinToolTip(*ObjectPin, LOCTEXT("ObjectPinDescription", "Object that owns this property"));

	// PropertyName Pin
	UEdGraphPin* PropertyNamePin = CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Name, GetPropertyHelper::PropertyNamePinName);
	SetPinToolTip(*PropertyNamePin, LOCTEXT("PropertyNamePinDescription", "Target property name to get"));

	// OutValue pin
	UEdGraphPin* ValuePin = CreatePin(EGPD_Output, UEdGraphSchema_K2::PC_Wildcard, GetPropertyHelper::OutValuePinName);
	SetPinToolTip(*ValuePin, LOCTEXT("ValuePinDescription", "The target value, if found"));

	Super::AllocateDefaultPins();
}

void UK2Node_GetUObjectPropertyByName::SetPinToolTip(UEdGraphPin& MutatablePin, const FText& PinDescription) const
{
	MutatablePin.PinToolTip = UEdGraphSchema_K2::TypeToText(MutatablePin.PinType).ToString();

	UEdGraphSchema_K2 const* const K2Schema = Cast<const UEdGraphSchema_K2>(GetSchema());
	if (K2Schema != nullptr)
	{
		MutatablePin.PinToolTip += TEXT(" ");
		MutatablePin.PinToolTip += K2Schema->GetPinDisplayName(&MutatablePin).ToString();
	}
	MutatablePin.PinToolTip += FString(TEXT("\n")) + PinDescription.ToString();
}

void UK2Node_GetUObjectPropertyByName::PropagateValuePinType()
{
	UEdGraphPin* ValuePin = GetValuePin();
	if (ValuePin->LinkedTo.Num() > 0 && ValuePin->LinkedTo[0]->PinType.PinCategory != UEdGraphSchema_K2::PC_Wildcard)
	{
		UEdGraphPin* ValueLinkedPin = ValuePin->LinkedTo[0];
		ValuePin->PinType = ValueLinkedPin->PinType;
		//ValuePin->PinType.bIsReference = true;
	}
	else
	{
		ValuePin->ResetDefaultValue();
		ValuePin->PinType.ContainerType = EPinContainerType::None;
		ValuePin->PinType.PinCategory = UEdGraphSchema_K2::PC_Wildcard;
	}
}


void UK2Node_GetUObjectPropertyByName::GetMenuActions(FBlueprintActionDatabaseRegistrar& ActionRegistrar) const
{
	// actions get registered under specific object-keys; the idea is that 
	// actions might have to be updated (or deleted) if their object-key is  
	// mutated (or removed)... here we use the node's class (so if the node 
	// type disappears, then the action should go with it)
	UClass* ActionKey = GetClass();
	// to keep from needlessly instantiating a UBlueprintNodeSpawner, first   
	// check to make sure that the registrar is looking for actions of this type
	// (could be regenerating actions for a specific asset, and therefore the 
	// registrar would only accept actions corresponding to that asset)
	if (ActionRegistrar.IsOpenForRegistration(ActionKey))
	{
		UBlueprintNodeSpawner* NodeSpawner = UBlueprintNodeSpawner::Create(GetClass());
		check(NodeSpawner != nullptr);

		ActionRegistrar.AddBlueprintAction(ActionKey, NodeSpawner);
	}
}

FText UK2Node_GetUObjectPropertyByName::GetMenuCategory() const
{
	return FEditorCategoryUtils::GetCommonCategory(FCommonEditorCategory::Utilities);
}


void UK2Node_GetUObjectPropertyByName::PinDefaultValueChanged(UEdGraphPin* ChangedPin)
{
	if (ChangedPin && ChangedPin->PinName == GetPropertyHelper::OutValuePinName)
	{
		PropagateValuePinType();
	}
}

FText UK2Node_GetUObjectPropertyByName::GetTooltipText() const
{
	return NodeTooltip;
}

FText UK2Node_GetUObjectPropertyByName::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
	return NSLOCTEXT("K2Node", "GetUObjectPropertyValueByName", "Get Property Value By Name");
}

void UK2Node_GetUObjectPropertyByName::ExpandNode(class FKismetCompilerContext& CompilerContext, UEdGraph* SourceGraph)
{
	Super::ExpandNode(CompilerContext, SourceGraph);

	UEdGraphPin* SrcValuePin = GetValuePin();
	if ((SrcValuePin == nullptr) || (SrcValuePin->LinkedTo.Num() == 0))
	{
		CompilerContext.MessageLog.Error(*LOCTEXT("GetUObjectPropertyByName_Error", "GetUObjectPropertyByName must have a Value specified.").ToString(), this);
		// we break exec links so this is the only error we get
		BreakAllNodeLinks();
		return;
	}
	// Get Target call Function
	UK2Node_CallFunction* DstCallFunction = GetK2NodeDstCallFunction(CompilerContext, SourceGraph);
	if (!DstCallFunction)
	{
		CompilerContext.MessageLog.Error(*LOCTEXT("GetUObjectPropertyByName_Error", "DstCallFunction is NULL.").ToString(), this);
		// we break exec links so this is the only error we get
		BreakAllNodeLinks();
		return;
	}

	// ExecPin
	CompilerContext.MovePinLinksToIntermediate(*GetExecPin(), *(DstCallFunction->GetExecPin()));

	// DstCallFunction Object Pin
	UEdGraphPin* DstObjectPin = DstCallFunction->FindPinChecked(GetPropertyHelper::ObjectPinName);
	CompilerContext.MovePinLinksToIntermediate(*GetObjectPin(), *DstObjectPin);

	// DstCallFunction PropertyName Pin
	UEdGraphPin* DstPropertyNamePin = DstCallFunction->FindPinChecked(GetPropertyHelper::PropertyNamePinName);
	CompilerContext.MovePinLinksToIntermediate(*GetPropertyNamePin(), *DstPropertyNamePin);

	// DstCallFunction Value pin
	UEdGraphPin* DstValuePin = DstCallFunction->FindPinChecked(GetPropertyHelper::OutValuePinName);
	DstValuePin->PinType = SrcValuePin->PinType;
	DstValuePin->PinType.PinSubCategoryObject = SrcValuePin->PinType.PinSubCategoryObject;
	CompilerContext.MovePinLinksToIntermediate(*SrcValuePin, *DstValuePin);

	// DstCallFunction Then pin
	UEdGraphPin* DstFunctionThenPin = DstCallFunction->FindPinChecked(UEdGraphSchema_K2::PN_Then);
	// DstCallFunction ReturnValue pin
	UEdGraphPin* DstFunctionReturnPin = DstCallFunction->FindPinChecked(UEdGraphSchema_K2::PN_ReturnValue);

	//BRANCH NODE
	UK2Node_IfThenElse* BranchNode = CompilerContext.SpawnIntermediateNode<UK2Node_IfThenElse>(this, SourceGraph);
	BranchNode->AllocateDefaultPins();
	// Hook up inputs to branch
	DstFunctionThenPin->MakeLinkTo(BranchNode->GetExecPin());
	DstFunctionReturnPin->MakeLinkTo(BranchNode->GetConditionPin());

	// Hook up outputs
	CompilerContext.MovePinLinksToIntermediate(*GetThenPin(), *(BranchNode->GetThenPin()));
	CompilerContext.MovePinLinksToIntermediate(*GetFailedPin(), *(BranchNode->GetElsePin()));

	BreakAllNodeLinks();
}

UK2Node_CallFunction* UK2Node_GetUObjectPropertyByName::GetK2NodeDstCallFunction(class FKismetCompilerContext& CompilerContext, UEdGraph* SourceGraph)
{
	UK2Node_CallFunction* DstCallFunction = nullptr;
	EPinContainerType ContainerType = GetValuePin()->PinType.ContainerType;

	FName FunctionName;
	if (ContainerType == EPinContainerType::None)
	{
		FunctionName = GET_FUNCTION_NAME_CHECKED(UK2UObjectFunctionLibrary, GetSingelPropertyByName);
	}
	else if (ContainerType == EPinContainerType::Array)
	{
		FunctionName = GET_FUNCTION_NAME_CHECKED(UK2UObjectFunctionLibrary, GetArrayPropertyByName);
	}
	else if (ContainerType == EPinContainerType::Map)
	{
		FunctionName = GET_FUNCTION_NAME_CHECKED(UK2UObjectFunctionLibrary, GetMapPropertyByName);
	}
	else if (ContainerType == EPinContainerType::Set)
	{
		FunctionName = GET_FUNCTION_NAME_CHECKED(UK2UObjectFunctionLibrary, GetSetPropertyByName);
	}
	DstCallFunction = CompilerContext.SpawnIntermediateNode<UK2Node_CallFunction>(this, SourceGraph);
	DstCallFunction->FunctionReference.SetExternalMember(FunctionName, UK2UObjectFunctionLibrary::StaticClass());
	DstCallFunction->AllocateDefaultPins();

	return DstCallFunction;
}

FSlateIcon UK2Node_GetUObjectPropertyByName::GetIconAndTint(FLinearColor& OutColor) const
{
	OutColor = GetNodeTitleColor();
	static FSlateIcon Icon("EditorStyle", "Kismet.AllClasses.FunctionIcon");
	return Icon;
}

void UK2Node_GetUObjectPropertyByName::PostReconstructNode()
{
	Super::PostReconstructNode();

	PropagateValuePinType();
}


void UK2Node_GetUObjectPropertyByName::NotifyPinConnectionListChanged(UEdGraphPin* Pin)
{
	Super::NotifyPinConnectionListChanged(Pin);

	if (Pin == GetValuePin())
	{
		PropagateValuePinType();
	}
}

#undef LOCTEXT_NAMESPACE
