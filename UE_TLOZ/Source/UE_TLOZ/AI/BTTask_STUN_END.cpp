// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask_STUN_END.h"

EBTNodeResult::Type UBTTask_STUN_END::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	return EBTNodeResult::Type::InProgress;
}

void UBTTask_STUN_END::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);


	UAnimMontage* Montage = GetGlobalCharacter(OwnerComp)->GetAnimMontage(GetAiState(OwnerComp));
	float Time = Montage->CalculateSequenceLength();

	if (Time <= GetStateTime(OwnerComp))
	{
		SetStateChange(OwnerComp, MONSTER_AISTATE::CHASE);
		GetGlobalCharacter(OwnerComp)->Stunned(false);
		GetBlackboardComponent(OwnerComp)->SetValueAsBool(TEXT("Stun"), false);
		return;
	}
}
