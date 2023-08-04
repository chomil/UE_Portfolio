// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask_CHASE.h"

EBTNodeResult::Type UBTTask_CHASE::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	UCharacterMovementComponent* MoveCom = Cast<UCharacterMovementComponent>(GetGlobalCharacter(OwnerComp)->GetMovementComponent());

	if (nullptr != MoveCom)
	{
		MoveCom->MaxWalkSpeed = 300.0f;
	}

	return EBTNodeResult::Type::InProgress;
}

void UBTTask_CHASE::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DelataSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DelataSeconds);

	UObject* TargetObject = GetBlackboardComponent(OwnerComp)->GetValueAsObject(TEXT("TargetActor"));
	AActor* TargetActor = Cast<AActor>(TargetObject);

	//플레이어 놓침
	if (nullptr == TargetActor)
	{
		//SetStateChange(OwnerComp, MONSTER_AISTATE::RETURN);
		return;
	}

	float Degree = 0.f;
	//회전
	{
		FVector TargetPos = TargetActor->GetActorLocation();
		FVector ThisPos = GetGlobalCharacter(OwnerComp)->GetActorLocation();
		TargetPos.Z = 0.0f;
		ThisPos.Z = 0.0f;

		FVector Dir = TargetPos - ThisPos;
		Dir.Normalize();

		FVector ThisForward = GetGlobalCharacter(OwnerComp)->GetActorForwardVector();
		ThisForward.Normalize();

		FVector Cross = FVector::CrossProduct(ThisForward, Dir);
		float Angle0 = Dir.Rotation().Yaw;
		float Angle1 = ThisForward.Rotation().Yaw;
		Degree = Angle0 - Angle1;

		if (FMath::Abs(Degree) >= 5.0f)
		{
			FRotator Rot = FRotator::MakeFromEuler({ 0, 0, Cross.Z * 500.0f * DelataSeconds });
			GetGlobalCharacter(OwnerComp)->AddActorWorldRotation(Rot);
		}
		else {
			FRotator Rot = Dir.Rotation();
			GetGlobalCharacter(OwnerComp)->SetActorRotation(Rot);
		}
	}


	//이동
	{
		FVector PawnPos = GetGlobalCharacter(OwnerComp)->GetActorLocation();
		FVector TargetPos = TargetActor->GetActorLocation();
		PawnPos.Z = 0.0f;
		TargetPos.Z = 0.0f;

		FVector Dir = TargetPos - PawnPos;

		GetGlobalCharacter(OwnerComp)->AddMovementInput(Dir);

		FVector OriginPos = GetBlackboardComponent(OwnerComp)->GetValueAsVector(TEXT("OriginPos"));
		float SearchRange = GetBlackboardComponent(OwnerComp)->GetValueAsFloat(TEXT("SearchRange"));
		float AttackRange = GetBlackboardComponent(OwnerComp)->GetValueAsFloat(TEXT("AttackRange"));

		OriginPos.Z = 0.0f;
		FVector OriginDir = OriginPos - PawnPos;

		//if (OriginDir.Size() >= SearchRange * 1.5f)
		//{
		//	GetBlackboardComponent(OwnerComp)->SetValueAsObject(TEXT("TargetActor"), nullptr);
		//	SetStateChange(OwnerComp, MONSTER_AISTATE::RETURN);
		//	int a = 0;
		//	return;
		//}

		if (SearchRange * 1.2f < Dir.Size())
		{
			SetStateChange(OwnerComp, MONSTER_AISTATE::IDLE);
			return;
		}

		// 충분히 근접했다.
		if (Dir.Size() <= AttackRange)
		{
			//+-30도 이내면 공격 
			if (FMath::Abs(Degree) < 30.f)
			{
				int RandomInt = UGlobalStatic::MainRandom.RandRange(1, 3);
				if (RandomInt == 1)
				{
					SetStateChange(OwnerComp, MONSTER_AISTATE::ATTACK_STRONG);
				}
				else
				{
					SetStateChange(OwnerComp, MONSTER_AISTATE::ATTACK);
				}
				return;
			}
		}
		else if (AttackRange * 2 <= Dir.Size() && Dir.Size() <= AttackRange * 4)
		{
			//랜덤으로 점프공격
			int RandomInt = UGlobalStatic::MainRandom.RandRange(1, 100);
			if (RandomInt == 1)
			{
				SetStateChange(OwnerComp, MONSTER_AISTATE::ATTACK_JUMP_START);
				return;
			}
		}
	}
}
