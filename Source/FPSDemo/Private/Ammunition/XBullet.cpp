// Fill out your copyright notice in the Description page of Project Settings.


#include "Ammunition/XBullet.h"

#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"


// Sets default values
AXBullet::AXBullet()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BulletSM"));
	SetRootComponent(StaticMeshComponent);

	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollison"));
	CollisionComp->SetupAttachment(GetRootComponent());
	CollisionComp->OnComponentHit.AddDynamic(this, &AXBullet::OnHit);

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	ProjectileMovement->UpdatedComponent = StaticMeshComponent;
	ProjectileMovement->InitialSpeed = 3000.f;
	ProjectileMovement->MaxSpeed = 3000.f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = true;
}

// Called when the game starts or when spawned
void AXBullet::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AXBullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AXBullet::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	FVector NormalImpulse, const FHitResult& Hit)
{
	if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr) && OtherComp->IsSimulatingPhysics())
	{
		OtherComp->AddImpulseAtLocation(GetVelocity() * 100.0f, GetActorLocation());

		Destroy();
	}
}

