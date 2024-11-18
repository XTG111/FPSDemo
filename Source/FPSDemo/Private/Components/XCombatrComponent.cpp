// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/XCombatrComponent.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Ammunition/XBullet.h"
#include "Kismet/GameplayStatics.h"
#include "PlayerCharacter/XCharacter.h"

void UXCombatrComponent::BeginPlay()
{
	Super::BeginPlay();
	
	XCharacter = Cast<AXCharacter>(GetOwner());
	if (APlayerController* PlayerController = Cast<APlayerController>(XCharacter->GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			// Set the priority of the mapping to 1, so that it overrides the Jump action with the Fire action when using touch input
			Subsystem->AddMappingContext(CombatMappingContext, 1);
		}

		if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerController->InputComponent))
		{
			// Fire
			EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Triggered, this, &UXCombatrComponent::Fire);
		}
	}
}

void UXCombatrComponent::Fire()
{
	if(!XCharacter || !(XCharacter->GetController())) return;
	if(!bCanFire) return;
	bCanFire = false;
	check(SpawnBulletClass);
	UWorld* const World = GetWorld();
	if (World != nullptr)
	{
		APlayerController* PlayerController = Cast<APlayerController>(XCharacter->GetController());
		const FRotator SpawnRotation = PlayerController->PlayerCameraManager->GetCameraRotation();
		// MuzzleOffset is in camera space, so transform it to world space before offsetting from the character location to find the final muzzle position
		const FVector LeftSpawnLocation = XCharacter->GetMesh()->GetSocketLocation(FName("LeftBullet"));
		const FVector RightSpawnLocation = XCharacter->GetMesh()->GetSocketLocation(FName("RightBullet"));
	
		//Set Spawn Collision Handling Override
		FActorSpawnParameters ActorSpawnParams;
		ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;
	
		// Spawn the projectile at the muzzle
		World->SpawnActor<AXBullet>(SpawnBulletClass, LeftSpawnLocation, SpawnRotation, ActorSpawnParams);
		World->SpawnActor<AXBullet>(SpawnBulletClass, RightSpawnLocation, SpawnRotation, ActorSpawnParams);
		
	}
	if(FireSound)
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), FireSound, XCharacter->GetActorLocation());
	}
	if (FireAnimation)
	{
		// Get the animation object for the arms mesh
		UAnimInstance* AnimInstance = XCharacter->GetMesh()->GetAnimInstance();
		if (AnimInstance != nullptr)
		{
			AnimInstance->OnMontageEnded.Clear();
			AnimInstance->OnMontageEnded.AddDynamic(this, &UXCombatrComponent::EndOneFire);
			AnimInstance->Montage_Play(FireAnimation, 1.f);
		}
	}
}

void UXCombatrComponent::EndOneFire(UAnimMontage* Montage, bool bInterrupted)
{
	bCanFire = true;
}
