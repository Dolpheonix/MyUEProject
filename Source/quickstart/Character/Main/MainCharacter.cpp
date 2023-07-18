// Fill out your copyright notice in the Description page of Project Settings.
#include "MainCharacter.h"
#include "../../quickstart.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "../../UI/QuestStatus.h"
#include "../../UI/Notify.h"
#include "Blueprint/WidgetTree.h"
#include "Components/CanvasPanelSlot.h"
#include "../../UI/MainWidget.h"
#include "../../Core/GameInstance/MainGameInstance.h"
#include <Kismet/GameplayStatics.h>

AMainCharacter::AMainCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	// Capsule Component
	auto Capsule = Cast<UCapsuleComponent>(RootComponent);
	Capsule->SetCollisionProfileName("Character");

	// Character Mesh
	auto MainMesh = GetMesh();
	MainMesh->SetSkeletalMesh(Helpers::C_LoadObjectFromPath<USkeletalMesh>(TEXT("/Game/ShootingGame/Character/Main/Mesh/SK_Mannequin.SK_Mannequin")));
	Helpers::SetComponent<USkeletalMeshComponent>(&MainMesh, RootComponent, FVector(0.f, 0.f, -88.f), FRotator(0.0f, -90.0f, 0.f));
	MainMesh->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	MainMesh->SetRenderCustomDepth(true);
	MainMesh->SetCustomDepthStencilValue(1);
	MainMesh->SetGenerateOverlapEvents(true);
	MainMesh->SetCollisionProfileName("CharacterBody");

	// Camera Component
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	Helpers::SetComponent(&Camera, RootComponent, FVector(-172.0f, 0.0f, 85.0f), FRotator(-20.0f, 0.0f, 0.0f));
	Camera->bUsePawnControlRotation = true;
	Camera->PostProcessSettings.AddBlendable(Helpers::C_LoadObjectFromPath<UMaterial>(TEXT("/Game/ShootingGame/Asset/Material/PostProcess/M_Highlight.M_Highlight")), 1);

	// Fire Sound Component
	FireAudio = CreateDefaultSubobject<UAudioComponent>(TEXT("Audio"));
	FireAudio->SetupAttachment(RootComponent);
	FireAudio->SetSound(Helpers::C_LoadObjectFromPath<USoundCue>(TEXT("/Game/ShootingGame/Audio/SoundEffect/SoundCue/Explosion_01_Cue.Explosion_01_Cue")));
	FireAudio->bAutoActivate = false;

	// Muzzle Pos
	Muzzle = FVector(110.0f, 20.0f, 45.0f);

	// Weapon's Physics Constraint
	WeaponJoint = CreateDefaultSubobject<UPhysicsConstraintComponent>(TEXT("WeaponJoint"));
	WeaponJoint->SetLinearXLimit(ELinearConstraintMotion::LCM_Locked, 0.0f);
	WeaponJoint->SetLinearYLimit(ELinearConstraintMotion::LCM_Locked, 0.0f);
	WeaponJoint->SetLinearZLimit(ELinearConstraintMotion::LCM_Locked, 0.0f);
	WeaponJoint->SetAngularSwing1Limit(EAngularConstraintMotion::ACM_Locked, 0.0f);
	WeaponJoint->SetAngularSwing2Limit(EAngularConstraintMotion::ACM_Locked, 0.0f);
	WeaponJoint->SetAngularTwistLimit(EAngularConstraintMotion::ACM_Locked, 0.0f);
	WeaponJoint->SetupAttachment(RootComponent);
	WeaponJoint->SetRelativeLocation(FVector(100.0f, 0.0f, 0.0f));

	// Footstep Sound
	RunningSound = Helpers::C_LoadObjectFromPath<USoundCue>(TEXT("/Game/ShootingGame/Character/Main/Audio/SoundCue/Footstep_Run.Footstep_Run"));
	WalkingSound = Helpers::C_LoadObjectFromPath<USoundCue>(TEXT("/Game/ShootingGame/Character/Main/Audio/SoundCue/Footstep_Walk.Footstep_Walk"));
	FootstepAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("Footstep Audio"));
	FootstepAudioComponent->SetupAttachment(RootComponent);
}

