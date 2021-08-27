// Fill out your copyright notice in the Description page of Project Settings.


#include "Drone.h"

#include <vector>

#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Kismet/KismetMaterialLibrary.h"
#include "PhysicsEngine/PhysicsThrusterComponent.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
ADrone::ADrone()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//定义这些组件
	OutCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("OutCollision"));
	RootComponent = OutCollision;/*把OutCollision作为父节点*/
	OutCollision->SetBoxExtent(FVector(100.0f,100.0f,100.0f));
	OutCollision->SetSimulatePhysics(true);
	OutCollision->BodyInstance.bLockXRotation = true;
	OutCollision->BodyInstance.bLockYRotation = true;
	
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(OutCollision);
	Paddle1 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Paddle1"));
	Paddle2 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Paddle2"));
	Paddle3 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Paddle3"));
	Paddle4 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Paddle4"));
	Paddle1->SetupAttachment(Mesh,TEXT("Paddle1"));/*把组件Paddle1连接到Mesh下面*/
	Paddle2->SetupAttachment(Mesh,TEXT("Paddle2"));
	Paddle3->SetupAttachment(Mesh,TEXT("Paddle3"));
	Paddle4->SetupAttachment(Mesh,TEXT("Paddle4"));
	Paddles.Add(Paddle1);
	Paddles.Add(Paddle2);
	Paddles.Add(Paddle3);
	Paddles.Add(Paddle4);


	UpThruster = CreateDefaultSubobject<UPhysicsThrusterComponent>(TEXT("UpThruster"));
	UpThruster->SetupAttachment(RootComponent);/*把组件UpThruster连接到RootComponent下面*/
	UpThruster->ThrustStrength = 980.0f;
	UpThruster->SetAutoActivate(true);/*默认情况下开启*/
	UpThruster->SetWorldRotation(UKismetMathLibrary::MakeRotFromX(-this->GetActorUpVector()));/*将推进器朝向下方*/

	ForwardThruster = CreateDefaultSubobject<UPhysicsThrusterComponent>(TEXT("ForwardThruster"));
	ForwardThruster->SetupAttachment(RootComponent);
	ForwardThruster->ThrustStrength = 0.0f;
	ForwardThruster->SetAutoActivate(true);
	ForwardThruster->SetWorldRotation(UKismetMathLibrary::MakeRotFromX(-this->GetActorForwardVector()));
}

// Called when the game starts or when spawned
void ADrone::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ADrone::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if(InputComponent->GetAxisValue(TEXT("Lift")) == 0.0f)
	{
		UpThruster->ThrustStrength = 980.0f;
	}

	if(InputComponent->GetAxisValue(TEXT("Forward")) == 0.0f)
	{
		ForwardThruster->ThrustStrength = 0.0f;
	}
	RotatePaddle(DeltaTime);

	if(Mesh->GetRelativeRotation().Pitch != 0.0f)
	{
		float currentPitch = Mesh->GetRelativeRotation().Pitch;
		
		Mesh->AddRelativeRotation(FRotator(-currentPitch * DeltaTime, 0.0f, 0.0f));
		if(FMath::Abs(Mesh->GetRelativeRotation().Pitch) <= KINDA_SMALL_NUMBER)
		{
			Mesh->SetRelativeRotation(FRotator(.0f,.0f,.0f));
		}
	}

	
}

// Called to bind functionality to input
void ADrone::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("Lift"),this,&ADrone::Lift);/*BindAxis绑定持续性输入*/
	PlayerInputComponent->BindAxis(TEXT("Forward"),this,&ADrone::Forward);
	PlayerInputComponent->BindAxis(TEXT("Turn"),this,&ADrone::Turn);
}

void ADrone::Lift(float val)
{
	UpThruster->ThrustStrength += val * LiftAcc * GetWorld()->DeltaTimeSeconds;
	UpThruster->ThrustStrength = FMath::Clamp(UpThruster->ThrustStrength,-LiftThrustMax,LiftThrustMax);
}

void ADrone::Forward(float val)
{
	ForwardThruster->ThrustStrength += val * ForwardAcc * GetWorld()->DeltaTimeSeconds;
	ForwardThruster->ThrustStrength = FMath::Clamp(ForwardThruster->ThrustStrength,-ForwardThrustMax,ForwardThrustMax);

	float pitch = Mesh->GetRelativeRotation().Pitch;
	if(FMath::Abs(pitch) < 30.0f)
	{
		Mesh->AddRelativeRotation(FRotator(-100.0f * GetWorld()->DeltaTimeSeconds * val,0.0f,0.0f));
	}
}

void ADrone::Turn(float val)
{
	OutCollision->AddTorqueInDegrees(-this->GetActorUpVector() * val * TurnStrength);
}

void ADrone::RotatePaddle(float Delta)
{
	for(auto Paddle:Paddles)
	{
		Paddle->AddRelativeRotation(FRotator(.0f,Delta * PaddleRotateSpeed, .0f));
	}

}

