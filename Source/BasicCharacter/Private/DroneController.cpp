// Fill out your copyright notice in the Description page of Project Settings.


#include "DroneController.h"
#include "EnhancedInputSubsystems.h"

ADroneController::ADroneController()
	: DroneIMC(nullptr), MoveForwardAction(nullptr),
	MoveRightAction(nullptr), MoveUpAction(nullptr),
	YawRotationAction(nullptr), PitchRotationAction(nullptr),
	RollRotationAction(nullptr)
{

}

void ADroneController::BeginPlay()
{
	Super::BeginPlay();

	if (ULocalPlayer* LocalPlayer = GetLocalPlayer())
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
		{
			if (DroneIMC)
			{
				Subsystem->AddMappingContext(DroneIMC, 0);
			}
		}
	}
}