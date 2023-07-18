// Fill out your copyright notice in the Description page of Project Settings.


#include "Beacon.h"
#include "Components/BrushComponent.h"
#include "../../Utils/Helpers.h"
#include "Kismet/GameplayStatics.h"
#include "../../Character/Main/MainCharacter.h"

ABeacon::ABeacon()
{
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	Volume = CreateDefaultSubobject<UBoxComponent>(TEXT("Volume"));
	Helpers::SetComponent<UBoxComponent>(&Volume, RootComponent, FVector::ZeroVector, FRotator::ZeroRotator);
	Volume->SetBoxExtent(FVector(100.0f, 100.0f, 100.0f));
	Volume->SetCollisionProfileName("Volume");

	FXComponent = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("FX"));
	Helpers::SetComponent(&FXComponent, RootComponent, FVector::ZeroVector, FRotator::ZeroRotator);
	FXComponent->SetRelativeLocation(FVector(0.0f, 0.0f, -100.0f));

	Volume->OnComponentBeginOverlap.AddDynamic(this, &ABeacon::OnOverlapped_Arrival);
}

void ABeacon::RegisterQuest(FSingleQuest* quest)
{
	Ownerquest = quest;
	FXComponent->SetTemplate(quest->FXTemplate);
	Volume->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	Volume->SetCollisionProfileName("Volume");
	Volume->UpdateCollisionProfile();
}

void ABeacon::OnOverlapped_Arrival(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AMainCharacter* MainCharacter = Cast<AMainCharacter>(OtherActor);

	if (MainCharacter)
	{
		if(Ownerquest) MainCharacter->ReportArrival(Ownerquest);
		Destroy();
	}
}


