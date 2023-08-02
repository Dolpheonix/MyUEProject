// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../../Core/GameMode/MainGameMode.h"
#include "../Character_Root.h"
#include "../../Object/Item/Bullet.h"
#include "../../Object/Item/Throwable.h"
#include "../../Object/Volume/Beacon.h"
#include "../../Utils/NPCUtil.h"
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
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	// Get Team Id
	virtual FGenericTeamId GetGenericTeamId() const override;
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void LoadItemThumbnailAndMesh();

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
	UFUNCTION()
	void OpenQuestUI();
	UFUNCTION()
	void OpenMenu();

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
	void Register(FItemShortForm iteminfo);
	UFUNCTION()
	void DeleteItem(ETypeTag type, int index);

// Register Quest
	UFUNCTION()
	void RegisterQuest(FQuest& quest);
	UFUNCTION()
	void RegisterSubQuest(FSingleQuest& subquest);
	UFUNCTION()
	void EndQuest(FQuest& quest);
	UFUNCTION()
	void Notify();

// Condition
	UFUNCTION()
	bool CanAttack();

// Dead function
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
	virtual void OnHurt() override;
	virtual void OnDead() override;

// Quest checker
	void ReportKill(TSubclassOf<AActor> killclass);
	void ReportItem(FString itemname, int num);
	void ReportArrival(FSingleQuest* quest);
	ABeacon* RegisterDestinationFlagVolume(FSingleQuest* quest);
	UFUNCTION(BlueprintCallable)
	void ReportAction(int code);
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
// Rifle 관련
	UPROPERTY(EditDefaultsOnly, Category = Projectile)
	TSubclassOf<ABullet> ProjectileClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
	UAudioComponent* FireAudio;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = GamePlay)
	FVector Muzzle;

// Weapon 관련
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Weapons")
	UPhysicsConstraintComponent* WeaponJoint;
	UPROPERTY(BlueprintReadOnly)
	int32 WeaponCode=0;

// Item 관련
	UPROPERTY(BlueprintReadWrite)
	bool bQuickslotDirty=false;

// Cloth 관련

// Inventory
	UPROPERTY(BlueprintReadOnly)
	TArray<FWrappedItemForm> Inventory;
	UPROPERTY(BlueprintReadOnly)
	TArray<int> Quickslots_Before;
	UPROPERTY(BlueprintReadOnly)
	TArray<int> Quickslots_Now;
	UPROPERTY(BlueprintReadOnly)
	TArray<int> Quickslots_Next;

// Quest
	FWorkingQuestMemory QuestList;
	TArray<FReward> NotifyQueue;

// Respawn
	FVector StartPos;

// Helper Structure for Ladder Climbing
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FLadderInfo LadderInfo;

// Sound
	USoundCue* RunningSound;
	USoundCue* WalkingSound;
	UAudioComponent* FootstepAudioComponent;
	
// Profile
	int32 CurrMoney = 10000;
	int32 CurrLevel = 1;

// Game Mode
	UPROPERTY(BlueprintReadWrite)
	AMainGameMode* GameMode;
};
