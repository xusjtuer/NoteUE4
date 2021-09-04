// Fill out your copyright notice in the Description page of Project Settings.


#include "K2Node_SetUObjectPropertyByName.h"
#include "EdGraphSchema_K2.h"
#include "K2Node_CallFunction.h"
#include "Kismet2/BlueprintEditorUtils.h"
#include "KismetCompiler.h"
#include "BlueprintNodeSpawner.h"
#include "EditorCategoryUtils.h"
#include "BlueprintActionDatabaseRegistrar.h"
#include "K2UObjectFunctionLibrary.h"
#include "Kismet/KismetArrayLibrary.h"
#include "Kismet/BlueprintMapLibrary.h"
#include "Kismet/BlueprintSetLibrary.h"


#define LOCTEXT_NAMESPACE "K2Node_SetUObjectPropertyByName"

namespace SetPropertyHelper
{
	const FName ObjectPinName = "Object";
	const FName PropertyNamePinName = "PropertyName";
	const FName ValuePinName = "Value";
}

UK2Node_SetUObjectPropertyByName::UK2Node_SetUObjectPropertyByName(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	NodeTooltip = LOCTEXT("NodeTooltip", "Set UObject property value by name");
}


void UK2Node_SetUObjectPropertyByName::AllocateDefaultPins()
{
	const UEdGraphSchema_K2* K2Schema = GetDefault<UEdGraphSchema_K2>();

	// Add execution pins
	CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Exec, UEdGraphSchema_K2::PN_Execute);
	CreatePin(EGPD_Output, UEdGraphSchema_K2::PC_Exec, UEdGraphSchema_K2::PN_Then);

	// Add Target Object pin
	UEdGraphPin* ObjectPin = CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Object, UObject::StaticClass(), SetPropertyHelper::ObjectPinName);
	SetPinToolTip(*ObjectPin, LOCTEXT("ObjectPinDescription", "Object that owns this property"));

	// PropertyName Pin
	UEdGraphPin* PropertyNamePin = CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Name, SetPropertyHelper::PropertyNamePinName);
	SetPinToolTip(*PropertyNamePin, LOCTEXT("PropertyNamePinDescription", "Name is the variable to sort by for struct or object array. Otherwise, the parameter is ignored."));

	// Value pin
	UEdGraphPin* ValuePin = CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Wildcard, SetPropertyHelper::ValuePinName);
	SetPinToolTip(*ValuePin, LOCTEXT("ValuePinDescription", "The new value to set"));

	Super::AllocateDefaultPins();
}

void UK2Node_SetUObjectPropertyByName::SetPinToolTip(UEdGraphPin& MutatablePin, const FText& PinDescription) const
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

void UK2Node_SetUObjectPropertyByName::PropagateValuePinType()
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


void UK2Node_SetUObjectPropertyByName::GetMenuActions(FBlueprintActionDatabaseRegistrar& ActionRegistrar) const
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

FText UK2Node_SetUObjectPropertyByName::GetMenuCategory() const
{
	return FEditorCategoryUtils::GetCommonCategory(FCommonEditorCategory::Utilities);
}


void UK2Node_SetUObjectPropertyByName::PinDefaultValueChanged(UEdGraphPin* ChangedPin)
{
	if (ChangedPin && ChangedPin->PinName == SetPropertyHelper::ValuePinName)
	{
		PropagateValuePinType();
	}
}

FText UK2Node_SetUObjectPropertyByName::GetTooltipText() const
{
	return NodeTooltip;
}

FText UK2Node_SetUObjectPropertyByName::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
	return NSLOCTEXT("K2Node", "SetUObjectPropertyValueByName", "Set Property Value By Name");
}

