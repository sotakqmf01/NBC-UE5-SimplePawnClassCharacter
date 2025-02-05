// Fill out your copyright notice in the Description page of Project Settings.

#include "Drone.h"
#include "Components/ArrowComponent.h"
#include "Components/BoxComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputComponent.h"
#include "DroneController.h"
#include "Engine/GameEngine.h"

ADrone::ADrone()
{
	PrimaryActorTick.bCanEverTick = false;

	BoxCollisionComp = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollisionComp"));
	StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	ArrowComp = CreateDefaultSubobject<UArrowComponent>(TEXT("Arrow"));
	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));

	SetRootComponent(BoxCollisionComp);
	StaticMeshComp->SetupAttachment(BoxCollisionComp);
	ArrowComp->SetupAttachment(BoxCollisionComp);
	SpringArmComp->SetupAttachment(BoxCollisionComp);
	CameraComp->SetupAttachment(SpringArmComp);

	// 액터 Details창 -> Collision -> Simulation Generates Hit Events 활성화
	BoxCollisionComp->SetNotifyRigidBodyCollision(true);

	BoxCollisionComp->SetCollisionEnabled(ECollisionEnabled::Type::QueryOnly);
	BoxCollisionComp->SetCollisionObjectType(ECC_WorldDynamic);
	BoxCollisionComp->SetCollisionResponseToAllChannels(ECR_Block);
	BoxCollisionComp->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset(TEXT("/Game/Resources/Shapes/Shape_Sphere.Shape_Sphere"));
	if (MeshAsset.Succeeded())
	{
		StaticMeshComp->SetStaticMesh(MeshAsset.Object);
	}

	StaticMeshComp->SetRelativeScale3D(FVector(1.75f, 1.75f, 0.5f));

	SpeedXY = 1000.0f;
	SpeedZ = 600.0f;

	MouseSensitivity = 0.75f;
}

// 블루프린트에서 이것저것 만져서 생성자에서 코드를 변경해도 적용이 안됨
// => 실행 시 원하는 값으로 적용하기 위해 PostInitializeComponents() 사용
void ADrone::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	BoxCollisionComp->SetNotifyRigidBodyCollision(true);

	BoxCollisionComp->SetCollisionEnabled(ECollisionEnabled::Type::QueryOnly);
	BoxCollisionComp->SetCollisionObjectType(ECC_WorldDynamic);
	BoxCollisionComp->SetCollisionResponseToAllChannels(ECR_Block);
	BoxCollisionComp->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
}

void ADrone::BeginPlay()
{
	Super::BeginPlay();

	//FHitResult Hit;
	//FCollisionQueryParams Params;
	//Params.AddIgnoredActor(this);

	//bool bHit = GetWorld()->LineTraceSingleByChannel(
	//	Hit,
	//	GetActorLocation(),
	//	GetActorLocation() + FVector(0, 0, -1000),
	//	ECC_Visibility,
	//	Params
	//);

	//UE_LOG(LogTemp, Warning, TEXT("Trace Hit: %s"), bHit ? TEXT("YES") : TEXT("NO"));
}

void ADrone::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ADrone::NotifyHit(UPrimitiveComponent* MyComp, AActor* Other, UPrimitiveComponent* OtherComp,
	bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImperse, const FHitResult& Hit)
{
	Super::NotifyHit(MyComp, Other, OtherComp, bSelfMoved, HitLocation, HitNormal, NormalImperse, Hit);

	GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, FString::Printf(TEXT("Notify Hit")));
}

void ADrone::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Blue, FString::Printf(TEXT("Notify BeginOverlap")));
}

void ADrone::NotifyActorEndOverlap(AActor* OtherActor)
{
	Super::NotifyActorEndOverlap(OtherActor);

	GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Cyan, FString::Printf(TEXT("Notify EndOverlap")));
}

