// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

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
	UFUNCTION(BlueprintCallable)
	void AddItem(FName _ItemName, int _Num = 1, bool _Equip = false);

	const TArray<class UInvenItem*>& GetItems()
	{
		return InvenItems;
	}

	void RefreshInven();

private:
	UPROPERTY()
	TArray<class UInvenItem*> InvenItems;

	UPROPERTY()
	int MaxSlotNum = 0;
};
