// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "XAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class FPSDEMO_API UXAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
public:
	//动画蓝图初始化重载
	virtual void NativeInitializeAnimation() override;

	//类似Event Tick函数
	virtual void NativeUpdateAnimation(float DeltaTime) override;

private:

	//对于私有变量，要能够被蓝图可见必须加上meta=(AllowPrivateAccess="true")
	UPROPERTY(BlueprintReadOnly, Category = Character, meta = (AllowPrivateAccess = "true"))
	class AXCharacter* XCharacter;
	UPROPERTY(BlueprintReadOnly, Category = MoveMent, meta = (AllowPrivateAccess = "true"))
	float GroundSpeed;
	UPROPERTY(BlueprintReadOnly, Category = MoveMent, meta = (AllowPrivateAccess = "true"))
	bool bIsAccelerating;
	UPROPERTY(BlueprintReadOnly, Category = MoveControl, meta = (AllowPrivateAccess = "true"))
	float Direction;
	UPROPERTY(BlueprintReadOnly, Category = MoveControl, meta = (AllowPrivateAccess = "true"))
	float StopDirection;
	UPROPERTY(BlueprintReadOnly, Category = IsInAir, meta = (AllowPrivateAccess = "true"))
	bool bIsInAir;
	
	UPROPERTY(BlueprintReadOnly, Category = Look, meta = (AllowPrivateAccess = "true"))
	float Roll;
	UPROPERTY(BlueprintReadOnly, Category = Look, meta = (AllowPrivateAccess = "true"))
	float Pitch;
	UPROPERTY(BlueprintReadOnly, Category = Look, meta = (AllowPrivateAccess = "true"))
	float Yaw;
	//use for leans
	UPROPERTY(BlueprintReadOnly, Category = Look, meta = (AllowPrivateAccess = "true"))
	float YawDelta;
	UPROPERTY(BlueprintReadOnly, Category = Look, meta = (AllowPrivateAccess = "true"))
	FRotator RotationLastTick;

	//equip
	UPROPERTY(BlueprintReadOnly, Category = Look, meta = (AllowPrivateAccess = "true"))
	bool bEquipWeapon = false;
	
};
