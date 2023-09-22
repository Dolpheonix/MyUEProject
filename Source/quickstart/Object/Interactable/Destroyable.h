#pragma once

#include "CoreMinimal.h"
#include "Interactable.h"
#include "Destroyable.generated.h"

UCLASS()
class QUICKSTART_API ADestroyable : public AInteractable
{
	GENERATED_BODY()
	
public:
	ADestroyable();

protected:
	virtual void BeginPlay() override;
	virtual void Interact() override;

public:
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Mesh")
	UStaticMeshComponent* MeshComponent;
};