void AMainCharacter::BeginPlay()
{
	Super::BeginPlay();

	// Inventory와 Armory를 화면에 그려야 함
	bQuickslotDirty = true;

	StartPos = GetActorLocation();

	// Get Game Mode
	GameMode = (AMainGameMode*)UGameplayStatics::GetGameMode(this);

	UMainGameInstance* GI = Cast<UMainGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (GI)
	{
		GI->LoadToCharacter(this);
	}

	Cast<UMainWidget>(GameMode->MainUI)->RefreshHPBar();
	Cast<UMainWidget>(GameMode->MainUI)->RefreshQuickslot();
}

void AMainCharacter::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	UMainGameInstance* GI = Cast<UMainGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (GI)
	{
		GI->SaveFromCharacter(this);
	}
	Super::EndPlay(EndPlayReason);
}

void AMainCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (GetCharacterMovement()->MovementMode == EMovementMode::MOVE_Falling) bFalling = true;
	else bFalling = false;

	// Hurt 경직 효과
	if (bHurt && hurtFrameStep < 0)
	{
		hurtFrameStep = 0;
		Camera->PostProcessSettings.bOverride_SceneColorTint = true;
		DisableInput(UGameplayStatics::GetPlayerController(this, 0));
	}
	if (hurtFrameStep >= 0)
	{
		// Red Filter ON
		if (hurtFrameStep < 35)
		{
			float hurtIntensity = 1.5f * FMath::Sin(hurtFrameStep * (PI /30.0f)) + 1.0f; // 1.0f ~ 2.5f ~ 1.0f (0s ~ 3s)
			Camera->PostProcessSettings.SceneColorTint = FLinearColor(hurtIntensity, 1.0f, 1.0f);
			hurtFrameStep++;
		}
		// Red Filter OFF
		else if (hurtFrameStep >= 35)
		{
			Camera->PostProcessSettings.SceneColorTint = FLinearColor(1.0f, 1.0f, 1.0f);
			Camera->PostProcessSettings.bOverride_SceneColorTint = false;
			hurtFrameStep = -1;
			bHurt = false;
			EnableInput(UGameplayStatics::GetPlayerController(this, 0));
		}
	}

	// Attack Phase
	CheckEndMovement();
	CheckEndAction();
}

FGenericTeamId AMainCharacter::GetGenericTeamId() const
{
	return FGenericTeamId(1);
}

void AMainCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("Move_Forward", this, &AMainCharacter::MoveForward);                       // W,S
	PlayerInputComponent->BindAction("Move_W", IE_Pressed, this, &AMainCharacter::MoveStart);
	PlayerInputComponent->BindAction("Move_A", IE_Pressed, this, &AMainCharacter::MoveStart);
	PlayerInputComponent->BindAction("Move_S", IE_Pressed, this, &AMainCharacter::MoveStart);
	PlayerInputComponent->BindAction("Move_D", IE_Pressed, this, &AMainCharacter::MoveStart);
	PlayerInputComponent->BindAction("Move_End", IE_Released, this, &AMainCharacter::MoveEnd);

	PlayerInputComponent->BindAction("Run", IE_Pressed, this, &AMainCharacter::StartRun);                     // left Alt
	PlayerInputComponent->BindAction("Run", IE_Released, this, &AMainCharacter::StopRun);                   
	PlayerInputComponent->BindAction("Walk", IE_Pressed, this, &AMainCharacter::StartWalk);                   // left Shift
	PlayerInputComponent->BindAction("Walk", IE_Released, this, &AMainCharacter::StopWalk);

	PlayerInputComponent->BindAxis("Move_Right", this, &AMainCharacter::MoveRight);                           // D,A
	PlayerInputComponent->BindAxis("Turn", this, &AMainCharacter::Turn);                                      // Mouse LR
	PlayerInputComponent->BindAxis("LookUp", this, &AMainCharacter::LookUp);                                  // Mouse FB
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AMainCharacter::StartJump);                   // Spacebar press
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &AMainCharacter::StopJump);                   // Spacebar release
	PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &AMainCharacter::Interact);                // E press

	PlayerInputComponent->BindAction("RollItems", IE_Pressed, this, &AMainCharacter::RollItems);			  // R
	PlayerInputComponent->BindAction("RollWeapons", IE_Pressed, this, &AMainCharacter::RollWeapons);          // T
	PlayerInputComponent->BindAction("Use", IE_Pressed, this, &AMainCharacter::Use);                          // Q
	PlayerInputComponent->BindAction("Attack", IE_Pressed, this, &AMainCharacter::Attack);                    // Mouse Left
	PlayerInputComponent->BindAction("SubAttack", IE_Pressed, this, &AMainCharacter::SubAttack);                      // Mouse Right
	PlayerInputComponent->BindAction("SubAttack", IE_Released, this, &AMainCharacter::unSubAttack);

	PlayerInputComponent->BindAction("OpenShowroom", IE_Pressed, this, &AMainCharacter::OpenShowroom);        // I
	PlayerInputComponent->BindAction("OpenQuestUI", IE_Pressed, this, &AMainCharacter::OpenQuestUI);
}

