#include "Destroyable.h"

ADestroyable::ADestroyable()
{
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

