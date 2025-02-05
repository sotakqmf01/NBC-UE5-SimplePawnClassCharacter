// Fill out your copyright notice in the Description page of Project Settings.

#include "TestPawn.h"
#include "Components/ArrowComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputComponent.h"
#include "MyPlayerController.h"
#include "Engine/GameEngine.h"

ATestPawn::ATestPawn()
{
	PrimaryActorTick.bCanEverTick = false;

	CapsuleComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComp"));
	SkeletalMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletaMesh"));
	ArrowComp = CreateDefaultSubobject<UArrowComponent>(TEXT("Arrow"));
	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));

	SetRootComponent(CapsuleComp);
	SkeletalMeshComp->SetupAttachment(CapsuleComp);
	ArrowComp->SetupAttachment(CapsuleComp);
	SpringArmComp->SetupAttachment(CapsuleComp);
	CameraComp->SetupAttachment(SpringArmComp);

	CapsuleComp->SetCapsuleHalfHeight(95.0f);
	CapsuleComp->SetCapsuleRadius(37.0f);
	
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> MeshAsset(TEXT("/Game/Resources/Characters/Meshes/SKM_Manny.SKM_Manny"));
	if (MeshAsset.Succeeded())
	{
		SkeletalMeshComp->SetSkeletalMesh(MeshAsset.Object);
	}
	SkeletalMeshComp->SetRelativeLocation(FVector(0.0f, 0.0f, -87.0f));
	SkeletalMeshComp->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));

	SpringArmComp->TargetArmLength = 200.0f;
	SpringArmComp->SetRelativeLocation(FVector(0.0f, 0.0f, 150.0f));
	SpringArmComp->bUsePawnControlRotation = false;

	CameraComp->SetRelativeRotation(FRotator(-20.0f, 0.0f, 0.0f));
	CameraComp->bUsePawnControlRotation = false;

	NormalSpeed = 600.0f;
	TotalDeltaTime = 0.0f;

	MouseSensitivity = 0.75f;

}

void ATestPawn::BeginPlay()
{
	Super::BeginPlay();
	
}

void ATestPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATestPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		if (AMyPlayerController* PlayerController = Cast<AMyPlayerController>(GetController()))
		{
			// �̵�
			if (PlayerController->MoveAction)
			{
				EnhancedInput->BindAction(PlayerController->MoveAction, ETriggerEvent::Triggered, this, &ATestPawn::Move);
			}
			// ȸ��
			if (PlayerController->LookAction)
			{
				EnhancedInput->BindAction(PlayerController->LookAction, ETriggerEvent::Triggered, this, &ATestPawn::Look);
			}
		}
	}
}

void ATestPawn::Move(const FInputActionValue& value)
{
	if (!Controller) return;

	FVector2D MoveInput = value.Get<FVector2D>();
	//GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Blue, FString::Printf(TEXT("MoveInput : (%f, %f)"), MoveInput.X, MoveInput.Y));
	float DeltaTime = GetWorld()->GetDeltaSeconds();
	float Speed = NormalSpeed * DeltaTime;

	if (!FMath::IsNearlyZero(MoveInput.X))
	{
		// GetActorForwardVector()�� world ���� FVector�� ��ȯ
		//FVector ForwardV = GetActorForwardVector() ;
		//GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Blue, FString::Printf(TEXT("ForwardV : (%f, %f, %f)"), ForwardV.X, ForwardV.Y, ForwardV.Z));
		//AddActorWorldOffset(ForwardV * MoveInput.X);

		AddActorLocalOffset(FVector(1.0f, 0.0f, 0.0f) * MoveInput.X * Speed);
	}

	if (!FMath::IsNearlyZero(MoveInput.Y))
	{
		AddActorLocalOffset(FVector(0.0f, 1.0f, 0.0f) * MoveInput.Y * Speed);
	}
}

void ATestPawn::Look(const FInputActionValue& value)
{
	FVector2D LookInput = value.Get<FVector2D>();
	//GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Blue, FString::Printf(TEXT("LookInput : (%f, %f)"), LookInput.X, LookInput.Y));

	if (!FMath::IsNearlyZero(LookInput.X))
	{
		AddActorLocalRotation(FRotator(0.0f, 1.0f, 0.0f) * LookInput.X * MouseSensitivity);
	}

	if (!FMath::IsNearlyZero(LookInput.Y))
	{
		// [SpringArmComp->bUsePawnControlRotation = true]�� ���
		//  - Pawn�� ������ SpringArmComp�� ���ư��� ���� => ī�޶� ����
		//  - ���콺�� �����̸� ī�޶�� ������ �ְ� Pawn�� ȸ���ϰ� ��
		//  - => ���콺�� �����̴� ���� Pawn�� ������ ī�޶� ���� ���� �ְ�, 
		//        Pawn�� ī�޶� �ޱ� ������ �������� ��쵵 �ִ�
		//    
		// [SpringArmComp->bUsePawnControlRotation = false]�� ���
		//  - Pawn�� ���������� SpringArmComp�� Pawn�� ���� ���ư� => [ī�޶� �׻� Pawn�� ������� ��]
		//  - ���콺�� ������ Pawn�� ��������
		//  - ���� Move() �Լ��� [ĳ���� �������� ����]�̵��� �����Ǿ� �ְ� + Pawn�� ȸ���� + ���� �ۿ��� ����
		//  - => �ϴ��� ���� ������ ���� ĳ���Ͱ� �ϴ÷� ������
		//AddActorLocalRotation(FRotator(1.0f, 0.0f, 0.0f) * -1.0f * LookInput.Y * MouseSensitivity);

		// ȸ�� ������ ������ �����ϱ� �̻���
		//SpringArmComp->AddLocalRotation(FRotator(1.0f, 0.0f, 0.0f) * -1.0f * LookInput.Y * MouseSensitivity);

		FRotator NewRotation = SpringArmComp->GetRelativeRotation();
		NewRotation.Pitch = FMath::Clamp(NewRotation.Pitch + (-1.0f * LookInput.Y * MouseSensitivity), -70.0f, 85.0f);
		SpringArmComp->SetRelativeRotation(NewRotation);
	}
}