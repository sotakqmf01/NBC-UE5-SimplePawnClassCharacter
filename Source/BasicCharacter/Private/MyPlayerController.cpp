// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Engine/GameEngine.h"

AMyPlayerController::AMyPlayerController()
	: InputMappingContext(nullptr),
	TabInputMappingContext(nullptr),
	MoveAction(nullptr),
	JumpAction(nullptr),
	LookAction(nullptr),
	SprintAction(nullptr),
	TapKeyAction(nullptr),
	CurrIMC(nullptr)
{

}

void AMyPlayerController::BeginPlay()
{
	Super::BeginPlay();

	// PlayerController와 연결되어 있는 플레이어 가져오기
	if (ULocalPlayer* LocalPlayer = GetLocalPlayer())
	{
		// 해당 플레이어의 IMC를 관리하는 Subsystem을 가져오기
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
		{
			// IMC를 할당
			if (InputMappingContext)
			{
				Subsystem->AddMappingContext(InputMappingContext, 0);
				CurrIMC = InputMappingContext;
			}
		}
	}
}

void AMyPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(InputComponent))
	{
		// Tab 키
		if (TapKeyAction)
		{
			EnhancedInput->BindAction(TapKeyAction, ETriggerEvent::Started, this, &AMyPlayerController::ChangeIMC);
		}
	}
}

void AMyPlayerController::ChangeIMC(const FInputActionValue& value)
{
	GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Blue, TEXT("Change IMC"));

	if (ULocalPlayer* LocalPlayer = GetLocalPlayer())
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
		{
			if (CurrIMC != TabInputMappingContext)
			{
				Subsystem->RemoveMappingContext(InputMappingContext);
				Subsystem->AddMappingContext(TabInputMappingContext, 1);
				CurrIMC = TabInputMappingContext;
				GEngine->AddOnScreenDebugMessage(-1, 30.0f, FColor::Blue, TEXT("TabInputMappingContext IMC"));
			}
			else
			{
				Subsystem->RemoveMappingContext(TabInputMappingContext);
				Subsystem->AddMappingContext(InputMappingContext, 0);
				CurrIMC = InputMappingContext;
				GEngine->AddOnScreenDebugMessage(-1, 30.0f, FColor::Blue, TEXT("InputMappingContext IMC"));
			}

			// UEnhancedInputLocalPlayerSubsystem -> GetPlayerInput() -> UEnhancedPlayerInput -> GetEnhancedActionMappings()
			// => 현재 적용된 IMC들이 담긴 TArray<FEnhancedActionKeyMapping>를 얻을 수 있음
		}
	}
}