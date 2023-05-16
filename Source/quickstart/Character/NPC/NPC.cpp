// Fill out your copyright notice in the Description page of Project Settings.


#include "NPC.h"
#include "Components/CapsuleComponent.h"
#include "../../quickstartGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "../../UI/DialogueBox.h"
#include "../../UI/Shop.h"
#include "../../Utils/Helpers.h"

ANPC::ANPC()
{
	PrimaryActorTick.bCanEverTick = true;

	auto Capsule = Cast<UCapsuleComponent>(RootComponent);
	Capsule->SetCollisionProfileName("Enemy");

	auto MainMesh = GetMesh();
	MainMesh->SetSkeletalMesh(Helpers::C_LoadObjectFromPath<USkeletalMesh>(TEXT("/Game/ShootingGame/Character/Main/Mesh/SK_Mannequin.SK_Mannequin")));
	Helpers::SetComponent<USkeletalMeshComponent>(&MainMesh, RootComponent, FVector(0.f, 0.f, -88.f), FRotator(0.0f, -90.0f, 0.f));
	MainMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	MainMesh->SetCollisionProfileName("NoCollision");
	MainMesh->SetGenerateOverlapEvents(true);

	SelfCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("SelfCam"));
	Helpers::SetComponent(&SelfCamera, RootComponent, FVector(0, 0, 0), FRotator(0, 0, 0));
}

void ANPC::BeginPlay()
{
	Super::BeginPlay();

	Player = Cast<AMainCharacter>(UGameplayStatics::GetPlayerPawn(this, 0));
}

void ANPC::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!bInteracted && FVector::Dist(GetActorLocation(), Player->GetActorLocation()) < 200.0f)
	{
		bInteractable = true;
		if (Player->GetCurrentAction() == ECustomActionMode::INTERACT)
		{
			Interact();
		}
	}
	else
	{
		bInteractable = false;
	}
}

void ANPC::OnHurt()
{

}

void ANPC::OnDead()
{

}

void ANPC::Interact()
{
	bInteracted = true;

	GetWorld()->GetFirstPlayerController()->SetViewTargetWithBlend(this, 1.0f);

	AquickstartGameModeBase* gamemode = Cast<AquickstartGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));

	gamemode->ChangeMenuWidget(gamemode->DialogueBoxUI);

	Cast<UDialogueBox>(gamemode->DialogueBoxUI)->InitDialogue(DialogueTree, this);
}

void ANPC::UnInteract()
{
	GetWorld()->GetFirstPlayerController()->SetViewTargetWithBlend(Player, 1.0f);
	bInteracted = false;
	Player->GameMode->ChangeMenuWidget(Player->GameMode->MainUI);
}

void ANPC::OpenShop()
{
	AquickstartGameModeBase* gamemode = Cast<AquickstartGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));

	gamemode->ChangeMenuWidget(gamemode->ShopUI);

	Cast<UShop>(gamemode->ShopUI)->InitShop(this);
}