// Fill out your copyright notice in the Description page of Project Settings.
#include "MainCharacter.h"
#include "../../quickstart.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "../../UI/QuestTable.h"
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
		GI->ApplyCharacterMemory(this);
		LoadItemThumbnailAndMesh();
	}
	else
	{
		FItemForm fist = FItemForm(FItemShortForm(ETypeTag::Weapon, "Fist"));
		FItemForm noitem = FItemForm(FItemShortForm(ETypeTag::Item, "NoItem"));
		FItemForm nocloth = FItemForm(FItemShortForm(ETypeTag::Cloth, "NoCloth"));
		fist.ShortForm.InfoTag = "Fist";
		fist.ShortForm.Code = 0;
		fist.ShortForm.bIsSellable = false;
		noitem.ShortForm.InfoTag = " NoItem";
		noitem.ShortForm.bIsSellable = false;
		nocloth.ShortForm.InfoTag = "NoCloth";
		nocloth.ShortForm.bIsSellable = false;

		fist.Thumbnail_N = Helpers::LoadObjectFromPath<UTexture2D>(TEXT("/Game/ShootingGame/Image/WidgetImage/Normal/Fist_Normal.Fist_Normal"));
		fist.Thumbnail_H = Helpers::LoadObjectFromPath<UTexture2D>(TEXT("/Game/ShootingGame/Image/WidgetImage/Hovered/Fist_Hovered.Fist_Hovered"));
		fist.Thumbnail_S = Helpers::LoadObjectFromPath<UTexture2D>(TEXT("/Game/ShootingGame/Image/WidgetImage/Selected/Fist_Selected.Fist_Selected"));
		noitem.Thumbnail_N = Helpers::LoadObjectFromPath<UTexture2D>(TEXT("/Game/ShootingGame/Image/WidgetImage/Normal/NoItem_Normal.NoItem_Normal"));
		noitem.Thumbnail_H = Helpers::LoadObjectFromPath<UTexture2D>(TEXT("/Game/ShootingGame/Image/WidgetImage/Hovered/NoItem_Hovered.NoItem_Hovered"));
		noitem.Thumbnail_S = Helpers::LoadObjectFromPath<UTexture2D>(TEXT("/Game/ShootingGame/Image/WidgetImage/Selected/NoItem_Selected.NoItem_Selected"));
		nocloth.Thumbnail_N = Helpers::LoadObjectFromPath<UTexture2D>(TEXT("/Game/ShootingGame/Image/WidgetImage/Normal/NoCloth_Normal.NoCloth_Normal"));
		nocloth.Thumbnail_H = Helpers::LoadObjectFromPath<UTexture2D>(TEXT("/Game/ShootingGame/Image/WidgetImage/Hovered/NoCloth_Hovered.NoCloth_Hovered"));
		nocloth.Thumbnail_S = Helpers::LoadObjectFromPath<UTexture2D>(TEXT("/Game/ShootingGame/Image/WidgetImage/Selected/NoCloth_Selected.NoCloth_Selected"));

		Inventory.Add(FWrappedItemForm()); // Weapons
		Inventory.Add(FWrappedItemForm()); // Items
		Inventory.Add(FWrappedItemForm()); // Clothes
		Inventory[uint8(ETypeTag::Weapon)].ItemForms.Add(fist);
		Inventory[uint8(ETypeTag::Item)].ItemForms.Add(noitem);
		Inventory[uint8(ETypeTag::Cloth)].ItemForms.Add(nocloth);

		Quickslots_Before = TArray<int>({ 0,0,0 });
		Quickslots_Now = TArray<int>({ 0,0,0 });
		Quickslots_Next = TArray<int>({ 0,0,0 });
		LoadItemThumbnailAndMesh();
	}

	Cast<UMainWidget>(GameMode->MainUI)->RefreshHPBar();
	Cast<UMainWidget>(GameMode->MainUI)->RefreshQuickslot();
}

void AMainCharacter::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

void AMainCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (GetCharacterMovement()->MovementMode == EMovementMode::MOVE_Falling) bFalling = true;
	else bFalling = false;

	// Hurt 경직 효과
	if (bHurt && HurtTimer < 0)
	{
		HurtTimer = 0.0f;
		Camera->PostProcessSettings.bOverride_SceneColorTint = true;
	}
	if (HurtTimer >= 0)
	{
		// Red Filter ON
		if (HurtTimer < 1.0f)
		{
			float hurtIntensity = 1.5f * FMath::Sin(HurtTimer * PI) + 1.0f; // 1.0f ~ 2.5f ~ 1.0f (0s ~ 3s)
			Camera->PostProcessSettings.SceneColorTint = FLinearColor(hurtIntensity, 1.0f, 1.0f);
			HurtTimer += DeltaTime;
		}
		// Red Filter OFF
		else if (HurtTimer >= 1.0f)
		{
			Camera->PostProcessSettings.SceneColorTint = FLinearColor(1.0f, 1.0f, 1.0f);
			Camera->PostProcessSettings.bOverride_SceneColorTint = false;
			HurtTimer = -1.f;
			bHurt = false;
		}
	}

	if (!bInteracting && InteractionFlag > 0)
	{
		Cast<UMainWidget>(GameMode->MainUI)->NotifyInteractImage->SetVisibility(ESlateVisibility::Visible);
		Cast<UMainWidget>(GameMode->MainUI)->NotifyInteractTextBlock->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		Cast<UMainWidget>(GameMode->MainUI)->NotifyInteractImage->SetVisibility(ESlateVisibility::Hidden);
		Cast<UMainWidget>(GameMode->MainUI)->NotifyInteractTextBlock->SetVisibility(ESlateVisibility::Hidden);
	}

	// Attack Phase
	if(!bHurt) CheckEndMovement();
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
	PlayerInputComponent->BindAction("Use", IE_Pressed, this, &AMainCharacter::Use);                          // F
	PlayerInputComponent->BindAction("Attack", IE_Pressed, this, &AMainCharacter::Attack);                    // Mouse Left
	PlayerInputComponent->BindAction("SubAttack", IE_Pressed, this, &AMainCharacter::SubAttack);              // Mouse Right
	PlayerInputComponent->BindAction("SubAttack", IE_Released, this, &AMainCharacter::unSubAttack);

	PlayerInputComponent->BindAction("OpenShowroom", IE_Pressed, this, &AMainCharacter::OpenShowroom);        // I
	PlayerInputComponent->BindAction("OpenQuestUI", IE_Pressed, this, &AMainCharacter::OpenQuestUI);		  // Q
	PlayerInputComponent->BindAction("OpenMenu", IE_Pressed, this, &AMainCharacter::OpenMenu);				  // ESC
}

void AMainCharacter::LoadItemThumbnailAndMesh()
{
	for (int i = 0; i < Inventory[(uint8)ETypeTag::Cloth].ItemForms.Num(); i++)
	{
		FItemShortForm iteminfo = Inventory[(uint8)ETypeTag::Cloth].ItemForms[i].ShortForm;
		Inventory[(uint8)ETypeTag::Cloth].ItemForms[i].Thumbnail_N = Helpers::LoadObjectFromPath<UTexture2D>(*Helpers::GetNormalThumbnailFromName(iteminfo.NameTag));
		Inventory[(uint8)ETypeTag::Cloth].ItemForms[i].Thumbnail_H = Helpers::LoadObjectFromPath<UTexture2D>(*Helpers::GetHoveredThumbnailFromName(iteminfo.NameTag));
		Inventory[(uint8)ETypeTag::Cloth].ItemForms[i].Thumbnail_S = Helpers::LoadObjectFromPath<UTexture2D>(*Helpers::GetSelectedThumbnailFromName(iteminfo.NameTag));

		if (i > 0)
		{
			Inventory[(uint8)ETypeTag::Cloth].ItemForms[i].MeshComponent = NewObject<UStaticMeshComponent>(this, UStaticMeshComponent::StaticClass(), FName(iteminfo.NameTag + "Mesh"));
			Inventory[(uint8)ETypeTag::Cloth].ItemForms[i].MeshComponent->SetStaticMesh(Helpers::LoadObjectFromPath<UStaticMesh>(*Helpers::GetMeshFromName(iteminfo.NameTag)));
			FAttachmentTransformRules rule = { EAttachmentRule::SnapToTarget, true };
			Inventory[(uint8)ETypeTag::Cloth].ItemForms[i].MeshComponent->AttachToComponent(GetMesh(), rule, FName(iteminfo.NameTag + "_unEquip"));
			Inventory[(uint8)ETypeTag::Cloth].ItemForms[i].MeshComponent->RegisterComponent();
		}
	}

	for (int i = 0; i < Inventory[(uint8)ETypeTag::Weapon].ItemForms.Num(); i++)
	{
		FItemShortForm iteminfo = Inventory[(uint8)ETypeTag::Weapon].ItemForms[i].ShortForm;
		Inventory[(uint8)ETypeTag::Weapon].ItemForms[i].Thumbnail_N = Helpers::LoadObjectFromPath<UTexture2D>(*Helpers::GetNormalThumbnailFromName(iteminfo.NameTag));
		Inventory[(uint8)ETypeTag::Weapon].ItemForms[i].Thumbnail_H = Helpers::LoadObjectFromPath<UTexture2D>(*Helpers::GetHoveredThumbnailFromName(iteminfo.NameTag));
		Inventory[(uint8)ETypeTag::Weapon].ItemForms[i].Thumbnail_S = Helpers::LoadObjectFromPath<UTexture2D>(*Helpers::GetSelectedThumbnailFromName(iteminfo.NameTag));

		if (i > 0)
		{
			Inventory[(uint8)ETypeTag::Weapon].ItemForms[i].MeshComponent = NewObject<UStaticMeshComponent>(this, UStaticMeshComponent::StaticClass(), FName(iteminfo.NameTag + "Mesh"));
			Inventory[(uint8)ETypeTag::Weapon].ItemForms[i].MeshComponent->SetStaticMesh(Helpers::LoadObjectFromPath<UStaticMesh>(*Helpers::GetMeshFromName(iteminfo.NameTag)));
			FAttachmentTransformRules rule = { EAttachmentRule::SnapToTarget, true };
			Inventory[(uint8)ETypeTag::Weapon].ItemForms[i].MeshComponent->AttachToComponent(GetMesh(), rule, FName(iteminfo.NameTag + "_unEquip"));
			Inventory[(uint8)ETypeTag::Weapon].ItemForms[i].MeshComponent->RegisterComponent();

			Inventory[(uint8)(ETypeTag::Weapon)].ItemForms[i].MeshComponent->OnComponentHit.AddDynamic(this, &AMainCharacter::OnHit);
			Inventory[(uint8)(ETypeTag::Weapon)].ItemForms[i].MeshComponent->OnComponentBeginOverlap.AddDynamic(this, &AMainCharacter::OnOverlapped);
		}
	}

	for (int i = 0; i < Inventory[(uint8)ETypeTag::Item].ItemForms.Num(); i++)
	{
		FItemShortForm iteminfo = Inventory[(uint8)ETypeTag::Item].ItemForms[i].ShortForm;
		Inventory[(uint8)ETypeTag::Item].ItemForms[i].Thumbnail_N = Helpers::LoadObjectFromPath<UTexture2D>(*Helpers::GetNormalThumbnailFromName(iteminfo.NameTag));
		Inventory[(uint8)ETypeTag::Item].ItemForms[i].Thumbnail_H = Helpers::LoadObjectFromPath<UTexture2D>(*Helpers::GetHoveredThumbnailFromName(iteminfo.NameTag));
		Inventory[(uint8)ETypeTag::Item].ItemForms[i].Thumbnail_S = Helpers::LoadObjectFromPath<UTexture2D>(*Helpers::GetSelectedThumbnailFromName(iteminfo.NameTag));
	}

	Equip();
}

