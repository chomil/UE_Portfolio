// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask_ATTACK_STRONG.h"

EBTNodeResult::Type UBTTask_ATTACK_STRONG::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	return EBTNodeResult::Type::InProgress;
}

void UBTTask_ATTACK_STRONG::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);


	UAnimMontage* Montage = GetGlobalCharacter(OwnerComp)->GetAnimMontage(GetAiState(OwnerComp));
	float Time = Montage->CalculateSequenceLength();

	if (Time <= GetStateTime(OwnerComp))
	{
		SetStateChange(OwnerComp, MONSTER_AISTATE::CHASE);
		return;
	}
}