void AMainCharacter::MoveForward(float val)
{
	// 사다리에 매달렸을 경우, Forward Axis를 사다리 방향으로 조정한다
	if (LadderInfo.onLadder)
	{
		FVector direction = GetActorForwardVector().RotateAngleAxis(-LadderInfo.Slope, GetActorRightVector());
		AddMovementInput(direction, val);
	}
	else if(GetCurrentAction() == ECustomActionMode::IDLE)
	{
		FVector direction = GetActorForwardVector();
		AddMovementInput(direction, val);
	}
}

void AMainCharacter::MoveStart()
{
	MoveKeyPressed++;
}

void AMainCharacter::MoveEnd()
{
	if (LadderInfo.onLadder)
	{
		GetCharacterMovement()->StopMovementImmediately();
	}
	else 
	{
		MoveKeyPressed--;
	}
}

void AMainCharacter::StartRun()
{
	bRunning = true;
	GetCharacterMovement()->MaxWalkSpeed = 1200.0f;
	FootstepAudioComponent->SetPitchMultiplier(1.7f);
}

void AMainCharacter::StopRun()
{
	bRunning = false;
	GetCharacterMovement()->MaxWalkSpeed = 600.0f;
	FootstepAudioComponent->SetPitchMultiplier(1.5f);
}

void AMainCharacter::StartWalk()
{
	bWalking = true;
	GetCharacterMovement()->MaxWalkSpeed = 300.0f;
	FootstepAudioComponent->SetPitchMultiplier(0.7f);
}

void AMainCharacter::StopWalk()
{
	bWalking = false;
	GetCharacterMovement()->MaxWalkSpeed = 600.0f;
	FootstepAudioComponent->SetPitchMultiplier(1.5f);
}

void AMainCharacter::MoveRight(float val)
{
	if (!LadderInfo.onLadder && GetCurrentAction() == ECustomActionMode::IDLE)
	{
		FVector direction = GetActorRightVector();
		AddMovementInput(direction, val);
	}
}

void AMainCharacter::Turn(float val)
{
	if (!LadderInfo.onLadder && GetCurrentAction() == ECustomActionMode::IDLE)
	{
		AddControllerYawInput(val);
	}
}

void AMainCharacter::LookUp(float val)
{
	AddControllerPitchInput(val);
}

void AMainCharacter::StartJump()
{
	if (LadderInfo.onLadder)
	{
		LadderInfo.onLadder = false;
		LadderInfo.dirty = true;
		GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
	}
	else if (GetCurrentAction() == ECustomActionMode::IDLE && !bFalling && !bForced)
	{
		bPressedJump = true;
		SetCurrentMovement(ECustomMovementMode::JUMP);
		FootstepAudioComponent->Stop();
	}
}

void AMainCharacter::StopJump()
{
	bPressedJump = false;
}