void AMainCharacter::MoveForward(float val)
{
	if (!bHurt)
	{
		// 사다리에 매달렸을 경우, Forward Axis를 사다리 방향으로 조정한다
		if (LadderInfo.onLadder)
		{
			FVector direction = GetActorForwardVector().RotateAngleAxis(-LadderInfo.Slope, GetActorRightVector());
			AddMovementInput(direction, val);
		}
		else if (GetCurrentAction() == ECustomActionMode::IDLE)
		{
			FVector direction = GetActorForwardVector();
			AddMovementInput(direction, val);
		}
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
	if (!bHurt)
	{
		if (!LadderInfo.onLadder && GetCurrentAction() == ECustomActionMode::IDLE)
		{
			FVector direction = GetActorRightVector();
			AddMovementInput(direction, val);
		}
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
	if (!bHurt)
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
}

void AMainCharacter::StopJump()
{
	bPressedJump = false;
}

void AMainCharacter::Interact()
{
	if (!bHurt)
	{
		if (GetCurrentMovement() == ECustomMovementMode::IDLE && GetCurrentAction() == ECustomActionMode::IDLE)
		{
			SetCurrentAction(ECustomActionMode::INTERACT);
		}
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
	if (!bHurt)
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
			else if (Curr == "Seed")
			{
				ThrowSeed();
			}
		}
	}
}

void AMainCharacter::Attack()
{
	if (!bHurt)
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
}

void AMainCharacter::SubAttack()
{
	if (!bHurt)
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
	QuestList.WorkingQuests.Add(&quest);
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
			break;
		default:
			break;
		}
	}

	if (quest.Type == EQuestType::Serial)
	{
		quest.CurrPhase = 0;
		RegisterSubQuest(quest.SubQuests[quest.CurrPhase]);
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
		QuestList.ArrivalQuests.Add(&subquest);
		RegisterDestinationFlagVolume(&subquest);
		break;
	}
	case ESingleQuestType::Hunt:
		QuestList.HuntingQuests.Add(&subquest);
		break;
	case ESingleQuestType::Item:
	{
		for (int i = 0; i < subquest.ItemLists.Num(); i++)
		{
			FString ItemName = subquest.ItemLists[i].ItemName;
			auto preexist = Inventory[(uint8)subquest.ItemLists[i].ItemType].ItemForms.FindByPredicate([ItemName](const FItemForm& item) {return ItemName == item.ShortForm.NameTag; });
			if (preexist) subquest.currAmounts[i] = preexist->ShortForm.Num;
		}
		QuestList.ItemQuests.Add(&subquest);
		break;
	}
	case ESingleQuestType::Action:
		QuestList.ActionQuests.Add(&subquest);
		break;
	default:
		break;
	}
}

