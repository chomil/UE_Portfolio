// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "InvenItem.h"
#include "Inventory.generated.h"

/**
 * 
 */
UCLASS()
class UE_TLOZ_API UInventory : public UObject
{

	GENERATED_BODY()
public:
	UInventory();
	~UInventory();

public:
	void AddItem(FName ItemName, int Num = 1);

	const TArray<UInvenItem*>& GetItems()
	{
		return InvenItems;
	}

	void RefreshInven();

private:
	UPROPERTY()
	TArray<UInvenItem*> InvenItems;
	UPROPERTY()
	int MaxSlotNum = 0;
};
