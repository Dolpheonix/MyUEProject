// Fill out your copyright notice in the Description page of Project Settings.


#include "Giant.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "../../Utils/Helpers.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AGiant::AGiant()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	auto MainMesh = GetMesh();
	MainMesh->SetSkeletalMesh(Helpers::C_LoadObjectFromPath<USkeletalMesh>(TEXT("/Game/ShootingGame/Character/Main/Mesh/SK_Mannequin.SK_Mannequin")));
	Helpers::SetComponent<USkeletalMeshComponent>(&MainMesh, RootComponent, FVector(0.f, 0.f, -88.f), FRotator(0.0f, -90.0f, 0.f));
	MainMesh->SetAnimationMode(EAnimationMode::AnimationBlueprint);

	AIControllerClass = AGiant_Controller::StaticClass();
	SetActorScale3D(FVector(2.75f, 2.75f, 2.75f));
}

// Called when the game starts or when spawned
void AGiant::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AGiant::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AGiant::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AGiant::Attack(int elapsedFrame)
{
	// spawn rock ---> fire in direction
}

