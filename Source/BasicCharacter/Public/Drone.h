// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Drone.generated.h"

class UBoxComponent;
class UArrowComponent;
class USpringArmComponent;
class UCameraComponent;
struct FInputActionValue;

UCLASS()
class BASICCHARACTER_API ADrone : public APawn
{
	GENERATED_BODY()

public:
	ADrone();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Drone|Components")
	UBoxComponent* BoxCollisionComp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Drone|Components")
	UStaticMeshComponent* StaticMeshComp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Drone|Components")
	UArrowComponent* ArrowComp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Drone|Components")
	USpringArmComponent* SpringArmComp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Drone|Components")
	UCameraComponent* CameraComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Drone|Properties")
	float SpeedXY;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Drone|Properties")
	float SpeedZ;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Drone|Properties")
	float MouseSensitivity;

protected:
	virtual void PostInitializeComponents() override;
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	// block되었을 때 Hit 이벤트 발생
	virtual void NotifyHit(UPrimitiveComponent* MyComp, AActor* Other, UPrimitiveComponent* OtherComp,
		bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImperse, const FHitResult& Hit) override;
	// overlap되었을 때 BeginOverlap 이벤트 발생
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
	// overlap이 끝났을 때 EndOverlap 이벤트 발생
	virtual void NotifyActorEndOverlap(AActor* OtherActor) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION()
	void MoveForward(const FInputActionValue& value);
	UFUNCTION()
	void MoveRight(const FInputActionValue& value);
	UFUNCTION()
	void MoveUp(const FInputActionValue& value);
	UFUNCTION()
	void YawRotation(const FInputActionValue& value);
	UFUNCTION()
	void PitchRotation(const FInputActionValue& value);
	UFUNCTION()
	void RollRotation(const FInputActionValue& value);

	//void test();
};
