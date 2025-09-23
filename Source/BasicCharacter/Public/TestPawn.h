// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "TestPawn.generated.h"

class UCapsuleComponent;
class UArrowComponent;
class USpringArmComponent;
class UCameraComponent;
struct FInputActionValue;

UCLASS()
class BASICCHARACTER_API ATestPawn : public APawn
{
	GENERATED_BODY()

public:
	ATestPawn();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player|Components")
	UCapsuleComponent* CapsuleComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player|Components")
	USkeletalMeshComponent* SkeletalMeshComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player|Components")
	UArrowComponent* ArrowComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player|Camera")
	USpringArmComponent* SpringArmComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player|Camera")
	UCameraComponent* CameraComp;

	float NormalSpeed;
	float TotalDeltaTime;

	float MouseSensitivity;

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void Move(const FInputActionValue& value);
	void Look(const FInputActionValue& value);

};