void AMainCharacter::Interact()
{
	if (GetCurrentMovement() == ECustomMovementMode::IDLE && GetCurrentAction() == ECustomActionMode::IDLE)
	{
		SetCurrentAction(ECustomActionMode::INTERACT);
	}
}

void AMainCharacter::RollItems()
{
	if (Inventory[(uint8)(ETypeTag::Item)].ItemForms.Num() > 0 && GetCurrentAction() == ECustomActionMode::IDLE)
	{
		Quickslots_Now[(uint8)(ETypeTag::Item)] = Quickslots_Next[(uint8)(ETypeTag::Item)];
		Quickslots_Next[(uint8)(ETypeTag::Item)] = MathUtil::CircularPlus(Quickslots_Now[(uint8)(ETypeTag::Item)], Inventory[(uint8)(ETypeTag::Item)].ItemForms.Num());
		bQuickslotDirty = true;
	}
}

void AMainCharacter::RollWeapons()
{
	if (Inventory[(uint8)(ETypeTag::Weapon)].ItemForms.Num() > 0 && GetCurrentAction() == ECustomActionMode::IDLE)
	{
		Quickslots_Before[(uint8)(ETypeTag::Weapon)] = Quickslots_Now[(uint8)(ETypeTag::Weapon)];
		Quickslots_Now[(uint8)(ETypeTag::Weapon)] = Quickslots_Next[(uint8)(ETypeTag::Weapon)];
		Quickslots_Next[(uint8)(ETypeTag::Weapon)] = MathUtil::CircularPlus(Quickslots_Now[(uint8)(ETypeTag::Weapon)], Inventory[(uint8)(ETypeTag::Weapon)].ItemForms.Num());
		unEquip();
		Equip();
		bQuickslotDirty = true;
	}
}

void AMainCharacter::RefreshInventory(ETypeTag type)
{
	Quickslots_Next[(uint8)type] = MathUtil::CircularPlus(Quickslots_Now[(uint8)type], Inventory[(uint8)type].ItemForms.Num());
	Cast<UMainWidget>(GameMode->MainUI)->RefreshQuickslot();
}

void AMainCharacter::Register(FItemShortForm iteminfo)
{
	FString name = iteminfo.NameTag;
	int index = Inventory[(uint8)iteminfo.TypeTag].ItemForms.IndexOfByPredicate([name](const FItemForm& itemform) {return itemform.ShortForm.NameTag == name; });

	if (index >= 0)
	{
		Inventory[(uint8)iteminfo.TypeTag].ItemForms[index].ShortForm.Num += iteminfo.Num;
	}
	else
	{
		FItemForm registerform(iteminfo);

		registerform.Thumbnail_N = Helpers::LoadObjectFromPath<UTexture2D>(*Helpers::GetNormalThumbnailFromName(iteminfo.NameTag));
		registerform.Thumbnail_H = Helpers::LoadObjectFromPath<UTexture2D>(*Helpers::GetHoveredThumbnailFromName(iteminfo.NameTag));
		registerform.Thumbnail_S = Helpers::LoadObjectFromPath<UTexture2D>(*Helpers::GetSelectedThumbnailFromName(iteminfo.NameTag));
		if (iteminfo.TypeTag != ETypeTag::Item)
		{
			registerform.MeshComponent = NewObject<UStaticMeshComponent>(this, UStaticMeshComponent::StaticClass(), FName(iteminfo.NameTag + "Mesh"));
			registerform.MeshComponent->SetStaticMesh(Helpers::LoadObjectFromPath<UStaticMesh>(*Helpers::GetMeshFromName(iteminfo.NameTag)));
			FAttachmentTransformRules rule = { EAttachmentRule::SnapToTarget, true };
			registerform.MeshComponent->AttachToComponent(GetMesh(), rule, FName(iteminfo.NameTag + "_unEquip"));
			registerform.MeshComponent->RegisterComponent();
		}
		Inventory[(uint8)(iteminfo.TypeTag)].ItemForms.Add(registerform);
		if (iteminfo.TypeTag == ETypeTag::Weapon)
		{
			Inventory[(uint8)(ETypeTag::Weapon)].ItemForms.Last().MeshComponent->OnComponentHit.AddDynamic(this, &AMainCharacter::OnHit);
			Inventory[(uint8)(ETypeTag::Weapon)].ItemForms.Last().MeshComponent->OnComponentBeginOverlap.AddDynamic(this, &AMainCharacter::OnOverlapped);
		}
	}
	ReportItem(name, iteminfo.Num);
	RefreshInventory(iteminfo.TypeTag);
}

