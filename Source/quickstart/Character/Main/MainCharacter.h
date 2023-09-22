// Character - Main Character
// 게임의 메인 플레이어 캐릭터
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

// 캐릭터의 이동 모드
UENUM(BlueprintType)
enum class ECustomMovementMode : uint8
{
	IDLE,	// 가만히 있는 상태
	WALK,	// WASD 키를 사용해 이동하는 상태
	JUMP,	// 점프 상태
	FALL,	// 점프 외에 공중에 떠있는 상태
	MAX,
};

// 캐릭터의 액션 모드
UENUM(BlueprintType)
enum class ECustomActionMode : uint8
{
	IDLE,		// 가만히 있는 상태
	ATTACK,		// 공격 상태
	INTERACT,	// 상호작용 상태
	GUARD,		// 가드 상태
	MAX,		
};

// Main Character의 애니메이션 재생 시간
struct AnimationAssetPhases
{
public:
	static constexpr float Fist = 1.0f;
	static constexpr float Fire = 1.1f;
	static constexpr float Wield[4] = { 1.0f, 2.0f, 3.0f, 4.8f };
	static constexpr float Interact = 0.8f;
	static constexpr float Jump = 0.9f;
};

// 캐릭터가 사다리를 탈 때 전달받을 사다리의 정보
USTRUCT(Atomic, BlueprintType)
struct FLadderInfo
{
	GENERATED_BODY()
	// 현재 사다리에 타고 있는가?
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool onLadder;
	// 사다리의 경사
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Slope;
	// 사다리의 너비
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Width;
	// onLadder 값의 dirty flag
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
	// 팀 ID를 반환 (Perception Component의 Affiliation filter에서 사용)
	virtual FGenericTeamId GetGenericTeamId() const override;
	// 입력 바인딩
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// 아이템의 썸네일과 메시를 로드
	void LoadItemThumbnailAndMesh();

// 바인딩 함수
	// 앞뒤로 이동
	UFUNCTION()
	void MoveForward(float val);
	// 좌우로 이동
	UFUNCTION()
	void MoveRight(float val);
	// WASD 키를 눌러 이동을 시작
	UFUNCTION()
	void MoveStart();
	// WASD 키를 떼 이동을 종료
	UFUNCTION()
	void MoveEnd();
	// 달리기를 시작
	UFUNCTION()
	void StartRun();
	// 달리기를 종료
	UFUNCTION()
	void StopRun();
	// 걷기를 시작
	UFUNCTION()
	void StartWalk();
	// 걷기를 종료
	UFUNCTION()
	void StopWalk();
	// 점프를 시작
	UFUNCTION()
	void StartJump();
	// 점프를 종료
	UFUNCTION()
	void StopJump();
	// 카메라를 좌우로 회전
	UFUNCTION()
	void Turn(float val);
	// 카메라를 위아래로 회전
	UFUNCTION()
	void LookUp(float val);
	// 오브젝트와 상호작용
	UFUNCTION()
	void Interact();
	// 아이템을 사용
	UFUNCTION()
	void Use();
	// 무기로 공격
	UFUNCTION()
	void Attack();
	// 보조공격 시작
	UFUNCTION()
	void SubAttack();
	// 보조공격을 종료
	UFUNCTION()
	void unSubAttack();
	// 아이템을 전환
	UFUNCTION()
	void RollItems();
	// 무기를 전환
	UFUNCTION()
	void RollWeapons();
	// 인벤토리를 엶
	UFUNCTION()
	void OpenShowroom();
	// 퀘스트 상태창을 엶
	UFUNCTION()
	void OpenQuestUI();
	// 메뉴창을 엶
	UFUNCTION()
	void OpenMenu();

	// 아이템 사용 함수
	UFUNCTION()
	void NoItem();
	UFUNCTION()
	void ThrowApple();
	UFUNCTION()
	void ThrowSeed();

	// 공격 함수
	UFUNCTION()
	void Fist();
	UFUNCTION()
	void Fire();
	UFUNCTION()
	void Wield();

