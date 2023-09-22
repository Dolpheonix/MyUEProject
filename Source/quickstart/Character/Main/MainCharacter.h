// Character - Main Character
// ������ ���� �÷��̾� ĳ����
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
#include "Camera/CameraComponent.h"
#include "Components/AudioComponent.h"
#include "Sound/SoundCue.h"
#include "GenericTeamAgentInterface.h"
#include "MainCharacter.generated.h"

// ĳ������ �̵� ���
UENUM(BlueprintType)
enum class ECustomMovementMode : uint8
{
	IDLE,	// ������ �ִ� ����
	WALK,	// WASD Ű�� ����� �̵��ϴ� ����
	JUMP,	// ���� ����
	FALL,	// ���� �ܿ� ���߿� ���ִ� ����
	MAX,
};

// ĳ������ �׼� ���
UENUM(BlueprintType)
enum class ECustomActionMode : uint8
{
	IDLE,		// ������ �ִ� ����
	ATTACK,		// ���� ����
	INTERACT,	// ��ȣ�ۿ� ����
	GUARD,		// ���� ����
	MAX,		
};

// Main Character�� �ִϸ��̼� ��� �ð�
struct AnimationAssetPhases
{
public:
	static constexpr float Fist = 1.0f;
	static constexpr float Fire = 1.1f;
	static constexpr float Wield[4] = { 1.0f, 2.0f, 3.0f, 4.8f };
	static constexpr float Interact = 0.8f;
	static constexpr float Jump = 0.9f;
};

// ĳ���Ͱ� ��ٸ��� Ż �� ���޹��� ��ٸ��� ����
USTRUCT(Atomic, BlueprintType)
struct FLadderInfo
{
	GENERATED_BODY()
	// ���� ��ٸ��� Ÿ�� �ִ°�?
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool onLadder;
	// ��ٸ��� ���
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Slope;
	// ��ٸ��� �ʺ�
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Width;
	// onLadder ���� dirty flag
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bDirty;
};

UCLASS()
class QUICKSTART_API AMainCharacter : public ACharacter_Root, public IGenericTeamAgentInterface
{
	GENERATED_BODY()

public:
	AMainCharacter();

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:	
	virtual void Tick(float DeltaTime) override;
	// �� ID�� ��ȯ (Perception Component�� Affiliation filter���� ���)
	virtual FGenericTeamId GetGenericTeamId() const override;
	// �Է� ���ε�
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// �������� ����ϰ� �޽ø� �ε�
	void LoadItemThumbnailAndMesh();

// ���ε� �Լ�
	// �յڷ� �̵�
	UFUNCTION()
	void MoveForward(float val);
	// �¿�� �̵�
	UFUNCTION()
	void MoveRight(float val);
	// WASD Ű�� ���� �̵��� ����
	UFUNCTION()
	void MoveStart();
	// WASD Ű�� �� �̵��� ����
	UFUNCTION()
	void MoveEnd();
	// �޸��⸦ ����
	UFUNCTION()
	void StartRun();
	// �޸��⸦ ����
	UFUNCTION()
	void StopRun();
	// �ȱ⸦ ����
	UFUNCTION()
	void StartWalk();
	// �ȱ⸦ ����
	UFUNCTION()
	void StopWalk();
	// ������ ����
	UFUNCTION()
	void StartJump();
	// ������ ����
	UFUNCTION()
	void StopJump();
	// ī�޶� �¿�� ȸ��
	UFUNCTION()
	void Turn(float val);
	// ī�޶� ���Ʒ��� ȸ��
	UFUNCTION()
	void LookUp(float val);
	// ������Ʈ�� ��ȣ�ۿ�
	UFUNCTION()
	void Interact();
	// �������� ���
	UFUNCTION()
	void Use();
	// ����� ����
	UFUNCTION()
	void Attack();
	// �������� ����
	UFUNCTION()
	void SubAttack();
	// ���������� ����
	UFUNCTION()
	void unSubAttack();
	// �������� ��ȯ
	UFUNCTION()
	void RollItems();
	// ���⸦ ��ȯ
	UFUNCTION()
	void RollWeapons();
	// �κ��丮�� ��
	UFUNCTION()
	void OpenShowroom();
	// ����Ʈ ����â�� ��
	UFUNCTION()
	void OpenQuestUI();
	// �޴�â�� ��
	UFUNCTION()
	void OpenMenu();

