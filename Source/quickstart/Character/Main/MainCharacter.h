// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../../quickstartGameModeBase.h"
#include "../Character_Root.h"
#include "../../Object/Item/Bullet.h"
#include "../../Object/Item/Throwable.h"
#include "../../Utils/MathUtil.h"
#include "../../Utils/Structs.h"
#include "../../Utils/Helpers.h"
#include "PhysicsEngine/PhysicsConstraintComponent.h"
#include "LadderInfo.h"
#include "Camera/CameraComponent.h"
#include "Components/AudioComponent.h"
#include "Sound/SoundCue.h"
#include "GenericTeamAgentInterface.h"
#include "MainCharacter.generated.h"

UENUM(BlueprintType)
enum class ECustomMovementMode : uint8
{
	IDLE,
	WALK,
	JUMP,
	FALL,
	MAX,
};

UENUM(BlueprintType)
enum class ECustomActionMode : uint8
{
	IDLE,
	ATTACK,
	INTERACT,
	GUARD,
	MAX,
};

UCLASS()
class QUICKSTART_API AMainCharacter : public ACharacter_Root, public IGenericTeamAgentInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMainCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	// Get Team Id
	virtual FGenericTeamId GetGenericTeamId() const override;
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

// Binding Functions
// Locomotive
	UFUNCTION()
	void MoveForward(float val);
	UFUNCTION()
	void MoveRight(float val);
	UFUNCTION()
	void MoveStart();
	UFUNCTION()
	void MoveEnd();
	UFUNCTION()
	void StartRun();
	UFUNCTION()
	void StopRun();
	UFUNCTION()
	void StartWalk();
	UFUNCTION()
	void StopWalk();
	UFUNCTION()
	void StartJump();
	UFUNCTION()
	void StopJump();
// Rotation
	UFUNCTION()
	void Turn(float val);
	UFUNCTION()
	void LookUp(float val);
// Actions
	UFUNCTION()
	void Interact();
	UFUNCTION()
	void Use();
	UFUNCTION()
	void Attack();
	UFUNCTION()
	void SubAttack();
	UFUNCTION()
	void unSubAttack();
// UI Interaction
	UFUNCTION()
	void RollItems();
	UFUNCTION()
	void RollWeapons();
	UFUNCTION(BlueprintImplementableEvent, Category="test")
	void OpenShowroom();

// Item Functions
	UFUNCTION()
	void NoItem();
	UFUNCTION()
	void ThrowApple();

// Attack Functions
	UFUNCTION()
	void Fist();
	UFUNCTION()
	void Fire();
	UFUNCTION()
	void Wield();
	UFUNCTION()
	void CheckEndMovement();
	UFUNCTION()
	void CheckEndAction();
	UFUNCTION()
	void EndAction();
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit);
	UFUNCTION()
	void OnOverlapped(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

// Inventory Functions
	UFUNCTION()
	void RefreshInventory(ETypeTag type);
	UFUNCTION()
	void unEquip();
	UFUNCTION()
	void Equip();
	UFUNCTION()
	void Register(ETypeTag type, FString name, FString info, int32 code);
	UFUNCTION()
	void DeleteItem(ETypeTag type, int index);

// Condition
	UFUNCTION()
	bool CanAttack();

// Dead function
	virtual void OnHurt() override;
	virtual void OnDead() override;
public:
// Third-person Camera
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UCameraComponent* Camera;

// Character State
	UPROPERTY(BlueprintReadOnly)
	bool bRunning=false;
	UPROPERTY(BlueprintReadOnly)
	bool bWalking=false;
	UPROPERTY(BlueprintReadOnly)
	int32 MoveKeyPressed = 0;
	UPROPERTY(BlueprintReadWrite)
	bool bContinueAttack = false;
	UPROPERTY(BlueprintReadWrite)
	bool bFalling = false;
	UPROPERTY(BlueprintReadWrite)
	int AttackPhase = -1;
	UPROPERTY(BlueprintReadWrite)
	bool bAttackBlocked = false;
	UPROPERTY(BlueprintReadWrite)
	bool bHurt = false;

private:
	UPROPERTY()
	ECustomMovementMode CurrentMovement = ECustomMovementMode::IDLE;
	UPROPERTY()
	ECustomActionMode CurrentAction = ECustomActionMode::IDLE;
public:
	UFUNCTION(BlueprintPure)
	ECustomMovementMode GetCurrentMovement();
	UFUNCTION(BlueprintCallable)
	void SetCurrentMovement(ECustomMovementMode NewMovement);
	UFUNCTION(BlueprintPure)
	ECustomActionMode GetCurrentAction();
	UFUNCTION(BlueprintCallable)
	void SetCurrentAction(ECustomActionMode NewAction);

public:
// Rifle ����
	UPROPERTY(EditDefaultsOnly, Category = Projectile)
	TSubclassOf<ABullet> ProjectileClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
	UAudioComponent* FireAudio;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = GamePlay)
	FVector Muzzle;

// Weapon ����
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Weapons")
	TArray<FItemForm> Weapons;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Weapons")
	UPhysicsConstraintComponent* WeaponJoint;
	UPROPERTY(BlueprintReadOnly)
	int Weapon_Before = 0;
	UPROPERTY(BlueprintReadOnly)
	int Weapon_Now=0;
	UPROPERTY(BlueprintReadOnly)
	int32 WeaponCode=0;
	UPROPERTY(BlueprintReadOnly)
	int Weapon_Next=0;

// Item ����
	UPROPERTY(BlueprintReadOnly)
	TArray<FItemForm> Items;
	UPROPERTY(BlueprintReadWrite)
	TArray<bool> Usable;
	UPROPERTY(BlueprintReadOnly)
	int Item_Now=0;
	UPROPERTY(BlueprintReadOnly)
	int Item_Next=0;
	UPROPERTY(BlueprintReadWrite)
	bool bQuickslotDirty=false;

// Cloth ����
	UPROPERTY(BlueprintReadOnly)
	TArray<FItemForm> Clothes;
	UPROPERTY(BlueprintReadOnly)
	int Cloth_Now=0;
	UPROPERTY(BlueprintReadOnly)
	int Cloth_Next=0;

// Respawn
	FVector StartPos;

// Helper Structure for Ladder Climbing
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FLadderInfo LadderInfo;

// Sound
	USoundCue* RunningSound;
	USoundCue* WalkingSound;
	UAudioComponent* FootstepAudioComponent;
	

// Game Mode
	UPROPERTY(BlueprintReadWrite)
	AquickstartGameModeBase* GameMode;
};