	// Custom Movement의 전환 여부 체크
	UFUNCTION()
	void CheckEndMovement();
	// Custom Action의 전환 여부 체크
	UFUNCTION()
	void CheckEndAction();
	// 사다리에 타고 있는지 아닌지 체크
	UFUNCTION()
	void CheckClimb();
	// 공격을 할 수 있는 상태인가?
	UFUNCTION()
	bool CanAttack();
	// 현재 Acton을 중단하고 Idle 상태로 돌아감
	UFUNCTION()
	void EndAction();
	// 무기 메시의 히트 판정 (스태틱 오브젝트와의 충돌)
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit);
	// 무기 메시의 오버랩 판정 (적 오브젝트와의 충돌)
	UFUNCTION()
	void OnOverlapped(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	// 인벤토리를 업데이트
	UFUNCTION()
	void RefreshInventory(ETypeTag type);
	// 장착중인 무기를 장착 해제
	UFUNCTION()
	void unEquip();
	// 무기 장착
	UFUNCTION()
	void Equip();
	// 획득한 아이템을 인벤토리에 등록
	UFUNCTION()
	void Register(FItemShortForm iteminfo);
	// 아이템을 인벤토리에서 제거
	UFUNCTION()
	void DeleteItem(ETypeTag type, int index);

	// 퀘스트를 플레이어에게 등록
	UFUNCTION()
	void RegisterQuest(FQuest& quest);
	// 서브퀘스트를 플레이어에게 등록
	UFUNCTION()
	void RegisterSubQuest(FSingleQuest& subquest);
	// 퀘스트를 종료
	UFUNCTION()
	void EndQuest(FQuest& quest);

	// Notify Queue의 알림 사항을 플레이어에게 알림
	UFUNCTION()
	void Notify();

	// 데미지를 받음
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
	// Hurt 상태일 때 호출
	virtual void OnHurt() override;
	// 사망 상태일 때 호출
	virtual void OnDead() override;

	// 처치한 Enemy의 라벨을 받아, 처치 퀘스트의 진행도 체크 및 업데이트
	void ReportKill(TArray<FString> labels);
	// 획득한 아이템의 정보를 받아, 획득 퀘스트의 진행도 체크 및 업데이트
	void ReportItem(FString itemname, int num);
	// 도착 퀘스트의 완료를 알림
	void ReportArrival(FSingleQuest* quest);
	// 도착 퀘스트 등록 시, 해당 위치에 트리거 볼륨 생성 및 완료 함수 등록
	ABeacon* RegisterDestinationFlagVolume(FSingleQuest* quest);
	// 액션 퀘스트의 완료를 알림
	UFUNCTION(BlueprintCallable)
	void ReportAction(int code);

public:
	// 3인칭 카메라
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UCameraComponent* Camera;

// 캐릭터의 상태
	// 뛰는중인가?
	UPROPERTY(BlueprintReadOnly)
	bool bRunning=false;			
	// 걷는중인가?
	UPROPERTY(BlueprintReadOnly)
	bool bWalking=false;
	// 떨어지는 상태인가?
	UPROPERTY(BlueprintReadWrite)
	bool bFalling = false;
	// 데미지를 입은 상태인가?
	UPROPERTY(BlueprintReadWrite)
	bool bHurt = false;
	// 현재 눌린 이동키의 수 (WASD)
	UPROPERTY(BlueprintReadOnly)
	int32 MoveKeyPressed = 0;
	// (다단 공격 시) 다음 단계로 진행할 수 있는 상태인가?
	UPROPERTY(BlueprintReadWrite)
	bool bContinueAttack = false;	
	// (다단 공격 시) 현재 공격 단계
	UPROPERTY(BlueprintReadWrite)
	int AttackPhase = -1;
	// 공격이 막혔는가?
	UPROPERTY(BlueprintReadWrite)
	bool bAttackBlocked = false;
	// 상호작용 중인가?
	UPROPERTY(BlueprintReadWrite)
	bool bInteracting = false;
	// 현재 상호작용이 가능한 오브젝트의 수
	UPROPERTY(BlueprintReadWrite)
	int32 InteractionFlag = 0;

private:
	// 현재 Movement
	UPROPERTY()
	ECustomMovementMode CurrentMovement = ECustomMovementMode::IDLE;
	// 현재 Action
	UPROPERTY()
	ECustomActionMode CurrentAction = ECustomActionMode::IDLE;
public:
	// 현재 Movement를 반환
	UFUNCTION(BlueprintPure)
	ECustomMovementMode GetCurrentMovement();
	// Movement를 변경
	UFUNCTION(BlueprintCallable)
	void SetCurrentMovement(ECustomMovementMode NewMovement);
	// 현재 Action을 반환
	UFUNCTION(BlueprintPure)
	ECustomActionMode GetCurrentAction();
	// Action을 변경
	UFUNCTION(BlueprintCallable)
	void SetCurrentAction(ECustomActionMode NewAction);

public:
// Rifle 무기 관련
	// 총알 발사체의 클래스
	UPROPERTY(EditDefaultsOnly, Category = Projectile)
	TSubclassOf<ABullet> ProjectileClass;
	// 발사음 Audio Component
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
	UAudioComponent* FireAudio;

// Sword 무기 관련
	// 무기의 Physics을 켤때, 손과 무기를 묶어줌
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Weapons")
	UPhysicsConstraintComponent* WeaponJoint;

	// 무기의 코드 번호
	UPROPERTY(BlueprintReadOnly)
	int32 WeaponCode=0;

	// 인벤토리
	UPROPERTY(BlueprintReadOnly)
	TArray<FWrappedItemForm> Inventory;
	// 이전 퀵슬롯 (Cloth, Weapon, Item 순)
	UPROPERTY(BlueprintReadOnly)
	TArray<int> Quickslots_Before;
	// 현재 퀵슬롯 (Cloth, Weapon, Item 순)
	UPROPERTY(BlueprintReadOnly)
	TArray<int> Quickslots_Now;
	// 다음 퀵슬롯 (Cloth, Weapon, Item 순)
	UPROPERTY(BlueprintReadOnly)
	TArray<int> Quickslots_Next;
	// 퀵슬롯의 dirty flag
	UPROPERTY(BlueprintReadWrite)
	bool bQuickslotDirty = false;

	// 현재 진행중인 퀘스트
	FWorkingQuestMemory QuestList;
	
	// 리워드 알림 큐
	TArray<FReward> NotifyQueue;

	// 캐릭터의 시작 위치
	FVector StartPos;

	// 사다리에 올라탔을 때, 사다리 관련 정보를 담는 구조체
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FLadderInfo LadderInfo;

	// 달리기 Sound Cue
	USoundCue* RunningSound;
	// 걷기 Sound Cue
	USoundCue* WalkingSound;
	// 발자국 소리 Audio Component
	UAudioComponent* FootstepAudioComponent;
	
	// 현재 돈
	int32 CurrMoney = 10000;
	// 현재 레벨
	int32 CurrLevel = 1;

	// 게임 모드
	UPROPERTY(BlueprintReadWrite)
	AMainGameMode* GameMode;
};
