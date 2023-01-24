// Fill out your copyright notice in the Description page of Project Settings.


#include "Lever.h"

ALever::ALever()
{
	//RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	LeverCase = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LeverCase"));
	LeverCase->SetStaticMesh(Helpers::C_LoadObjectFromPath<UStaticMesh>(TEXT("/Game/ShootingGame/Asset/Lever/Lever_Case.Lever_Case")));
	LeverCase->SetupAttachment(RootComponent);

	LeverBody = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LeverBody"));
	LeverBody->SetStaticMesh(Helpers::C_LoadObjectFromPath<UStaticMesh>(TEXT("/Game/ShootingGame/Asset/Lever/Lever_Body.Lever_Body")));
	Helpers::SetComponent(&LeverBody, RootComponent, FVector(10.0f, 0.0f, 10.0f), FRotator(0.0f, 0.0f, 0.0f), FVector(1.5f, 1.0f, 1.0f));
}

void ALever::BeginPlay()
{
	Super::BeginPlay();

	LeverBody->SetRelativeRotation(FRotator(40.0f, 0.0f, 0.0f));
	Forward = GetActorForwardVector();
}

void ALever::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bLeverDown)
	{
		if (LeverBody->GetRelativeRotation().Pitch <= -40.0f)
		{
			bLeverDown = false;
		}
		else
		{
			LeverBody->AddRelativeRotation(FRotator(-1.0f, 0.0f, 0.0f));
		}
	}
	else if (bWait)
	{
		if (LeverBody->GetRelativeRotation().Pitch >= 40.0f)
		{
			bWait = false;
		}
		else
		{
			LeverBody->AddRelativeRotation(FRotator(1.0f, 0.0f, 0.0f));
		}
	}

	if (bInteractable)
	{
		LeverBody->SetRenderCustomDepth(true);
		LeverCase->SetRenderCustomDepth(true);
		LeverBody->SetCustomDepthStencilValue(0);
		LeverCase->SetCustomDepthStencilValue(0);
	}
	else
	{
		LeverBody->SetRenderCustomDepth(false);
		LeverCase->SetRenderCustomDepth(false);
	}
}

void ALever::Interact()
{
	bLeverDown = true;
	bWait = true;
	Lever_On();
}