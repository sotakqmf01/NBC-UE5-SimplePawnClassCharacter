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

	// PlayerController�� ����Ǿ� �ִ� �÷��̾� ��������
	if (ULocalPlayer* LocalPlayer = GetLocalPlayer())
	{
		// �ش� �÷��̾��� IMC�� �����ϴ� Subsystem�� ��������
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
		{
			// IMC�� �Ҵ�
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
		// Tab Ű
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
			// => ���� ����� IMC���� ��� TArray<FEnhancedActionKeyMapping>�� ���� �� ����
		}
	}
}