void AMainCharacter::DeleteItem(ETypeTag type, int index)
{
	if (index > 0)
	{
		if (type == ETypeTag::Weapon)
		{
			Inventory[(uint8)type].ItemForms[index].MeshComponent->DestroyComponent();
		}
		Inventory[(uint8)type].ItemForms.RemoveAt(index);
		Quickslots_Now[(uint8)type] = Quickslots_Now[(uint8)type] >= Inventory[(uint8)type].ItemForms.Num() ? Inventory[(uint8)type].ItemForms.Num() - 1 : Quickslots_Now[(uint8)type];
		if (type == ETypeTag::Weapon)
		{
			Equip();
		}
	}

	RefreshInventory(type);
	bQuickslotDirty = true;
}

void AMainCharacter::unEquip()
{
	FItemForm* toTakeOff = &Inventory[(uint8)ETypeTag::Weapon].ItemForms[Quickslots_Before[(uint8)ETypeTag::Weapon]];
	FString Before = toTakeOff->ShortForm.NameTag;
	if (Before == "Fist")
	{

	}
	else
	{
		// Constraint between back & weapon + physics & Collision settings off
		toTakeOff->MeshComponent->AttachToComponent(GetMesh(), { EAttachmentRule::SnapToTarget, true }, FName(Before + "_unEquip"));
		toTakeOff->MeshComponent->SetSimulatePhysics(false);
		toTakeOff->MeshComponent->SetGenerateOverlapEvents(false);
		toTakeOff->MeshComponent->SetNotifyRigidBodyCollision(false);
	}
}

void AMainCharacter::Equip()
{
	FItemForm* toTakeOn = &Inventory[(uint8)ETypeTag::Weapon].ItemForms[Quickslots_Now[(uint8)ETypeTag::Weapon]];
	FString Curr = toTakeOn->ShortForm.NameTag;
	if (Curr == "Fist")
	{

	}
	else
	{
		// Constraint between hand & weapon + physics & Collision Settings ON
		toTakeOn->MeshComponent->AttachToComponent(GetMesh(), { EAttachmentRule::SnapToTarget, true }, FName(Curr + "_Equip"));
		WeaponJoint->SetConstrainedComponents(GetMesh(), FName("hand_r"), toTakeOn->MeshComponent, FName(""));
		toTakeOn->MeshComponent->SetSimulatePhysics(true);
		toTakeOn->MeshComponent->SetEnableGravity(false);
		toTakeOn->MeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		toTakeOn->MeshComponent->SetCollisionProfileName("Weapon");
		toTakeOn->MeshComponent->SetGenerateOverlapEvents(true);
		toTakeOn->MeshComponent->SetNotifyRigidBodyCollision(true);
	}
	WeaponCode = toTakeOn->ShortForm.Code;
}

void AMainCharacter::Use()
{
	FItemForm* toUse = &Inventory[(uint8)ETypeTag::Item].ItemForms[Quickslots_Now[(uint8)ETypeTag::Item]];
	if (GetCurrentAction() == ECustomActionMode::IDLE)
	{
		FString Curr = toUse->ShortForm.NameTag;

		if (Curr == "No Item")
		{
			NoItem();
		}
		else if (Curr == "Apple")
		{
			ThrowApple();
		}
	}
}

