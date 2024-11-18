// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSDemo/Public/Controllers/XPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "GameFramework/Character.h"


AXPlayerController::AXPlayerController()
{
	bReplicates = true;
}

void AXPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	//限制视角上下
	float curPitch = GetControlRotation().Pitch;
	UE_LOG(LogTemp, Warning, TEXT("Current Pitch: %f"), curPitch);
	float clampedPitch = curPitch>270.f ? FMath::Clamp(curPitch, 300.0f, 360.0f) : FMath::Clamp(curPitch, 0.0f, 40.0f);
	SetControlRotation(FRotator(clampedPitch, GetControlRotation().Yaw, GetControlRotation().Roll));
}

void AXPlayerController::BeginPlay()
{
	Super::BeginPlay();
	check(XPlayerContext);
	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	if(Subsystem) //只有LocalPlayer的Client才会获得Subsystem
	{
		Subsystem->AddMappingContext(XPlayerContext, 0);
	}

}

void AXPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent);

	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AXPlayerController::Move);
	EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AXPlayerController::Look);

	EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &AXPlayerController::Jump);
	EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &AXPlayerController::StopJump);
}

void AXPlayerController::Move(const struct FInputActionValue& InputActionValue)
{
	const FVector2D MovementVector = InputActionValue.Get<FVector2D>();
	const FRotator Rotation = GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);

	// get forward vector
	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

	// get right vector 
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	// add movement 
	if (APawn* ControlledPawn = GetPawn<APawn>())
	{
		ControlledPawn->AddMovementInput(ForwardDirection, MovementVector.Y);
		ControlledPawn->AddMovementInput(RightDirection, MovementVector.X);
	}

}

void AXPlayerController::Look(const struct FInputActionValue& InputActionValue)
{
	const FVector2D LookAxisVector = InputActionValue.Get<FVector2D>();
	const FRotator Rotation = GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);
	
	if (APawn* ControlledPawn = GetPawn<APawn>())
	{
		// add yaw and pitch input to controller
		ControlledPawn->AddControllerYawInput(LookAxisVector.X);
		ControlledPawn->AddControllerPitchInput(LookAxisVector.Y);
		Roll = FMath::Clamp(Roll + LookAxisVector.Y, -30.0f,30.0f);
	}
}

void AXPlayerController::Jump()
{
	if (ACharacter* ControlledPawn = GetCharacter())
	{
		ControlledPawn->Jump();
	}
}

void AXPlayerController::StopJump()
{
	if (ACharacter* ControlledPawn = GetCharacter())
	{
		ControlledPawn->StopJumping();
	}
}
