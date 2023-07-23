﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "MainCharacter.h"
#include "GamePlayMode.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
AMainCharacter::AMainCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 720.0f, 0.0f); // ...at this rotation rate

	GetCharacterMovement()->MaxWalkSpeed = 250.f;
}

// Called when the game starts or when spawned
void AMainCharacter::BeginPlay()
{
	Super::BeginPlay();


	AGameModeBase* GameModePtr = UGameplayStatics::GetGameMode(GetWorld());
	if (nullptr == GameModePtr)
	{
		return;
	}

	AGamePlayMode* GamePlayMode = Cast<AGamePlayMode>(GameModePtr);
	PlayMode = GamePlayMode;
	
}

// Called every frame
void AMainCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	

	if (PlayMode == nullptr)
	{
		return;
	}


	switch (aniState)
	{
	case PLAYER_ANISTATE::JUMP:		
		if (JumpCurrentCount == 0)
		{
			if (vInputDir.IsZero())
			{
				aniState = PLAYER_ANISTATE::LAND;
			}
			else
			{
				aniState = PLAYER_ANISTATE::RUN;
			}
		}
		break;
	case PLAYER_ANISTATE::ATTACK1:
	case PLAYER_ANISTATE::ATTACK2:
	case PLAYER_ANISTATE::ATTACK3:
	case PLAYER_ANISTATE::ATTACK4:
	case PLAYER_ANISTATE::ATTACK_DASH:
		fComboTime += DeltaTime;
		break;
	default:
		GetCharacterMovement()->GroundFriction = 8.f;
		fComboTime = 0.f;
		break;
	}

	//UE_LOG(LogTemp, Log, TEXT("%d"), (int)aniState);
	PlayMode->SetWidgetText(GetActorLocation().ToString());

}

// Called to bind functionality to input
void AMainCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);


	static bool bBindingsAdded = false;
	if (!bBindingsAdded)
	{
		bBindingsAdded = true;

		UPlayerInput::AddEngineDefinedAxisMapping(FInputAxisKeyMapping("Player_MoveForward", EKeys::W, 1.f));
		UPlayerInput::AddEngineDefinedAxisMapping(FInputAxisKeyMapping("Player_MoveForward", EKeys::S, -1.f));

		UPlayerInput::AddEngineDefinedAxisMapping(FInputAxisKeyMapping("Player_MoveRight", EKeys::A, -1.f));
		UPlayerInput::AddEngineDefinedAxisMapping(FInputAxisKeyMapping("Player_MoveRight", EKeys::D, 1.f));


		UPlayerInput::AddEngineDefinedAxisMapping(FInputAxisKeyMapping("Player_Dash", EKeys::LeftShift, 1.f));


		UPlayerInput::AddEngineDefinedAxisMapping(FInputAxisKeyMapping("Player_Turn", EKeys::MouseX, 1.f));
		UPlayerInput::AddEngineDefinedAxisMapping(FInputAxisKeyMapping("Player_LookUp", EKeys::MouseY, -1.f));


		UPlayerInput::AddEngineDefinedActionMapping(FInputActionKeyMapping("Player_Jump", EKeys::SpaceBar));
		UPlayerInput::AddEngineDefinedActionMapping(FInputActionKeyMapping("Player_Click", EKeys::LeftMouseButton));


	}

	PlayerInputComponent->BindAxis("Player_MoveForward", this, &AMainCharacter::MoveForward);
	PlayerInputComponent->BindAxis("Player_MoveRight", this, &AMainCharacter::MoveRight);
	PlayerInputComponent->BindAxis("Player_Turn", this, &AMainCharacter::TurnCamera);
	PlayerInputComponent->BindAxis("Player_LookUp", this, &AMainCharacter::LookUpCamera);
	PlayerInputComponent->BindAxis("Player_Dash", this, &AMainCharacter::Dash);

	PlayerInputComponent->BindAction("Player_Jump", EInputEvent::IE_Pressed, this, &AMainCharacter::PlayerJump);
	PlayerInputComponent->BindAction("Player_Click", EInputEvent::IE_Pressed, this, &AMainCharacter::Attack);

}


void AMainCharacter::MoveRight(float Val)
{
	vInputDir.Y = Val;

	switch (aniState)
	{
	case PLAYER_ANISTATE::JUMP:
	case PLAYER_ANISTATE::LAND:
	case PLAYER_ANISTATE::ATTACK1:
	case PLAYER_ANISTATE::ATTACK2:
	case PLAYER_ANISTATE::ATTACK3:
	case PLAYER_ANISTATE::ATTACK4:
	case PLAYER_ANISTATE::ATTACK_DASH:
		return;
	default:
		break;
	}

	if (Val != 0.f)
	{
		FAttachmentTransformRules Rule = FAttachmentTransformRules::KeepRelativeTransform;
		Rule.ScaleRule = EAttachmentRule::KeepWorld;
		WeaponPtr->AttachToComponent(WeaponPtr->GetAttachParent(), Rule, FName("Sword_Wait"));

		if (bIsDash == true)
		{
			aniState = PLAYER_ANISTATE::DASH;
		}
		else
		{
			aniState = PLAYER_ANISTATE::RUN;
		}

		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(RightDirection, Val);
	}
	else
	{
		if (vInputDir.IsZero())
		{
			aniState = PLAYER_ANISTATE::IDLE;
		}
	}
}

