// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelActor.h"
#include <Kismet/GameplayStatics.h>
#include "UI/PreviewActor.h"

ALevelActor::ALevelActor()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ALevelActor::BeginPlay()
{
	Super::BeginPlay();

	deadPoint = -1000.0f;

	Player = UGameplayStatics::GetPlayerPawn(this, 0);
	startLocation = Player->GetActorLocation();
	startRotation = Player->GetController()->GetControlRotation();

	auto BP_PreviewClass = Helpers::LoadObjectFromPath<UBlueprint>(TEXT("/Game/ShootingGame/Blueprint/UI/PreviewActor/BP_PreiviewActor.BP_PreiviewActor"));
	FActorSpawnParameters params;
	params.Owner = this;
	params.Instigator = GetInstigator();
	GetWorld()->SpawnActor<APreviewActor>(BP_PreviewClass->GeneratedClass, FVector(0.0f, 0.0f, -2912.0f), FRotator(0.0f, 0.0f, 0.0f), params);
}

void ALevelActor::Tick(float DeltaTime) 
{
	Super::Tick(DeltaTime);

	float pawnHeight = Player->GetActorLocation().Z;

	if (pawnHeight < deadPoint)
	{
		bIsDead = true;
	}

	if (bIsDead)
	{
		Player->SetActorLocation(startLocation);
		Player->GetController()->SetControlRotation(startRotation);
		bIsDead = false;
	}
}