// Fill out your copyright notice in the Description page of Project Settings.


#include "MainPlay/Inventory.h"
#include <Global/GlobalGameInstance.h>
#include "InvenSlotWidget.h"

UInventory::UInventory()
{
	if (GetWorld() == nullptr)
	{
		return;
	}

	MaxSlotNum = 10;

	//�ʱ� �κ��丮 ĭ��ŭ ��ĭ ����
	for (int i = 0; i < MaxSlotNum; i++)
	{
		UInvenItem* InvenItem = NewObject<UInvenItem>(GetWorld());
		InvenItem->SetItemData(nullptr);
		InvenItems.Add(InvenItem);
	}

	//�ʱ� �κ��丮 ������ �߰�
	AddItem(TEXT("Sword"), 1, true);
	AddItem(TEXT("Bow"), 1, true);
	AddItem(TEXT("Apple"), 2);
	AddItem(TEXT("Herb"), 3);
	AddItem(TEXT("Apple"), 3);
	AddItem(TEXT("BokoHorn"), 2);
}

UInventory::~UInventory()
{
}

void UInventory::AddItem(FName _ItemName, int _Num, bool _Equip)
{
	const FItemData* ItemData = GetWorld()->GetGameInstance<UGlobalGameInstance>()->GetItemData(_ItemName);
	int GetIndex = -1;
	for (int i = 0; i < InvenItems.Num(); i++)
	{
		if (ItemData == InvenItems[i]->GetItemData()) //�̹� ������ �߰�
		{
			InvenItems[i]->AddItemCnt(_Num);
			return;
		}
		if (GetIndex == -1 && InvenItems[i]->GetItemData() == nullptr) 
		{
			GetIndex = i;
		}
	}

	if (InvenItems.IsValidIndex(GetIndex)) //������ ���� ���� ��ĭ�� �߰�
	{
		InvenItems[GetIndex]->SetItemData(ItemData, _Num, _Equip);
	}
}


void UInventory::RefreshInven() //�κ��丮 ĭ �ڵ� ����
{
	TArray<UInvenItem*> NewInvenItems;
	for (int i = 0; i < InvenItems.Num(); i++)
	{
		if (InvenItems[i]->GetItemData() != nullptr && InvenItems[i]->GetItemCnt() > 0)
		{
			NewInvenItems.Add(InvenItems[i]);
		}
	}

	for (int i = 0; i < MaxSlotNum; i++)
	{
		if (NewInvenItems.IsValidIndex(i))
		{
			InvenItems[i]->SetItemData(NewInvenItems[i]->GetItemData(), NewInvenItems[i]->GetItemCnt(), NewInvenItems[i]->GetIsEquip());
		}
		else
		{
			InvenItems[i]->SetItemData(nullptr);
		}
	}
}


