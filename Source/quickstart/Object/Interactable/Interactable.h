// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../../Character/Main/MainCharacter.h"
#include "../../Interface/InteractionInterface.h"
#include "Blueprint/UserWidget.h"
#include "Interactable.generated.h"

UCLASS(Abstract)
class QUICKSTART_API AInteractable : public AActor, public IInteractionInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AInteractable();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;

protected:
	AMainCharacter* Player;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Interact")
	FVector InteractPoint;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interact")
	FVector InteractForward;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Interact")
	float InteractRadius;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interact")
	float InteractRange;

	UArrowComponent* InteractionForwardArrow;
	UArrowComponent* InteractionRangeArrow_R;
	UArrowComponent* InteractionRangeArrow_L;

	UUserWidget* NotifyPopup;

	bool bInteractable = false;

	UPROPERTY(BlueprintReadWrite)
	bool bActive = true;
	bool bWait = false;
};
