// Fill out your copyright notice in the Description page of Project Settings.


#include "XPlayerAnimation/XAnimInstance.h"

#include "Controllers/XPlayerController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PawnMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "PlayerCharacter/XCharacter.h"


void UXAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	XCharacter = Cast<AXCharacter>(TryGetPawnOwner());
}

void UXAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
	Super::NativeUpdateAnimation(DeltaTime);
	if (XCharacter)
	{
		bIsInAir = XCharacter->GetMovementComponent()->IsFalling();
		GroundSpeed = XCharacter->GetVelocity().Length();
#pragma region Roll Pitch Yaw
		Roll = XCharacter->Roll; //UKismetMathLibrary::NormalizedDeltaRotator(XCharacter->GetBaseAimRotation(), XCharacter->GetActorRotation()).Roll;
		Pitch = UKismetMathLibrary::NormalizedDeltaRotator(XCharacter->GetBaseAimRotation(), XCharacter->GetActorRotation()).Pitch;
		Yaw = UKismetMathLibrary::NormalizedDeltaRotator(XCharacter->GetBaseAimRotation(), XCharacter->GetActorRotation()).Yaw;
#pragma endregion
#pragma region YawDelta 
		YawDelta = UKismetMathLibrary::FInterpTo(
			YawDelta,
			UKismetMathLibrary::NormalizedDeltaRotator(RotationLastTick, XCharacter->GetActorRotation()).Yaw / (DeltaTime * 15.0f), //15.0f is lean intensity scalling you can change it
			DeltaTime,
			10.0f
			);
		RotationLastTick = XCharacter->GetActorRotation();
#pragma endregion
		bIsAccelerating = XCharacter->GetCharacterMovement()->GetCurrentAcceleration().Length() > 0.0f;

		Direction = UKismetMathLibrary::NormalizedDeltaRotator(XCharacter->GetActorRotation(), UKismetMathLibrary::Conv_VectorToRotator(XCharacter->GetVelocity())).Yaw * -1.0f;
		if(bIsAccelerating) StopDirection = Direction;
	}
}