void ADrone::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		if (ADroneController* DroneController = Cast<ADroneController>(GetController()))
		{
			if (DroneController->MoveForwardAction)
			{
				EnhancedInput->BindAction(DroneController->MoveForwardAction, ETriggerEvent::Triggered, this, &ADrone::MoveForward);
			}

			if (DroneController->MoveRightAction)
			{
				EnhancedInput->BindAction(DroneController->MoveRightAction, ETriggerEvent::Triggered, this, &ADrone::MoveRight);
			}

			if (DroneController->MoveUpAction)
			{
				EnhancedInput->BindAction(DroneController->MoveUpAction, ETriggerEvent::Triggered, this, &ADrone::MoveUp);
			}

			if (DroneController->YawRotationAction)
			{
				EnhancedInput->BindAction(DroneController->YawRotationAction, ETriggerEvent::Triggered, this, &ADrone::YawRotation);
			}

			if (DroneController->PitchRotationAction)
			{
				EnhancedInput->BindAction(DroneController->PitchRotationAction, ETriggerEvent::Triggered, this, &ADrone::PitchRotation);
			}

			if (DroneController->RollRotationAction)
			{
				EnhancedInput->BindAction(DroneController->RollRotationAction, ETriggerEvent::Triggered, this, &ADrone::RollRotation);
			}
		}
	}
}

void ADrone::MoveForward(const FInputActionValue& value)
{
	float MoveForwardInput = value.Get<float>();
	float DeltaTime = GetWorld()->GetDeltaSeconds();
	float Speed = SpeedXY * DeltaTime;
	//GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red, FString::Printf(TEXT("MoveForwardInput : %f"), MoveForwardInput));

	AddActorLocalOffset(FVector(1.0f, 0.0f, 0.0f) * MoveForwardInput * Speed, true);
}

void ADrone::MoveRight(const FInputActionValue& value)
{
	float MoveRightInput = value.Get<float>();
	float DeltaTime = GetWorld()->GetDeltaSeconds();
	float Speed = SpeedXY * DeltaTime;

	AddActorLocalOffset(FVector(0.0f, 1.0f, 0.0f) * MoveRightInput * Speed, true);
}

void ADrone::MoveUp(const FInputActionValue& value)
{
	float MoveUpInput = value.Get<float>();
	float DeltaTime = GetWorld()->GetDeltaSeconds();
	float Speed = SpeedZ * DeltaTime;

	AddActorLocalOffset(FVector(0.0f, 0.0f, 1.0f) * MoveUpInput * Speed, true);
}

void ADrone::YawRotation(const FInputActionValue& value)
{
	float YawRotationInput = value.Get<float>();

	//FRotator NextRotation = GetActorRotation();
	//NextRotation.Yaw = FMath::Clamp(NextRotation.Yaw + YawRotationInput * MouseSensitivity, -20.0f, 20.0f);
	//NextRotation.Yaw = NextRotation.Yaw + YawRotationInput * MouseSensitivity;

	if (!FMath::IsNearlyZero(YawRotationInput))
	{
		//SetActorRotation(NextRotation);
		//SetActorRelativeRotation(NextRotation);
		AddActorLocalRotation(FQuat(FRotator(0.0f, 1.0f, 0.0f) * YawRotationInput * MouseSensitivity), true);
	}
}

void ADrone::PitchRotation(const FInputActionValue& value)
{
	float PitchRotationInput = value.Get<float>();

	if (!FMath::IsNearlyZero(PitchRotationInput))
	{
		AddActorLocalRotation(FQuat(FRotator(1.0f, 0.0f, 0.0f) * PitchRotationInput * MouseSensitivity), true);
	}
}

void ADrone::RollRotation(const FInputActionValue& value)
{
	float RollRotationInput = value.Get<float>();

	if (!FMath::IsNearlyZero(RollRotationInput))
	{
		AddActorLocalRotation(FQuat(FRotator(0.0f, 0.0f, 1.0f) * RollRotationInput * 0.5f), true);
	}
}