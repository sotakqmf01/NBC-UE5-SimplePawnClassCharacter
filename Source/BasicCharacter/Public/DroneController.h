// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "DroneController.generated.h"

class UInputMappingContext;
class UInputAction;

UCLASS()
class BASICCHARACTER_API ADroneController : public APlayerController
{
	GENERATED_BODY()

public:
	ADroneController();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input|IMC")
	UInputMappingContext* DroneIMC;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input|IA")
	UInputAction* MoveForwardAction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input|IA")
	UInputAction* MoveRightAction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input|IA")
	UInputAction* MoveUpAction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input|IA")
	UInputAction* YawRotationAction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input|IA")
	UInputAction* PitchRotationAction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input|IA")
	UInputAction* RollRotationAction;

protected:
	virtual void BeginPlay() override;
	
};
