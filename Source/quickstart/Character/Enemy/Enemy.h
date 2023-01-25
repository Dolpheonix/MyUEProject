// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Character_Root.h"
#include "Components/WidgetComponent.h"
#include "../../UI/HPBar.h"
#include "Enemy.generated.h"

/**
 * 
 */
UCLASS()
class QUICKSTART_API AEnemy : public ACharacter_Root
{
	GENERATED_BODY()

public:
	AEnemy();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void Attack(int elpasedFrame);

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Widget")
	UWidgetComponent* HPWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widget")
	TSubclassOf<UHPBar> HPWidgetClass;
};
