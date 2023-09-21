#include "Interactable_LevelEvent.h"
#include "../../Utils/Helpers.h"

AInteractable_LevelEvent::AInteractable_LevelEvent()
{
	MainMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	Helpers::SetComponent<UStaticMeshComponent>(&MainMeshComponent, RootComponent, FVector::ZeroVector, FRotator::ZeroRotator);
}

void AInteractable_LevelEvent::PostEditChangeProperty(struct FPropertyChangedEvent& e)
{
	Super::PostEditChangeProperty(e);

	FString propname = e.Property->GetName();

	if (propname == TEXT("MainMesh"))
	{
		MainMeshComponent->SetStaticMesh(MainMesh);
	}
}

void AInteractable_LevelEvent::Interact()
{
	LevelEvent.Broadcast();	// ��ȣ�ۿ� ��, ���� �̺�Ʈ�� Broadcast
}
