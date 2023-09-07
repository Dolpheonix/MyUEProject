// Fill out your copyright notice in the Description page of Project Settings.


#include "Destroyable.h"

ADestroyable::ADestroyable()
{
	//RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	MeshComponent->SetupAttachment(RootComponent);
}

void ADestroyable::BeginPlay()
{
	Super::BeginPlay();
}

void ADestroyable::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bInteractable)
	{
		MeshComponent->SetRenderCustomDepth(true);
		MeshComponent->SetCustomDepthStencilValue(0);
	}
	else
	{
		MeshComponent->SetRenderCustomDepth(false);
	}
}

void ADestroyable::Interact()
{
	Destroy();
}

