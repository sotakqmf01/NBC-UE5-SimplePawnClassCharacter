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

	//FRotator ControlRotation = GetController()->GetControlRotation();
	//FRotator ControlRotationYaw(0.0f, ControlRotation.Yaw, 0.0f);

	//FVector ForwardVector = FRotationMatrix(ControlRotationYaw).GetUnitAxis(EAxis::X);
	//FVector RightVetor = FRotationMatrix(ControlRotationYaw).GetUnitAxis(EAxis::Y);

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
			// 이동
			if (PlayerController->MoveAction)
			{
				EnhancedInput->BindAction(PlayerController->MoveAction, ETriggerEvent::Triggered, this, &ATestPawn::Move);
			}
			// 회전
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
		// GetActorForwardVector()는 world 기준 FVector를 반환
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
		// [SpringArmComp->bUsePawnControlRotation = true]인 경우
		//  - Pawn을 돌려도 SpringArmComp가 돌아가지 않음 => 카메라도 고정
		//  - 마우스를 움직이면 카메라는 가만히 있고 Pawn만 회전하게 됨
		//  - => 마우스를 움직이다 보면 Pawn의 정면이 카메라를 향할 수도 있고, 
		//        Pawn이 카메라 앵글 밖으로 나가지는 경우도 있다
		//    
		// [SpringArmComp->bUsePawnControlRotation = false]인 경우
		//  - Pawn을 돌려버리면 SpringArmComp도 Pawn을 따라서 돌아감 => [카메라가 항상 Pawn의 뒤통수를 봄]
		//  - 마우스를 내리면 Pawn이 누워버림
		//  - 위에 Move() 함수가 [캐릭터 기준으로 움직]이도록 구현되어 있고 + Pawn이 회전함 + 물리 작용이 없음
		//  - => 하늘을 보고 앞으로 가면 캐릭터가 하늘로 움직임
		//AddActorLocalRotation(FRotator(1.0f, 0.0f, 0.0f) * -1.0f * LookInput.Y * MouseSensitivity);

		// 회전 각도에 제한이 없으니까 이상함
		//SpringArmComp->AddLocalRotation(FRotator(1.0f, 0.0f, 0.0f) * -1.0f * LookInput.Y * MouseSensitivity);

		FRotator NewRotation = SpringArmComp->GetRelativeRotation();
		NewRotation.Pitch = FMath::Clamp(NewRotation.Pitch + (-1.0f * LookInput.Y * MouseSensitivity), -70.0f, 85.0f);
		SpringArmComp->SetRelativeRotation(NewRotation);
	}
}