void AMainCharacter::Attack()
{
	FItemForm* toUse = &Inventory[(uint8)ETypeTag::Weapon].ItemForms[Quickslots_Now[(uint8)ETypeTag::Weapon]];
	if (WeaponCode != 0 && !toUse->MeshComponent->IsSimulatingPhysics())
	{
		toUse->MeshComponent->SetSimulatePhysics(true);
	}

	if (CanAttack())
	{
		switch (WeaponCode)
		{
		case 0:
			Fist();
			break;
		case 1:
			Fire();
			break;
		case 2:
			Wield();
			break;
		default:
			break;
		}
	}
}

void AMainCharacter::SubAttack()
{
	switch (WeaponCode)
	{
	case 0:
		if (GetCurrentAction() == ECustomActionMode::IDLE && GetCurrentMovement() != ECustomMovementMode::JUMP && !bFalling)
		{
			SetCurrentAction(ECustomActionMode::GUARD);
		}
		break;
	case 1:
	{
		Camera->SetRelativeLocation(FVector(-62.0f, -15.0f, 95.0f), true);
		GameMode->SniperMode(true);
		break;
	}
	case 2:
		if (GetCurrentAction() == ECustomActionMode::IDLE && GetCurrentMovement() != ECustomMovementMode::JUMP && !bFalling)
		{
			SetCurrentAction(ECustomActionMode::GUARD);
		}
		break;
	default:
		break;
	}

}

void AMainCharacter::unSubAttack()
{
	switch (WeaponCode)
	{
	case 0:
		SetCurrentAction(ECustomActionMode::IDLE);
		break;
	case 1:
		Camera->SetRelativeLocationAndRotation(FVector(-172.0f, 0.0f, 85.0f), FRotator(-20.0f, 0.0f, 0.0f), true);
		GameMode->SniperMode(false);
		break;
	case 2:
		SetCurrentAction(ECustomActionMode::IDLE);
		break;
	default:
		break;
	}
}

void AMainCharacter::RegisterQuest(FQuest& quest)
{
	quest.Progress = EQuestProgress::InProgress;
	WorkingQuests.Add(&quest);
	for (int i = 0; i < quest.SubQuests.Num(); i++)
	{
		FSingleQuest* subquest = &quest.SubQuests[i];
		subquest->Owner = &quest;
		switch (subquest->Type)
		{
		case ESingleQuestType::Hunt:
			subquest->currAmounts.SetNum(subquest->HuntingLists.Num());
			break;
		case ESingleQuestType::Item:
			subquest->currAmounts.SetNum(subquest->ItemLists.Num());
			for (int j = 0; j < subquest->ItemLists.Num(); j++)
			{
				FString itemname = subquest->ItemLists[j].ItemName;
				auto preexist = Inventory[(uint8)subquest->ItemLists[j].ItemType].ItemForms.FindByPredicate([itemname](const FItemForm& item) {return itemname == item.ShortForm.NameTag; });
				if (preexist)
				{
					subquest->currAmounts[j] = preexist->ShortForm.Num;
				}
			}
			break;
		default:
			break;
		}
	}

	if (quest.Type == EQuestType::Serial)
	{
		quest.currPhase = 0;
		RegisterSubQuest(quest.SubQuests[quest.currPhase]);
	}
	else
	{
		quest.Remains = quest.SubQuests.Num();
		for (int i = 0; i < quest.SubQuests.Num(); i++)
		{
			RegisterSubQuest(quest.SubQuests[i]);
		}
	}
}

void AMainCharacter::RegisterSubQuest(FSingleQuest& subquest)
{
	switch (subquest.Type)
	{
	case ESingleQuestType::Arrival:
	{
		ArrivalQuests.Add(&subquest);
		RegisterDestinationFlagVolume(&subquest);
		break;
	}
	case ESingleQuestType::Hunt:
		HuntingQuests.Add(&subquest);
		break;
	case ESingleQuestType::Item:
		ItemQuests.Add(&subquest);
		break;
	case ESingleQuestType::Action:
		ActionQuests.Add(&subquest);
		break;
	default:
		break;
	}
}

