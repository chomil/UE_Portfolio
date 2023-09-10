// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InvenItem.h"
#include "Blueprint/UserWidget.h"
#include "InvenSlotWidget.generated.h"

/**
 * 
 */
UCLASS()
class UE_TLOZ_API UInvenSlotWidget : public UUserWidget
{
	GENERATED_BODY()


public:
	UFUNCTION(BlueprintCallable)
	void AddInvenItem(UObject* _Item);


	void Refresh();

public:

	UPROPERTY(Category = "Item", EditAnywhere, blueprintReadWrite)
	int ListIndex = 0;
	UPROPERTY(Category = "Item", EditAnywhere, blueprintReadWrite)
	UInvenItem* InvenItem = nullptr;
	UPROPERTY(Category = "Item", EditAnywhere, blueprintReadWrite)
	int ItemCnt = 0;

	UPROPERTY(Category = "Item", EditAnywhere, blueprintReadWrite)
	UTexture* ItemTexture = nullptr;

	UPROPERTY(Category = "Item", EditAnywhere, blueprintReadWrite)
	bool bShowItem = false;

	UPROPERTY(Category = "Item", EditAnywhere, blueprintReadWrite)
	bool bShowSlot = true;

	UPROPERTY(Category = "Item", EditAnywhere, blueprintReadWrite)
	bool bIsItem = false;

	UPROPERTY(Category = "Item", EditAnywhere, blueprintReadWrite)
	bool bIsEquip = false;

	UPROPERTY(Category = "Item", EditAnywhere, blueprintReadWrite)
	bool bIsSelect = false;
};