void AMainCharacter::MoveForward(float Val)
{
	vInputDir.X = Val;
	switch (aniState)
	{
	case PLAYER_ANISTATE::JUMP:
	case PLAYER_ANISTATE::LAND:
	case PLAYER_ANISTATE::ATTACK1:
	case PLAYER_ANISTATE::ATTACK2:
	case PLAYER_ANISTATE::ATTACK3:
	case PLAYER_ANISTATE::ATTACK4:
	case PLAYER_ANISTATE::ATTACK_DASH:
		return;
	default:
		break;
	}

	if (Val != 0.f)
	{
		FAttachmentTransformRules Rule = FAttachmentTransformRules::KeepRelativeTransform;
		Rule.ScaleRule = EAttachmentRule::KeepWorld;
		WeaponPtr->AttachToComponent(WeaponPtr->GetAttachParent(), Rule, FName("Sword_Wait"));

		if (bIsDash == true)
		{
			aniState = PLAYER_ANISTATE::DASH;
		}
		else
		{
			aniState = PLAYER_ANISTATE::RUN;
		}

		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		// add movement 
		AddMovementInput(ForwardDirection, Val);
	}
	else
	{
		if (vInputDir.IsZero())
		{
			aniState = PLAYER_ANISTATE::IDLE;
		}
	}
}

void AMainCharacter::Dash(float Val)
{
	switch (aniState)
	{
	case PLAYER_ANISTATE::JUMP:
	case PLAYER_ANISTATE::LAND:
	case PLAYER_ANISTATE::ATTACK1:
	case PLAYER_ANISTATE::ATTACK2:
	case PLAYER_ANISTATE::ATTACK3:
	case PLAYER_ANISTATE::ATTACK4:
	case PLAYER_ANISTATE::ATTACK_DASH:
		return;
	default:
		break;
	}
	if (Val == 1.f)
	{
		if (bIsDash == false)
		{
			bIsDash = true;
			GetCharacterMovement()->MaxWalkSpeed = 400.f;
		}
	}
	else
	{
		if (bIsDash == true)
		{
			bIsDash = false;
			GetCharacterMovement()->MaxWalkSpeed = 250.f;
		}
	}
}


void AMainCharacter::PlayerJump()
{
	if (aniState == PLAYER_ANISTATE::JUMP || aniState == PLAYER_ANISTATE::LAND)
	{
		return;
	}

	aniState = PLAYER_ANISTATE::JUMP;
	Jump();
}

void AMainCharacter::Attack()
{
	UE_LOG(LogTemp, Log, TEXT("현재 %d"), (int)aniState);
	switch (aniState)
	{
	case PLAYER_ANISTATE::DASH:
		GetCharacterMovement()->GroundFriction = 0;
		GetCharacterMovement()->AddImpulse(GetActorForwardVector() * 500, true);
		aniState = PLAYER_ANISTATE::ATTACK_DASH;
		break;
	case PLAYER_ANISTATE::IDLE:
	case PLAYER_ANISTATE::RUN:
	{
		aniState = PLAYER_ANISTATE::ATTACK1;
		FAttachmentTransformRules Rule = FAttachmentTransformRules::KeepRelativeTransform;
		Rule.ScaleRule = EAttachmentRule::KeepWorld;
		WeaponPtr->AttachToComponent(WeaponPtr->GetAttachParent(), Rule, FName("Weapon_R"));
		break;
	}
	case PLAYER_ANISTATE::ATTACK1:
		if (fComboTime > 0.3f)
		{
			fComboTime = 0.f;
			aniState = PLAYER_ANISTATE::ATTACK2;
			break;
		}
	case PLAYER_ANISTATE::ATTACK2:
		if (fComboTime > 0.3f)
		{
			fComboTime = 0.f;
			aniState = PLAYER_ANISTATE::ATTACK3;
			break;
		}
	case PLAYER_ANISTATE::ATTACK3:
		if (fComboTime > 0.3f)
		{
			fComboTime = 0.f;
			aniState = PLAYER_ANISTATE::ATTACK4;
			break;
		}
	case PLAYER_ANISTATE::ATTACK4:
		if (fComboTime > 0.9f)
		{
			fComboTime = 0.f;
			aniState = PLAYER_ANISTATE::ATTACK1;
			break;
		}
	case PLAYER_ANISTATE::ATTACK_DASH:
		if (fComboTime > 0.6f)
		{
			fComboTime = 0.f;
			aniState = PLAYER_ANISTATE::ATTACK1;
			break;
		}
	default:
		break;
	}
	UE_LOG(LogTemp, Log, TEXT("바꿈 %d"), (int)aniState);
}

void AMainCharacter::TurnCamera(float Val)
{
	AddControllerYawInput(Val * 0.5f);
}

void AMainCharacter::LookUpCamera(float Val)
{
	AddControllerPitchInput(Val * 0.5f);
}
