// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Drone.generated.h"

UCLASS()
class MYPROJECT2_API ADrone : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ADrone();

	//声明这些组件
	UPROPERTY(VisibleAnywhere)
	class UBoxComponent* OutCollision;
	
	UPROPERTY(VisibleAnywhere)/*‘UPROPERTY’为类提供垃圾回收功能； ’VisibleAnywhere‘任何地方可见*/
	class UStaticMeshComponent* Mesh;
	UPROPERTY(VisibleAnywhere)
	class UStaticMeshComponent* Paddle1;
	UPROPERTY(VisibleAnywhere)
	class UStaticMeshComponent* Paddle2;
	UPROPERTY(VisibleAnywhere)
	class UStaticMeshComponent* Paddle3;
	UPROPERTY(VisibleAnywhere)
	class UStaticMeshComponent* Paddle4;
	UPROPERTY(VisibleAnywhere)
	class UPhysicsThrusterComponent* UpThruster;
	UPROPERTY(VisibleAnywhere)
	class UPhysicsThrusterComponent* ForwardThruster;

	/*EditAnywhere,任何地方可编辑;
	*BlueprintReadWrite在蓝图里可浏览编辑
	* Category设置分类
	*/
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Init")
	float LiftAcc = 1000.0f;//上下升的速度
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Init")
	float LiftThrustMax = 2000.0f;//上下升的速度最大值限制
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Init")
	float ForwardAcc = 500.0f;//前进后退的速度
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Init")
	float ForwardThrustMax = 2000.0f;//前进后退的速度最大限制

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Init")
	float TurnStrength = 500000.0f;//旋转速度

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Init")
	float PaddleRotateSpeed = 500.0f;//扇叶旋转速度
	
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	UFUNCTION()
	void Lift(float val);
	UFUNCTION()
	void Forward(float val);
	UFUNCTION()
	void Turn(float val);

	TArray<UStaticMeshComponent*>Paddles;
	void RotatePaddle(float Delta);
	
};
