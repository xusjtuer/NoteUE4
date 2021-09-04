// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "K2Node.h"
#include "K2Node_ArraySort.generated.h"

class FBlueprintActionDatabaseRegistrar;
class UEdGraphPin;
class UEdGraph;
struct FEdGraphPinType;

/**
 *
 */
UCLASS()
class AWESOMEBLUEPRINTSEDITOR_API UK2Node_ArraySort : public UK2Node
{
	GENERATED_UCLASS_BODY()

	//~ Begin UEdGraphNode Interface.
	virtual void AllocateDefaultPins() override;
	virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;
	virtual void PinDefaultValueChanged(UEdGraphPin* Pin) override;
	virtual FText GetTooltipText() const override;
	virtual void ExpandNode(class FKismetCompilerContext& CompilerContext, UEdGraph* SourceGraph) override;
	virtual FSlateIcon GetIconAndTint(FLinearColor& OutColor) const override;
	virtual void PostReconstructNode() override;
	//~ End UEdGraphNode Interface.

	//~ Begin UK2Node Interface
	virtual bool IsNodeSafeToIgnore() const override { return true; }
	virtual void GetMenuActions(FBlueprintActionDatabaseRegistrar& ActionRegistrar) const override;
	virtual FText GetMenuCategory() const override;
	//virtual bool IsConnectionDisallowed(const UEdGraphPin* MyPin, const UEdGraphPin* OtherPin, FString& OutReason) const override;
	virtual void EarlyValidation(class FCompilerResultsLog& MessageLog) const override;
	virtual void NotifyPinConnectionListChanged(UEdGraphPin* Pin) override;
	//~ End UK2Node Interface

	/** Get the then output pin */
	UEdGraphPin* GetThenPin()const { return Pins[1]; }

	/** Helper function to return the array pin */
	UEdGraphPin* GetTargetArrayPin() const { return Pins[2]; }

	/** Helper function to return the index pin */
	UEdGraphPin* GetPropertyNamePin() const { return Pins[3]; }

	/** Helper function to return the result pin */
	UEdGraphPin* GetAscendingPinPin() const { return Pins[4]; }

	/** Helper function to set default value of PropertyNamePin */
	void SetDefaultValueOfPropertyNamePin();

	/**	Helper function to get sortable property name of inner property of Target Array */
	TArray<FString> GetPropertyNames();

private:
	/**
	 * Takes the specified "MutatablePin" and sets its 'PinToolTip' field (according
	 * to the specified description)
	 *
	 * @param   MutatablePin	The pin you want to set tool-tip text on
	 * @param   PinDescription	A string describing the pin's purpose
	 */
	void SetPinToolTip(UEdGraphPin& MutatablePin, const FText& PinDescription) const;

	/** Triggers a refresh which will update the node's widget; aimed at updating the dropdown menu for the PropertyName input */
	void RefreshPropertyNameOptions();

	/** Tooltip text for this node. */
	FText NodeTooltip;

	/** Propagates pin type to the Array pin and the PropertyName pin */
	void PropagatePinType(FEdGraphPinType& InType);
};