void AMainCharacter::EndQuest(FQuest& quest)
{
	quest.Progress = EQuestProgress::AlreadyDone;

	FQuest* qptr = &quest;
	int questindex = QuestList.WorkingQuests.IndexOfByPredicate([qptr](const FQuest* ptr) {return qptr == ptr; });

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
				arr = &QuestList.ArrivalQuests;
				break;
			case ESingleQuestType::Item:
				arr = &QuestList.ItemQuests;
				break;
			case ESingleQuestType::Hunt:
				arr = &QuestList.HuntingQuests;
				break;
			case ESingleQuestType::Action:
				arr = &QuestList.ActionQuests;
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

		QuestList.WorkingQuests.RemoveAt(questindex);
	}
}

void AMainCharacter::OpenQuestUI()
{
	GameMode->ChangeMenuWidget(GameMode->QuestUI);
	Cast<UQuestTable>(GameMode->QuestUI)->InitQuestUI(this);
}

void AMainCharacter::OpenMenu()
{
	GameMode->ChangeMenuWidget(GameMode->InGameMenuUI);
}

void AMainCharacter::OpenShowroom()
{
	if (!GameMode->bShowroom)
	{
		GameMode->ChangeMenuWidget(GameMode->ShowroomUI);
		GameMode->bShowroom = true;
		UGameplayStatics::GetPlayerController(this, 0)->SetInputMode(FInputModeUIOnly());
	}
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

void AMainCharacter::ReportKill(TArray<FString> labels)
{
	for (int i = 0; i < QuestList.HuntingQuests.Num(); i++)
	{
		int idx = QuestList.HuntingQuests[i]->HuntingLists.IndexOfByPredicate([labels](const FHuntingQuestForm& item) {
			for (auto label : labels)
			{
				if (label == item.Huntee) return true;
			}
			return false;
			});

		if (idx >= 0)
		{
			QuestList.HuntingQuests[i]->currAmounts[idx]++;

			if (!QuestList.HuntingQuests[i]->Completed)
			{
				if (QuestList.HuntingQuests[i]->CheckCompletion())
				{
					FQuest* Ownerquest = QuestList.HuntingQuests[i]->Owner;

					bool finished = Ownerquest->EndSingleTask();

					if (!finished && Ownerquest->Type == EQuestType::Serial)
					{
						RegisterSubQuest(Ownerquest->SubQuests[Ownerquest->CurrPhase]);
					}
				}
			}
		}
	}
}

void AMainCharacter::ReportItem(FString name, int num)
{
	for (int i = 0; i < QuestList.ItemQuests.Num(); i++)
	{
		int idx = QuestList.ItemQuests[i]->ItemLists.IndexOfByPredicate([name](const FItemQuestForm& item) {return name == item.ItemName; });
		if (idx >= 0)
		{
			QuestList.ItemQuests[i]->currAmounts[idx] += num;

			if (num > 0 && !QuestList.ItemQuests[i]->Completed)	// item을 얻은 경우이므로, 이전에 다 모았던 item quest는 건드릴 필요 없음
			{
				if (QuestList.ItemQuests[i]->CheckCompletion())
				{
					FQuest* Ownerquest = QuestList.ItemQuests[i]->Owner;

					bool finished = Ownerquest->EndSingleTask();

					if (!finished && Ownerquest->Type == EQuestType::Serial)
					{
						RegisterSubQuest(Ownerquest->SubQuests[Ownerquest->CurrPhase]);
					}
				}
			}
			else if (num < 0 && QuestList.ItemQuests[i]->Completed) // item을 잃은 경우 ==> 기존에 Complete 처리한 SubQuest를 다시 검사
			{
				if (!QuestList.ItemQuests[i]->CheckCompletion())
				{
					FQuest* Ownerquest = QuestList.ItemQuests[i]->Owner;

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
		RegisterSubQuest(Ownerquest->SubQuests[Ownerquest->CurrPhase]);
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
	for (auto quest : QuestList.ActionQuests)
	{
		if (quest->ActionCode == code && !quest->Completed)
		{
			quest->Completed = true;
			FQuest* Ownerquest = quest->Owner;
			if (!Ownerquest->EndSingleTask() && Ownerquest->Type == EQuestType::Serial)
			{
				RegisterSubQuest(Ownerquest->SubQuests[Ownerquest->CurrPhase]);
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