	// ������ ��� �Լ�
	UFUNCTION()
	void NoItem();
	UFUNCTION()
	void ThrowApple();
	UFUNCTION()
	void ThrowSeed();

	// ���� �Լ�
	UFUNCTION()
	void Fist();
	UFUNCTION()
	void Fire();
	UFUNCTION()
	void Wield();

	// Custom Movement�� ��ȯ ���� üũ
	UFUNCTION()
	void CheckEndMovement();
	// Custom Action�� ��ȯ ���� üũ
	UFUNCTION()
	void CheckEndAction();
	// ��ٸ��� Ÿ�� �ִ��� �ƴ��� üũ
	UFUNCTION()
	void CheckClimb();
	// ������ �� �� �ִ� �����ΰ�?
	UFUNCTION()
	bool CanAttack();
	// ���� Acton�� �ߴ��ϰ� Idle ���·� ���ư�
	UFUNCTION()
	void EndAction();
	// ���� �޽��� ��Ʈ ���� (����ƽ ������Ʈ���� �浹)
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit);
	// ���� �޽��� ������ ���� (�� ������Ʈ���� �浹)
	UFUNCTION()
	void OnOverlapped(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	// �κ��丮�� ������Ʈ
	UFUNCTION()
	void RefreshInventory(ETypeTag type);
	// �������� ���⸦ ���� ����
	UFUNCTION()
	void unEquip();
	// ���� ����
	UFUNCTION()
	void Equip();
	// ȹ���� �������� �κ��丮�� ���
	UFUNCTION()
	void Register(FItemShortForm iteminfo);
	// �������� �κ��丮���� ����
	UFUNCTION()
	void DeleteItem(ETypeTag type, int index);

	// ����Ʈ�� �÷��̾�� ���
	UFUNCTION()
	void RegisterQuest(FQuest& quest);
	// ��������Ʈ�� �÷��̾�� ���
	UFUNCTION()
	void RegisterSubQuest(FSingleQuest& subquest);
	// ����Ʈ�� ����
	UFUNCTION()
	void EndQuest(FQuest& quest);

	// Notify Queue�� �˸� ������ �÷��̾�� �˸�
	UFUNCTION()
	void Notify();

	// �������� ����
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
	// Hurt ������ �� ȣ��
	virtual void OnHurt() override;
	// ��� ������ �� ȣ��
	virtual void OnDead() override;

	// óġ�� Enemy�� ���� �޾�, óġ ����Ʈ�� ���൵ üũ �� ������Ʈ
	void ReportKill(TArray<FString> labels);
	// ȹ���� �������� ������ �޾�, ȹ�� ����Ʈ�� ���൵ üũ �� ������Ʈ
	void ReportItem(FString itemname, int num);
	// ���� ����Ʈ�� �ϷḦ �˸�
	void ReportArrival(FSingleQuest* quest);
	// ���� ����Ʈ ��� ��, �ش� ��ġ�� Ʈ���� ���� ���� �� �Ϸ� �Լ� ���
	ABeacon* RegisterDestinationFlagVolume(FSingleQuest* quest);
	// �׼� ����Ʈ�� �ϷḦ �˸�
	UFUNCTION(BlueprintCallable)
	void ReportAction(int code);

public:
	// 3��Ī ī�޶�
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UCameraComponent* Camera;

// ĳ������ ����
	// �ٴ����ΰ�?
	UPROPERTY(BlueprintReadOnly)
	bool bRunning=false;			
	// �ȴ����ΰ�?
	UPROPERTY(BlueprintReadOnly)
	bool bWalking=false;
	// �������� �����ΰ�?
	UPROPERTY(BlueprintReadWrite)
	bool bFalling = false;
	// �������� ���� �����ΰ�?
	UPROPERTY(BlueprintReadWrite)
	bool bHurt = false;
	// ���� ���� �̵�Ű�� �� (WASD)
	UPROPERTY(BlueprintReadOnly)
	int32 MoveKeyPressed = 0;
	// (�ٴ� ���� ��) ���� �ܰ�� ������ �� �ִ� �����ΰ�?
	UPROPERTY(BlueprintReadWrite)
	bool bContinueAttack = false;	
	// (�ٴ� ���� ��) ���� ���� �ܰ�
	UPROPERTY(BlueprintReadWrite)
	int AttackPhase = -1;
	// ������ �����°�?
	UPROPERTY(BlueprintReadWrite)
	bool bAttackBlocked = false;
	// ��ȣ�ۿ� ���ΰ�?
	UPROPERTY(BlueprintReadWrite)
	bool bInteracting = false;
	// ���� ��ȣ�ۿ��� ������ ������Ʈ�� ��
	UPROPERTY(BlueprintReadWrite)
	int32 InteractionFlag = 0;

private:
	// ���� Movement
	UPROPERTY()
	ECustomMovementMode CurrentMovement = ECustomMovementMode::IDLE;
	// ���� Action
	UPROPERTY()
	ECustomActionMode CurrentAction = ECustomActionMode::IDLE;
public:
	// ���� Movement�� ��ȯ
	UFUNCTION(BlueprintPure)
	ECustomMovementMode GetCurrentMovement();
	// Movement�� ����
	UFUNCTION(BlueprintCallable)
	void SetCurrentMovement(ECustomMovementMode NewMovement);
	// ���� Action�� ��ȯ
	UFUNCTION(BlueprintPure)
	ECustomActionMode GetCurrentAction();
	// Action�� ����
	UFUNCTION(BlueprintCallable)
	void SetCurrentAction(ECustomActionMode NewAction);

public:
// Rifle ���� ����
	// �Ѿ� �߻�ü�� Ŭ����
	UPROPERTY(EditDefaultsOnly, Category = Projectile)
	TSubclassOf<ABullet> ProjectileClass;
	// �߻��� Audio Component
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
	UAudioComponent* FireAudio;

// Sword ���� ����
	// ������ Physics�� �Ӷ�, �հ� ���⸦ ������
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Weapons")
	UPhysicsConstraintComponent* WeaponJoint;

	// ������ �ڵ� ��ȣ
	UPROPERTY(BlueprintReadOnly)
	int32 WeaponCode=0;

	// �κ��丮
	UPROPERTY(BlueprintReadOnly)
	TArray<FWrappedItemForm> Inventory;
	// ���� ������ (Cloth, Weapon, Item ��)
	UPROPERTY(BlueprintReadOnly)
	TArray<int> Quickslots_Before;
	// ���� ������ (Cloth, Weapon, Item ��)
	UPROPERTY(BlueprintReadOnly)
	TArray<int> Quickslots_Now;
	// ���� ������ (Cloth, Weapon, Item ��)
	UPROPERTY(BlueprintReadOnly)
	TArray<int> Quickslots_Next;
	// �������� dirty flag
	UPROPERTY(BlueprintReadWrite)
	bool bQuickslotDirty = false;

	// ���� �������� ����Ʈ
	FWorkingQuestMemory QuestList;
	
	// ������ �˸� ť
	TArray<FReward> NotifyQueue;

	// ĳ������ ���� ��ġ
	FVector StartPos;

	// ��ٸ��� �ö����� ��, ��ٸ� ���� ������ ��� ����ü
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FLadderInfo LadderInfo;

	// �޸��� Sound Cue
	USoundCue* RunningSound;
	// �ȱ� Sound Cue
	USoundCue* WalkingSound;
	// ���ڱ� �Ҹ� Audio Component
	UAudioComponent* FootstepAudioComponent;
	
	// ���� ��
	int32 CurrMoney = 10000;
	// ���� ����
	int32 CurrLevel = 1;

	// ���� ���
	UPROPERTY(BlueprintReadWrite)
	AMainGameMode* GameMode;
};