void AMainCharacter::EndQuest(FQuest& quest)
{
	quest.Progress = EQuestProgress::AlreadyDone;

	FQuest* qptr = &quest;
	int questindex = WorkingQuests.IndexOfByPredicate([qptr](const FQuest* ptr) {return qptr == ptr; });

	if (questindex < 0)
	{
		UE_LOG(ErrQuest, Log, TEXT("Quest that want to remove does not exist"));
	}
	else
	{
		for (int i = 0; i < qptr->SubQuests.Num(); i++)
		{
			FSingleQuest* sptr = &qptr->SubQuests[i];
			TArray<FSingleQuest*>* arr = nullptr;
			switch (sptr->Type)
			{
			case ESingleQuestType::Arrival:
				arr = &ArrivalQuests;
				break;
			case ESingleQuestType::Item:
				arr = &ItemQuests;
				break;
			case ESingleQuestType::Hunt:
				arr = &HuntingQuests;
				break;
			case ESingleQuestType::Action:
				arr = &ActionQuests;
				break;
			}

			if (!arr)
			{
				UE_LOG(ErrQuest, Log, TEXT("Quest type error"));
			}
			else
			{
				int subquestindex = arr->IndexOfByPredicate([sptr](const FSingleQuest* ptr) {return sptr == ptr; });
				if (subquestindex < 0)
				{
					UE_LOG(ErrQuest, Log, TEXT("Single Quest that want to remove does not exist"));
				}
				else
				{
					arr->RemoveAt(subquestindex);
				}
			}
		}

		for (FReward reward : qptr->Rewards)
		{
			switch (reward.Type)
			{
			case ERewardType::EXP:
				// TODO : EXP 시스템
				break;
			case ERewardType::ITEM:
				Register(reward.Item);
				break;
			case ERewardType::MONEY:
				CurrMoney += reward.Money;
				break;
			default:
				break;
			}

			NotifyQueue.Add(reward);
		}

		WorkingQuests.RemoveAt(questindex);
	}
}

void AMainCharacter::OpenQuestUI()
{
	GameMode->ChangeMenuWidget(GameMode->QuestUI);
	Cast<UQuestStatus>(GameMode->QuestUI)->InitQuestUI(this);
}

ECustomMovementMode AMainCharacter::GetCurrentMovement()
{
	return CurrentMovement;
}

void AMainCharacter::SetCurrentMovement(ECustomMovementMode NewMovement)
{
	CurrentMovement = NewMovement;
}

ECustomActionMode AMainCharacter::GetCurrentAction()
{
	return CurrentAction;
}

void AMainCharacter::SetCurrentAction(ECustomActionMode NewAction)
{
	CurrentAction = NewAction;
}

bool AMainCharacter::CanAttack()
{
	return (GetCurrentAction() == ECustomActionMode::IDLE || GetCurrentAction() == ECustomActionMode::ATTACK) && (!bFalling || GetCurrentMovement() == ECustomMovementMode::JUMP);
}

void AMainCharacter::OnHurt()
{
	bHurt = true;
}

void AMainCharacter::OnDead()
{
	GetMesh()->PlayAnimation(Helpers::LoadObjectFromPath<UAnimSequence>("/Game/ShootingGame/Character/Main/Animations/FistAnim/Anim_Fist_Death.Anim_Fist_Death"), false);

	FTimerHandle destroyhandle;
	GetWorld()->GetTimerManager().SetTimer(destroyhandle, [this]() { 
		this->SetActorLocation(StartPos);
		this->GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
		this->GetMesh()->SetAnimClass(Helpers::LoadObjectFromPath<UAnimBlueprintGeneratedClass>("/Game/ShootingGame/Character/Main/Animations/BP_Animation.BP_Animation_C"));
		this->HP = 100.0f;
		this->bDead = false;
		}, 0.5f, false, 2.0f);
}

void AMainCharacter::ReportKill(TSubclassOf<AActor> killclass)
{
	for (int i = 0; i < HuntingQuests.Num(); i++)
	{
		int idx = HuntingQuests[i]->HuntingLists.IndexOfByPredicate([killclass](const FHuntingQuestForm& item) {return killclass == item.Huntee; });

		if (idx >= 0)
		{
			HuntingQuests[i]->currAmounts[idx]++;

			if (!HuntingQuests[i]->Completed)
			{
				if (HuntingQuests[i]->CheckCompletion())
				{
					FQuest* Ownerquest = HuntingQuests[i]->Owner;

					bool finished = Ownerquest->EndSingleTask();

					if (!finished && Ownerquest->Type == EQuestType::Serial)
					{
						RegisterSubQuest(Ownerquest->SubQuests[Ownerquest->currPhase]);
					}
				}
			}
		}
	}
}

