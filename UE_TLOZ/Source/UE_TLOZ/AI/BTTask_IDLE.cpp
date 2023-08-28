// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask_IDLE.h"

EBTNodeResult::Type UBTTask_IDLE::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	GetBlackboardComponent(OwnerComp)->SetValueAsObject(TEXT("TargetActor"), nullptr);

	return EBTNodeResult::Type::InProgress;
}

void UBTTask_IDLE::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	if (2.0f <= GetStateTime(OwnerComp))
	{
		int RandomInt = UGlobalStatic::MainRandom.RandRange(1, 10);
		if (RandomInt <= 3) //30%
		{
			SetStateChange(OwnerComp, MONSTER_AISTATE::SEARCH);
			return;
		}
		else if (RandomInt <= 5) //20%
		{
			//patroll
		}
		else if (RandomInt <= 10) //10%
		{
			if (static_cast<MONSTER_TYPE>(GetBlackboardComponent(OwnerComp)->GetValueAsEnum(TEXT("MonsterType"))) == MONSTER_TYPE::BOSS_HINOX)
			{

				SetStateChange(OwnerComp, MONSTER_AISTATE::SLEEP_START);
				return;
			}
			//patroll
		}
		// else IDLE ����
		ResetStateTime(OwnerComp);
		return;
	}

	AActor* ResultTarget = Cast<AActor>(GetBlackboardComponent(OwnerComp)->GetValueAsObject(TEXT("TargetActor")));

	//��Ī �Ÿ����� ����
	if (nullptr != ResultTarget)
	{
		FVector Dist = ResultTarget->GetActorLocation() - GetGlobalCharacter(OwnerComp)->GetActorLocation();
		Dist.Z = 0;
		FVector Forward = GetGlobalCharacter(OwnerComp)->GetActorForwardVector();
		Forward.Z = 0;
		float TargetDegree = Dist.Rotation().Yaw - Forward.Rotation().Yaw;

		float Range = GetBlackboardComponent(OwnerComp)->GetValueAsFloat(TEXT("SearchRange"));
		
		if (FMath::Abs(TargetDegree) < 90.f)
		{
			//��Ī�Ÿ� 50%�� +-90�� �̳��� �����ϸ�
			if (Dist.Size() < Range * 0.5f)
			{
				SetStateChange(OwnerComp, MONSTER_AISTATE::FIND);
				return;
			}
		}
		else
		{
			//���� �ȿ� �ȵ��͵� ��Ī�Ÿ� 10%�� ���������
			if (Dist.Size() < Range * 0.1f)
			{
				SetStateChange(OwnerComp, MONSTER_AISTATE::FIND);
				return;
			}
		}
	}

	return;
}