void UK2Node_SetUObjectPropertyByName::ExpandNode(class FKismetCompilerContext& CompilerContext, UEdGraph* SourceGraph)
{
	Super::ExpandNode(CompilerContext, SourceGraph);

	UEdGraphPin* SrcValuePin = GetValuePin();
	if ((SrcValuePin == nullptr) || (SrcValuePin->LinkedTo.Num() == 0))
	{
		CompilerContext.MessageLog.Error(*LOCTEXT("SetUObjectPropertyByName_Error", "SetUObjectPropertyByName must have a Value specified.").ToString(), this);
		// we break exec links so this is the only error we get
		BreakAllNodeLinks();
		return;
	}
	// Get Target call Function
	UK2Node_CallFunction* DstCallFunction = GetK2NodeDstCallFunction(CompilerContext, SourceGraph);

	if (!DstCallFunction)
	{
		CompilerContext.MessageLog.Error(*LOCTEXT("SetUObjectPropertyByName_Error", "DstCallFunction is NULL.").ToString(), this);
		// we break exec links so this is the only error we get
		BreakAllNodeLinks();
		return;
	}

	// ExecPin
	CompilerContext.MovePinLinksToIntermediate(*GetExecPin(), *(DstCallFunction->GetExecPin()));

	// Object Pin
	UEdGraphPin* DstObjectPin = DstCallFunction->FindPinChecked(SetPropertyHelper::ObjectPinName);
	CompilerContext.MovePinLinksToIntermediate(*GetObjectPin(), *DstObjectPin);

	// PropertyName pin
	UEdGraphPin* DstPropertyNamePin = DstCallFunction->FindPinChecked(SetPropertyHelper::PropertyNamePinName);
	CompilerContext.MovePinLinksToIntermediate(*GetPropertyNamePin(), *DstPropertyNamePin);

	// Value pin
	UEdGraphPin* DstValuePin = DstCallFunction->FindPinChecked(SetPropertyHelper::ValuePinName);
	DstValuePin->PinType = SrcValuePin->PinType;
	DstValuePin->PinType.PinSubCategoryObject = SrcValuePin->PinType.PinSubCategoryObject;
	CompilerContext.MovePinLinksToIntermediate(*SrcValuePin, *DstValuePin);

	// Then pin
	UEdGraphPin* DstThenPin = DstCallFunction->FindPinChecked(UEdGraphSchema_K2::PN_Then);
	CompilerContext.MovePinLinksToIntermediate(*GetThenPin(), *DstThenPin);

	BreakAllNodeLinks();
}

UK2Node_CallFunction* UK2Node_SetUObjectPropertyByName::GetK2NodeDstCallFunction(class FKismetCompilerContext& CompilerContext, UEdGraph* SourceGraph)
{
	// SPBN: Set Property By Name: DstCallFunction
	UK2Node_CallFunction* DstCallFunction = nullptr;
	EPinContainerType ContainerType = GetValuePin()->PinType.ContainerType;
	switch (ContainerType)
	{
	case EPinContainerType::None:
	{

		const FName FunctionName = GET_FUNCTION_NAME_CHECKED(UK2UObjectFunctionLibrary, SetSingelPropertyByName);
		DstCallFunction = CompilerContext.SpawnIntermediateNode<UK2Node_CallFunction>(this, SourceGraph);
		DstCallFunction->FunctionReference.SetExternalMember(FunctionName, UK2UObjectFunctionLibrary::StaticClass());
		DstCallFunction->AllocateDefaultPins();
		break;
	}
	case EPinContainerType::Array:
	{
		// static void SetArrayPropertyByName(UObject* Object, FName PropertyName, const TArray<int32>& Value);
		const FName FunctionName = GET_FUNCTION_NAME_CHECKED(UKismetArrayLibrary, SetArrayPropertyByName);
		DstCallFunction = CompilerContext.SpawnIntermediateNode<UK2Node_CallFunction>(this, SourceGraph);
		DstCallFunction->FunctionReference.SetExternalMember(FunctionName, UKismetArrayLibrary::StaticClass());
		DstCallFunction->AllocateDefaultPins();
		break;
	}
	case EPinContainerType::Map:
	{
		// static void SetMapPropertyByName(UObject* Object, FName PropertyName, const TMap<int32, int32>& Value);
		const FName FunctionName = GET_FUNCTION_NAME_CHECKED(UBlueprintMapLibrary, SetMapPropertyByName);
		DstCallFunction = CompilerContext.SpawnIntermediateNode<UK2Node_CallFunction>(this, SourceGraph);
		DstCallFunction->FunctionReference.SetExternalMember(FunctionName, UBlueprintMapLibrary::StaticClass());
		DstCallFunction->AllocateDefaultPins();
		break;
	}
	case EPinContainerType::Set:
	{
		// static void SetSetPropertyByName(UObject* Object, FName PropertyName, const TSet<int32>& Value);
		const FName FunctionName = GET_FUNCTION_NAME_CHECKED(UBlueprintSetLibrary, SetSetPropertyByName);
		DstCallFunction = CompilerContext.SpawnIntermediateNode<UK2Node_CallFunction>(this, SourceGraph);
		DstCallFunction->FunctionReference.SetExternalMember(FunctionName, UBlueprintSetLibrary::StaticClass());
		DstCallFunction->AllocateDefaultPins();
		break;
	}
	}
	return DstCallFunction;
}

FSlateIcon UK2Node_SetUObjectPropertyByName::GetIconAndTint(FLinearColor& OutColor) const
{
	OutColor = GetNodeTitleColor();
	static FSlateIcon Icon("EditorStyle", "Kismet.AllClasses.FunctionIcon");
	return Icon;
}

void UK2Node_SetUObjectPropertyByName::PostReconstructNode()
{
	Super::PostReconstructNode();

	PropagateValuePinType();
}


void UK2Node_SetUObjectPropertyByName::NotifyPinConnectionListChanged(UEdGraphPin* Pin)
{
	Super::NotifyPinConnectionListChanged(Pin);

	if (Pin == GetValuePin())
	{
		PropagateValuePinType();
	}
}

#undef LOCTEXT_NAMESPACE