void AMainCharacter::ReportItem(FString name, int num)
{
	for (int i = 0; i < ItemQuests.Num(); i++)
	{
		int idx = ItemQuests[i]->ItemLists.IndexOfByPredicate([name](const FItemQuestForm& item) {return name == item.ItemName; });
		if (idx >= 0)
		{
			ItemQuests[i]->currAmounts[idx] += num;

			if (num > 0 && !ItemQuests[i]->Completed)	// item을 얻은 경우이므로, 이전에 다 모았던 item quest는 건드릴 필요 없음
			{
				if (ItemQuests[i]->CheckCompletion())
				{
					FQuest* Ownerquest = ItemQuests[i]->Owner;

					bool finished = Ownerquest->EndSingleTask();

					if (!finished && Ownerquest->Type == EQuestType::Serial)
					{
						RegisterSubQuest(Ownerquest->SubQuests[Ownerquest->currPhase]);
					}
				}
			}
			else if (num < 0 && ItemQuests[i]->Completed) // item을 잃은 경우 ==> 기존에 Complete 처리한 SubQuest를 다시 검사
			{
				if (!ItemQuests[i]->CheckCompletion())
				{
					FQuest* Ownerquest = ItemQuests[i]->Owner;

					Ownerquest->UndoSingleTask();
				}
			}
		}
	}
}

void AMainCharacter::ReportArrival(FSingleQuest* quest)
{
	quest->Completed = true;
	FQuest* Ownerquest = quest->Owner;

	if (!Ownerquest->EndSingleTask() && Ownerquest->Type == EQuestType::Serial)
	{
		RegisterSubQuest(Ownerquest->SubQuests[Ownerquest->currPhase]);
	}
}

ABeacon* AMainCharacter::RegisterDestinationFlagVolume(FSingleQuest* quest)
{
	FActorSpawnParameters param;
	param.Owner = this;
	param.Instigator = GetInstigator();

	ABeacon* beacon = GetWorld()->SpawnActor<ABeacon>(ABeacon::StaticClass(), quest->Destination, FRotator::ZeroRotator, param);
	beacon->SetHidden(false);
	beacon->SetActorEnableCollision(true);
	beacon->RegisterAllComponents();
	beacon->RegisterQuest(quest);

	return beacon;
}

void AMainCharacter::ReportAction(int code)
{
	for (auto quest : ActionQuests)
	{
		if (quest->ActionCode == code && !quest->Completed)
		{
			quest->Completed = true;
			FQuest* Ownerquest = quest->Owner;
			if (!Ownerquest->EndSingleTask() && Ownerquest->Type == EQuestType::Serial)
			{
				RegisterSubQuest(Ownerquest->SubQuests[Ownerquest->currPhase]);
			}
		}
	}
}

void AMainCharacter::Notify()
{
	if (NotifyQueue.Num() > 0)
	{
		UNotifyAcquire* NotifyPopup = GameMode->CurrentUI->WidgetTree->ConstructWidget<UNotifyAcquire>(GameMode->NotifyWidgetClass, TEXT("Notify"));
		if (NotifyPopup)
		{
			Cast<UCanvasPanel>(GameMode->CurrentUI->GetRootWidget())->AddChild(NotifyPopup);
			Cast<UCanvasPanelSlot>(NotifyPopup->Slot)->SetAnchors(FAnchors(0.5f, 0.5f, 0.5f, 0.5f));
			NotifyPopup->InitializeNotifications(NotifyQueue);
		}
	}
}

float AMainCharacter::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser)
{
	Cast<UMainWidget>(GameMode->MainUI)->RefreshHPBar();

	return Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
}