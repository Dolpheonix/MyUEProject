// Fill out your copyright notice in the Description page of Project Settings.


#include "Portal.h"
#include "../../Utils/Helpers.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
APortal::APortal()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	Volume = CreateDefaultSubobject<UBoxComponent>(TEXT("Volume"));
	Helpers::SetComponent<UBoxComponent>(&Volume, RootComponent, FVector(0.0f, 0.0f, 0.0f), FRotator(0.0f, 0.0f, 0.0f));
	Volume->SetBoxExtent(FVector(100.0f, 100.0f, 100.0f));
	Volume->SetCollisionProfileName("Volume");

	FXComponent = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("FX"));
	Helpers::SetComponent<UParticleSystemComponent>(&FXComponent, Volume, FVector(0.0f, 0.0f, 0.0f), FRotator(0.0f, 0.0f, 0.0f));

	Destination = CreateDefaultSubobject<UArrowComponent>(TEXT("Arrow"));
	Helpers::SetComponent<UArrowComponent>(&Destination, Volume, FVector(0.0f, 0.0f, 0.0f), FRotator(0.0f, 0.0f, 0.0f));
	Destination->SetWorldLocation(GetActorLocation() + FVector(Volume->GetScaledBoxExtent().X + 100.0f, 0.0f, 0.0f));
	Destination->SetMobility(EComponentMobility::Movable);

	Volume->OnComponentBeginOverlap.AddDynamic(this, &APortal::OnOverlapped_Teleport);
}

// Called when the game starts or when spawned
void APortal::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APortal::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APortal::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	auto prop = PropertyChangedEvent.Property;
	auto name = prop->GetName();

	if (name == "Pair")
	{
		if (Pair == this)
		{
			Pair = Oldpair;
		}
		else if(Pair != Oldpair)
		{
			if (Pair) Pair->UpdatePair(this);
			if (Oldpair) Oldpair->UpdatePair(nullptr);

			if (Pair)
			{
				Connect();
			}
			else
			{
				Disconnect();
			}

			Oldpair = Pair;
		}
	}
	else if (name == "bPairing")
	{
		if (bPairing && Pair)
		{
			Connect();
			Pair->bPairing = true;
			if(Pair->Pair) Pair->Connect();
		}
		else if(Pair)
		{
			Disconnect();
			Pair->bPairing = false;
			Pair->Disconnect();
		}
	}
	else if (name == "PortalFX")
	{
		FXComponent->SetTemplate(PortalFX);
	}
}

void APortal::OnOverlapped_Teleport(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	auto main = UGameplayStatics::GetPlayerPawn(this, 0);
	if (OtherActor == main)
	{
		auto destination = Destination->GetComponentLocation();

		main->SetActorLocation(destination);
		main->GetController()->SetControlRotation(Destination->GetComponentRotation());
	}
}

void APortal::UpdatePair(APortal* newpair)
{
	if (newpair) // 새로운 pair로부터 pairing됨
	{
		if (!bPairing)
		{
			bPairing = true;
		}

		if (Pair)
		{
			Pair->UpdatePair(nullptr);
		}

		Pair = newpair;
		Oldpair = Pair;
		Connect();
	}
	else // 기존의 pair가 pairing을 끊음
	{
		Pair = nullptr;
		Oldpair = Pair;
		Disconnect();
	}
}

void APortal::Connect()
{
	//Destination->DetachFromComponent(FDetachmentTransformRules(EDetachmentRule::KeepWorld, true));
	Destination->AttachToComponent(nullptr, FAttachmentTransformRules(EAttachmentRule::KeepWorld, true));
	Destination->SetWorldLocation(Pair->GetActorLocation() + FVector(Pair->Volume->GetScaledBoxExtent().X + 100.0f, 0.0f, 0.0f));
	Destination->AttachToComponent(Pair->Volume, FAttachmentTransformRules(EAttachmentRule::KeepWorld, true));
}

void APortal::Disconnect()
{
	//Destination->DetachFromComponent(FDetachmentTransformRules(EDetachmentRule::KeepWorld, true));
	Destination->AttachToComponent(nullptr, FAttachmentTransformRules(EAttachmentRule::KeepWorld, true));
	Destination->SetWorldLocation(GetActorLocation() + FVector(Volume->GetScaledBoxExtent().X + 100.0f, 0.0f, 0.0f));
	Destination->AttachToComponent(Volume, FAttachmentTransformRules(EAttachmentRule::KeepWorld, true));
}