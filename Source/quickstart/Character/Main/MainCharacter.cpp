// Fill out your copyright notice in the Description page of Project Settings.
#include "MainCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "../../UI/QuestStatus.h"
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
	Camera->PostProcessSettings.AddBlendable(Helpers::C_LoadObjectFromPath<UMaterial>(TEXT("/Game/ShootingGame/Material/PostProcess/M_Highlight.M_Highlight")), 1);

	// Fire Sound Component
	FireAudio = CreateDefaultSubobject<UAudioComponent>(TEXT("Audio"));
	FireAudio->SetupAttachment(RootComponent);
	FireAudio->SetSound(Helpers::C_LoadObjectFromPath<USoundWave>(TEXT("/Game/StarterContent/Audio/Explosion01.Explosion01")));
	FireAudio->bAutoActivate = false;

	// Muzzle Pos
	Muzzle = FVector(110.0f, 20.0f, 45.0f);

	// Armory & Inventory 초기화
	Inventory.Add(FWrappedItemForm()); // Weapons
	Inventory.Add(FWrappedItemForm()); // Items
	Inventory.Add(FWrappedItemForm()); // Clothes

	Quickslots_Before = TArray<int>({ 0,0,0 });
	Quickslots_Now = TArray<int>({ 0,0,0 });
	Quickslots_Next = TArray<int>({ 0,0,0 });

	FItemForm fist = FItemForm(FItemShortForm(ETypeTag::Weapon, "Fist"));
	FItemForm noitem = FItemForm(FItemShortForm(ETypeTag::Item, "NoItem"));
	fist.ShortForm.InfoTag = "Fist";
	fist.ShortForm.Code = 0;
	fist.ShortForm.bIsSellable = false;
	noitem.ShortForm.InfoTag = " NoItem";
	noitem.ShortForm.bIsSellable = false;

	fist.Thumbnail_N = Helpers::C_LoadObjectFromPath<UTexture2D>(TEXT("/Game/ShootingGame/Image/WidgetImage/Normal/Fist_Normal.Fist_Normal"));
	fist.Thumbnail_H = Helpers::C_LoadObjectFromPath<UTexture2D>(TEXT("/Game/ShootingGame/Image/WidgetImage/Hovered/Fist_Hovered.Fist_Hovered"));
	fist.Thumbnail_S = Helpers::C_LoadObjectFromPath<UTexture2D>(TEXT("/Game/ShootingGame/Image/WidgetImage/Selected/Fist_Selected.Fist_Selected"));
	noitem.Thumbnail_N = Helpers::C_LoadObjectFromPath<UTexture2D>(TEXT("/Game/ShootingGame/Image/WidgetImage/Normal/NoItem_Normal.NoItem_Normal"));
	noitem.Thumbnail_H = Helpers::C_LoadObjectFromPath<UTexture2D>(TEXT("/Game/ShootingGame/Image/WidgetImage/Hovered/NoItem_Hovered.NoItem_Hovered"));
	noitem.Thumbnail_S = Helpers::C_LoadObjectFromPath<UTexture2D>(TEXT("/Game/ShootingGame/Image/WidgetImage/Selected/NoItem_Selected.NoItem_Selected"));
	Inventory[uint8(ETypeTag::Weapon)].ItemForms.Add(fist);
	Inventory[uint8(ETypeTag::Item)].ItemForms.Add(noitem);

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
	RunningSound = Helpers::C_LoadObjectFromPath<USoundCue>(TEXT("/Game/ShootingGame/Character/Main/Sounds/Cue/Footstep_Run.Footstep_Run"));
	WalkingSound = Helpers::C_LoadObjectFromPath<USoundCue>(TEXT("/Game/ShootingGame/Character/Main/Sounds/Cue/Footstep_Walk.Footstep_Walk"));
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
	GameMode = (AquickstartGameModeBase*)UGameplayStatics::GetGameMode(this);
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
	bQuickslotDirty = true;
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
	WorkingQuests.Add(&quest);
	for (int i = 0; i < quest.SubQuests.Num(); i++)
	{
		quest.SubQuests[i].Owner = &quest;
	}

	if (quest.Type == EQuestType::Serial)
	{
		quest.currPhase = 0;
		RegisterSubQuest(quest.SubQuests[quest.currPhase]);

		GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, HuntingQuests[0]->Name);
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
		ArrivalQuests.Add(&subquest);
		break;
	case ESingleQuestType::Hunt:
		subquest.currAmounts.SetNum(subquest.HuntAmounts.Num());
		HuntingQuests.Add(&subquest);
		break;
	case ESingleQuestType::Item:
		subquest.currAmounts.SetNum(subquest.ItemAmounts.Num());
		ItemQuests.Add(&subquest);
		break;
	case ESingleQuestType::Action:
		ActionQuests.Add(&subquest);
		break;
	default:
		break;
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
	TArray<int> DeleteList;

	for (int i = 0; i < HuntingQuests.Num(); i++)
	{
		for (int j = 0; j < HuntingQuests[i]->Huntees.Num(); j++)
		{
			if (HuntingQuests[i]->Huntees[j] == killclass)
			{
				HuntingQuests[i]->currAmounts[j]++;
				if (HuntingQuests[i]->CheckCompletion())
				{
					FQuest* Ownerquest = HuntingQuests[i]->Owner;
					if (Ownerquest->Type == EQuestType::Serial)
					{
						DeleteList.Add(i);
						Ownerquest->currPhase++;
						if (Ownerquest->currPhase == Ownerquest->SubQuests.Num())
						{
							Ownerquest->Progress = EQuestProgress::Finished;
						}
						else
						{
							RegisterSubQuest(Ownerquest->SubQuests[Ownerquest->currPhase]);
						}
					}
					else
					{
						Ownerquest->Remains--;
						if (Ownerquest->Remains <= 0)
						{
							Ownerquest->Progress = EQuestProgress::Finished;
						}
					}
				}
			}
		}
	}

	HuntingQuests.Sort();
	for (int i = 0; i < DeleteList.Num(); i++)
	{
		HuntingQuests.RemoveAt(i, false);
	}
}

void AMainCharacter::ReportItem(FString name, int num)
{
	for (int i = 0; i < ItemQuests.Num(); i++)
	{
		for (int j = 0; j < ItemQuests[i]->ItemNames.Num(); j++)
		{
			if (ItemQuests[i]->ItemNames[j] == name)
			{
				ItemQuests[i]->currAmounts[j] += num;
				if (ItemQuests[i]->CheckCompletion())
				{
					FQuest* Ownerquest = ItemQuests[i]->Owner;

					if (Ownerquest->Type == EQuestType::Serial)
					{
						ItemQuests.RemoveAt(i);
						Ownerquest->currPhase++;
						if (Ownerquest->currPhase == Ownerquest->SubQuests.Num())
						{
							Ownerquest->Progress = EQuestProgress::Finished;
						}
						else
						{
							RegisterSubQuest(Ownerquest->SubQuests[Ownerquest->currPhase]);
						}
					}
					else
					{
						Ownerquest->Remains--;
						if (Ownerquest->Remains <= 0)
						{
							Ownerquest->Progress = EQuestProgress::Finished;
						}
					}
				}
				else
				{
					///// 아이템을 잃었을 때 ===> 완료된 퀘스트를 취소해야 함 (따라서, serial quest에는 아이템 획득 퀘스트는 중간에 배치하지 않도록 함)
				}
			}
		}
	}
}

////////// kill, Item, Arrival, Action에 대해 event(Or tick)을 통해 퀘스트 완료 여부를 검사 --> 서브 퀘스트 완료시 퀘스트 업데이트