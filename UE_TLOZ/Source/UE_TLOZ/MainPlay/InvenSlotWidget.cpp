// Fill out your copyright notice in the Description page of Project Settings.


#include "MainPlay/InvenSlotWidget.h"


void UInvenSlotWidget::AddInvenItem(UObject* _InvenItem)
{
	UInvenItem* NewInvenItem = Cast<UInvenItem>(_InvenItem);
	if (NewInvenItem == nullptr)
	{
		return;
	}


	if (NewInvenItem->ItemData == nullptr) //��ĭ���� ���鶧
	{
		InvenItem = NewInvenItem;
		InvenItem->Widget = this;
		ItemTexture = nullptr;
		ItemCnt = 0;
		bShowItem = false;
		bShowSlot = true;
		bIsItem = false;
		bIsEquip = false;
		bIsSelect = false;
		return;
	}

	if (InvenItem == nullptr) //�� ���Կ� �߰��� ��
	{
		InvenItem = NewInvenItem;
		InvenItem->Widget = this;
		ItemTexture = InvenItem->ItemData->SlotIcon;
		ItemCnt = NewInvenItem->ItemCnt;
		bShowItem = true;
		bShowSlot = true;
		bIsItem = true;
		bIsEquip = false;
		bIsSelect = false;
	}
	else
	{
		//���� ���Կ� �ٸ� �������� ������ ����

		//���� ���Կ� ���� �������� ������ ��ġ��
	}
}

void UInvenSlotWidget::Refresh() //�κ��丮â ���������� �������� �ʿ��Ҷ�
{
	if (InvenItem == nullptr) //����
	{
		return;
	}
	if (InvenItem->ItemData == nullptr) //��ĭ�϶� ������Ʈ
	{
		InvenItem->Widget = this;
		ItemTexture = nullptr;
		ItemCnt = 0;
		bShowItem = false;
		bShowSlot = true;
		bIsItem = false;
		bIsEquip = false;
		bIsSelect = false;
		return;
	}
	else
	{
		InvenItem->Widget = this;
		ItemTexture = InvenItem->ItemData->SlotIcon;
		ItemCnt = InvenItem->ItemCnt;
		bShowItem = true;
		bShowSlot = true;
		bIsItem = true;
		bIsEquip = false;
		bIsSelect = false;
	}
}