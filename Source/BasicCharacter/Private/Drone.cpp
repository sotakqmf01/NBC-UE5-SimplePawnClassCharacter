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

	//FHitResult HitResult;  // 충돌 정보를 저장할 변수
	//FVector Start = GetActorLocation();  // 현재 위치
	//FVector End = Start + FVector(100, 0, 0);  // X축으로 100만큼 이동

	//FCollisionShape CollisionShape = FCollisionShape::MakeSphere(50);  // 반지름 50인 구 모양의 충돌 범위

	//bool bHit = GetWorld()->SweepSingleByChannel(HitResult, Start, End, FQuat::Identity, ECC_Visibility, CollisionShape);
	//if (bHit)
	//{
	//	UE_LOG(LogTemp, Warning, TEXT("충돌 발생! 충돌한 오브젝트: %s"), *HitResult.GetActor()->GetName());
	//}

	//FVector GunMuzzleLocation = GetActorLocation();  // 총구 위치
	//FVector TargetLocation = GunMuzzleLocation + GetActorForwardVector() * 1000.0f;  // 발사체의 경로

	//FCollisionQueryParams QueryParams;
	//QueryParams.AddIgnoredActor(this);  // 발사체는 자기 자신을 무시

	//FHitResult HitResult;
	//bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, GunMuzzleLocation, TargetLocation, ECC_Visibility, QueryParams);

	//if (bHit)
	//{
	//	UE_LOG(LogTemp, Warning, TEXT("발사체가 장애물에 맞았습니다: %s"), *HitResult.GetActor()->GetName());
	//}
	//else
	//{
	//	UE_LOG(LogTemp, Warning, TEXT("발사체가 장애물 없이 목표에 도달했습니다!"));
	//}

	//const FVector CameraForward = FRotationMatrix(FRotator(0.0f, 30.0f, 0.0f)).GetUnitAxis(EAxis::X);
	//const FVector CameraRight = FRotationMatrix(FRotator(0.0f, 30.0f, 0.0f)).GetUnitAxis(EAxis::Y);

	//const FVector DesiredDirection = (CameraForward) + (CameraRight);
	//const FVector DesiredDirection2 = FRotator(0.0f, 30.0f, 0.0f).RotateVector(FVector(1.0f, 1.0f, 0.0f));

	//UE_LOG(LogTemp, Warning, TEXT("Dir : %f, %f"), DesiredDirection.X, DesiredDirection.Y);
	//UE_LOG(LogTemp, Warning, TEXT("Dir2 : %f, %f"), DesiredDirection2.X, DesiredDirection2.Y);
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

// 이동할 때 그냥 축 나눠서 각각에 대해 이동을 했는데
// 과제 제출하고 나서 해설 코드를 보다보니
// 이렇게 하면 대각선으로 갈 때가 그냥 앞/뒤, 좌/우로만 갈 때보다 더 빠르게 된다는 걸 알아냈다
// 과제 코드도 정규화 처리 안하면 대각선으로 갈 때가 더 빠름
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
	//AddActorWorldOffset(FVector(0.0f, 0.0f, 1.0f) * MoveUpInput * Speed, true, &HitResult);
	//SetActorLocation(FVector(0.0f, 10.0f, 10.0f), true, &HitResult);
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

	//const FVector DesiredDirection3 = GetActorRotation().RotateVector(FVector(1.0f, 1.0f, 1.0f));
	//const FVector DesiredDirection4 = GetActorForwardVector() + GetActorRightVector() + GetActorUpVector();

	//UE_LOG(LogTemp, Warning, TEXT("Dir3 : %f, %f, %f"), DesiredDirection3.X, DesiredDirection3.Y, DesiredDirection3.Z);
	//UE_LOG(LogTemp, Warning, TEXT("Dir4 : %f, %f, %f"), DesiredDirection4.X, DesiredDirection4.Y, DesiredDirection4.Z);
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

	//GetActorLocation();
	//GetActorRotation();
	//Get

	if (!FMath::IsNearlyZero(RollRotationInput))
	{
		AddActorLocalRotation(FQuat(FRotator(0.0f, 0.0f, 1.0f) * RollRotationInput * 0.5f), true);
	}
}

//void ADrone::test(){
//	// 이동 시 World 기준으로 움직이도록
//	// ----------------------- 캐릭터 이동 -----------------------
//	MovementInput = value.Get<FVector2D>();
//	
//	const FRotator CameraRotation = CameraComp->GetComponentRotation();
//	const FRotator YawOnlyRotation(0.f, CameraRotation.Yaw, 0.f);
//	
//	// 2) 카메라 기준으로 Forward, Right 벡터 계산
//	// Unreal 기준: X = Forward, Y = Right, Z = Up
//	const FVector CameraForward = FRotationMatrix(YawOnlyRotation).GetUnitAxis(EAxis::X);
//	const FVector CameraRight = FRotationMatrix(YawOnlyRotation).GetUnitAxis(EAxis::Y);
//	
//	// 3) 입력값에 따른 이동 방향 결정
//	// MovementInput.X: 전후 (W/S), MovementInput.Y: 좌우 (A/D)
//	const FVector DesiredDirection = (CameraForward * MovementInput.X) + (CameraRight * MovementInput.Y);
//	//  3.2) CameraRotation의 Pitch랑 Roll이 0.0f면 밑에 코드도 똑같은 동작을 함
//	const FVector DesiredDirection2 = CameraRotation.RotateVector(MovementInput);
//	//  3.3) 아니면 그냥 Z축만 뽑아온걸 써도됨
//	const FVector DesiredDirection3 = YawOnlyRotation.RotateVector(MovementInput);
//	
//	// 4) 목표 이동 속도 계산 (정규화하여 최대 속도 적용)
//	const FVector DesiredVelocity = InputDirection.GetSafeNormal() * MaxWalkSpeed;
//	
//	// ----------------------- 드론 이동 -----------------------
//	// (2) 입력값으로부터 이동 방향 계산 (로컬 공간)
//	FVector LocalInput(ForwardInput, RightInput, UpInput);
//	
//	// (2).1) 현재 Actor의 회전 기준으로 입력 벡터를 월드 공간으로 변환
//	const FVector DesiredDirection = GetActorRotation().RotateVector(LocalInput);
//	const FVector DesiredDirection2 = GetActorForwardVector() * LocalInput.X
//										+ GetActorRightVector() * LocalInput.Y
//										+ GetActorRightVector() * LocalInput.Z;
//	const FVector DesiredVelocity = DesiredDirection * ActualMaxSpeed;
//}