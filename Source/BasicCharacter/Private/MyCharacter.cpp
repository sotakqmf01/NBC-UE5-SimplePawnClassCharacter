// Fill out your copyright notice in the Description page of Project Settings.


#include "MyCharacter.h"
#include "EnhancedInputComponent.h"
#include "MyPlayerController.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Engine/GameEngine.h"

AMyCharacter::AMyCharacter()
{
	PrimaryActorTick.bCanEverTick = false;

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));

	SpringArmComp->SetupAttachment(RootComponent);
	CameraComp->SetupAttachment(SpringArmComp, USpringArmComponent::SocketName);

	SpringArmComp->TargetArmLength = 300.0f;
	//SpringArmComp->SetRelativeRotation(FRotator(30.0f, 0.0f, 0.0f));
	SpringArmComp->SetRelativeLocation(FVector(0.0f, 0.0f, 150.0f));
	SpringArmComp->bUsePawnControlRotation = true;		// Character가 아니라 [PlayerController의 회전]을 따르겠다

	CameraComp->SetRelativeRotation(FRotator(-20.0f, 0.0f, 0.0f));
	CameraComp->bUsePawnControlRotation = false;

	NormalSpeed = 600.0f;
	SprintSpeedMultiplier = 1.5f;
	SprintSpeed = NormalSpeed * SprintSpeedMultiplier;

	GetCharacterMovement()->MaxWalkSpeed = NormalSpeed;

	MouseSensitivity = 0.25f;

	JumpCount = 0;
}

void AMyCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void AMyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		if (AMyPlayerController* PlayerController = Cast<AMyPlayerController>(GetController()))
		{
			// 이동
			if (PlayerController->MoveAction)
			{
				EnhancedInput->BindAction(PlayerController->MoveAction, ETriggerEvent::Triggered, this, &AMyCharacter::Move);
			}
			// 점프
			if (PlayerController->JumpAction)
			{
				EnhancedInput->BindAction(PlayerController->JumpAction, ETriggerEvent::Triggered, this, &AMyCharacter::StartJump);
				EnhancedInput->BindAction(PlayerController->JumpAction, ETriggerEvent::Completed, this, &AMyCharacter::StopJump);
			}
			// 마우스
			if (PlayerController->LookAction)
			{
				EnhancedInput->BindAction(PlayerController->LookAction, ETriggerEvent::Triggered, this, &AMyCharacter::Look);
			}
			// 달리기
			if (PlayerController->SprintAction)
			{
				EnhancedInput->BindAction(PlayerController->SprintAction, ETriggerEvent::Triggered, this, &AMyCharacter::StartSprint);
				EnhancedInput->BindAction(PlayerController->SprintAction, ETriggerEvent::Completed, this, &AMyCharacter::StopSprint);
			}
		}
	}
}

// FInputActionValue는 UEnhancedInputComponent 안에 있음
void AMyCharacter::Move(const FInputActionValue& value)
{
	if (!Controller) return;

	const FVector2D MoveInput = value.Get<FVector2D>();

	if (!FMath::IsNearlyZero(MoveInput.X))
	{
		// 내부적으로 PawnMovementComponent에 구현되어 있는 함수 사용
		// 캐릭터를 World 기준으로 이동시킴
		AddMovementInput(GetActorForwardVector(), MoveInput.X);
	}

	if (!FMath::IsNearlyZero(MoveInput.Y))
	{
		AddMovementInput(GetActorRightVector(), MoveInput.Y);
	}
}

void AMyCharacter::StartJump(const FInputActionValue& value)
{
	if (value.Get<bool>())
	{
		// 내부적으로 CharacterMovementComponent에 구현되어 있는 함수 사용
		Jump();

		// Actor's current movement mode (walking, falling, etc).
		//	- walking : Walking on a surface, under the effects of friction, and able to "step up" barriers.Vertical velocity is zero.
		//	- falling : Falling under the effects of gravity, after jumping or walking off the edge of a surface.
		//	- flying : Flying, ignoring the effects of gravity.
		//	- swimming : Swimming through a fluid volume, under the effects of gravity and buoyancy.
		//	- custom : User - defined custom movement mode, including many possible sub - modes.
		// 키를 눌렀고 + 캐릭터의 상태가 MOVE_Falling이 아닐 때
		if (bPressedJump && (GetCharacterMovement()->MovementMode != MOVE_Falling))
		{
			JumpCount++;
			GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Yellow, FString::Printf(TEXT("JUMP(%d)"), JumpCount));
		}
	}
}

void AMyCharacter::StopJump(const FInputActionValue& value)
{
	if (!value.Get<bool>())
	{
		// 내부적으로 CharacterMovementComponent에 구현되어 있는 함수 사용
		StopJumping();
		
		//GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Yellow, TEXT("STOP JUMP"));
	}
}

void AMyCharacter::Look(const FInputActionValue& value)
{
	FVector2D LookInput = value.Get<FVector2D>();

	// 내부적으로 PlayerController에 구현되어 있는 함수 사용
	AddControllerYawInput(LookInput.X * MouseSensitivity);
	AddControllerPitchInput(LookInput.Y * MouseSensitivity);
}

void AMyCharacter::StartSprint(const FInputActionValue& value)
{
	GetCharacterMovement()->MaxWalkSpeed = SprintSpeed = NormalSpeed * SprintSpeedMultiplier;
}

void AMyCharacter::StopSprint(const FInputActionValue& value)
{
	GetCharacterMovement()->MaxWalkSpeed = NormalSpeed;
}