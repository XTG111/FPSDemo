// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSDemo/Public/PlayerCharacter/XCharacter.h"

#include "Components/XCombatrComponent.h"
#include "Controllers/XPlayerController.h"

// Sets default values
AXCharacter::AXCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	CombatComponent = CreateDefaultSubobject<UXCombatrComponent>(TEXT("CombatComponent"));
}

// Called when the game starts or when spawned
void AXCharacter::BeginPlay()
{
	Super::BeginPlay();
	Roll = Cast<AXPlayerController>(GetController())->Roll;
}

// Called every frame
void AXCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AXCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

