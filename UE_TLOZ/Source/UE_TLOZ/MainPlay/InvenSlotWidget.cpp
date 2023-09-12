// Fill out your copyright notice in the Description page of Project Settings.


#include "MainPlay/InvenSlotWidget.h"


void UInvenSlotWidget::AddInvenItem(UObject* _InvenItem)
{
	//���ο� ���� ������ ��
	UInvenItem* NewInvenItem = Cast<UInvenItem>(_InvenItem);
	if (NewInvenItem == nullptr)
	{
		return;
	}

	if (NewInvenItem->GetItemData() == nullptr) //��ĭ�� ���鶧
	{
		InvenItem = NewInvenItem;
		InvenItem->SetWidget(this);
	}
	else //������ ������ ���� ��
	{
		InvenItem = NewInvenItem;
		InvenItem->SetWidget(this);
	}
}

void UInvenSlotWidget::Refresh() //�κ��丮â ���������� �������� �ʿ��Ҷ�
{
	if (InvenItem == nullptr) //����
	{
		return;
	}
	if (InvenItem->GetItemData() == nullptr) //��ĭ�϶� ������Ʈ
	{
		InvenItem->SetWidget(this);
	}
	else
	{
		InvenItem->SetWidget(this);
	}
}
