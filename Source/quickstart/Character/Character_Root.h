// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Character_Root.generated.h"

UCLASS()
class QUICKSTART_API ACharacter_Root : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACharacter_Root();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:

	UPROPERTY(BlueprintReadWrite, Category = "Status")
	float HP = 100.0f;

	UPROPERTY(BlueprintReadWrite, Category = "Interaction")
	bool bHurt = false;

	UPROPERTY(BlueprintReadWrite, Category = "Attack")
	bool bAttacking = false;

	// hurt event�� �߻��ϸ� �����ð��� �ʿ��ϹǷ�, hurt animation�� ���ӽð���ŭ ������ �ش�.
	int hurtFrameStep = -1;
};
