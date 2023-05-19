// Fill out your copyright notice in the Description page of Project Settings.
#include "MainCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
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
	FItemForm fist = FItemForm(FItemShortForm(ETypeTag::Weapon, "Fist"));
	FItemForm noitem = FItemForm(FItemShortForm(ETypeTag::Item, "NoItem"));
	fist.ShortForm.InfoTag = "Fist";
	fist.ShortForm.Code = 0;
	noitem.ShortForm.InfoTag = " NoItem";
	Weapons.Add(fist);
	Items.Add(noitem);
	Weapons[0].Thumbnail_N = Helpers::C_LoadObjectFromPath<UTexture2D>(TEXT("/Game/ShootingGame/Image/WidgetImage/Normal/Fist_Normal.Fist_Normal"));
	Weapons[0].Thumbnail_H = Helpers::C_LoadObjectFromPath<UTexture2D>(TEXT("/Game/ShootingGame/Image/WidgetImage/Hovered/Fist_Hovered.Fist_Hovered"));
	Weapons[0].Thumbnail_S = Helpers::C_LoadObjectFromPath<UTexture2D>(TEXT("/Game/ShootingGame/Image/WidgetImage/Selected/Fist_Selected.Fist_Selected"));
	Items[0].Thumbnail_N = Helpers::C_LoadObjectFromPath<UTexture2D>(TEXT("/Game/ShootingGame/Image/WidgetImage/Normal/NoItem_Normal.NoItem_Normal"));
	Items[0].Thumbnail_H = Helpers::C_LoadObjectFromPath<UTexture2D>(TEXT("/Game/ShootingGame/Image/WidgetImage/Hovered/NoItem_Hovered.NoItem_Hovered"));
	Items[0].Thumbnail_S = Helpers::C_LoadObjectFromPath<UTexture2D>(TEXT("/Game/ShootingGame/Image/WidgetImage/Selected/NoItem_Selected.NoItem_Selected"));

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
	if (Items.Num() > 0 && GetCurrentAction() == ECustomActionMode::IDLE)
	{
		Item_Now = Item_Next;
		Item_Next = MathUtil::CircularPlus(Item_Now, Items.Num());
		bQuickslotDirty = true;
	}
}

void AMainCharacter::RollWeapons()
{
	if (Weapons.Num() > 0 && GetCurrentAction() == ECustomActionMode::IDLE)
	{
		Weapon_Before = Weapon_Now;
		Weapon_Now = Weapon_Next;
		Weapon_Next = MathUtil::CircularPlus(Weapon_Now, Weapons.Num());
		unEquip();
		Equip();
		bQuickslotDirty = true;
	}
}

void AMainCharacter::RefreshInventory(ETypeTag type)
{
	if (type == ETypeTag::Item)
	{
		Item_Next = MathUtil::CircularPlus(Item_Now, Items.Num());
		bQuickslotDirty = true;
	}
	else if (type == ETypeTag::Weapon)
	{
		Weapon_Next = MathUtil::CircularPlus(Weapon_Now, Weapons.Num());
		bQuickslotDirty = true;
	}
	else
	{
		Cloth_Next = MathUtil::CircularPlus(Cloth_Now, Clothes.Num());
	}
}

void AMainCharacter::Register(FItemShortForm iteminfo)
{
	FItemForm registerform(iteminfo);

	switch (registerform.ShortForm.TypeTag)
	{
	case ETypeTag::Weapon:
	{
		registerform.MeshComponent = NewObject<UStaticMeshComponent>(this, UStaticMeshComponent::StaticClass(), FName(iteminfo.NameTag + "Mesh"));
		registerform.Thumbnail_N = Helpers::LoadObjectFromPath<UTexture2D>(*Helpers::GetNormalThumbnailFromName(iteminfo.NameTag));
		registerform.Thumbnail_H = Helpers::LoadObjectFromPath<UTexture2D>(*Helpers::GetHoveredThumbnailFromName(iteminfo.NameTag));
		registerform.Thumbnail_S = Helpers::LoadObjectFromPath<UTexture2D>(*Helpers::GetSelectedThumbnailFromName(iteminfo.NameTag));
		registerform.MeshComponent->SetStaticMesh(Helpers::LoadObjectFromPath<UStaticMesh>(*Helpers::GetMeshFromName(iteminfo.NameTag)));
		FAttachmentTransformRules rule = { EAttachmentRule::SnapToTarget, true };
		registerform.MeshComponent->AttachToComponent(GetMesh(), rule, FName(iteminfo.NameTag + "_unEquip"));
		registerform.MeshComponent->RegisterComponent();
		Weapons.Add(registerform);
		Weapons[Weapons.Num() - 1].MeshComponent->OnComponentHit.AddDynamic(this, &AMainCharacter::OnHit);
		Weapons[Weapons.Num() - 1].MeshComponent->OnComponentBeginOverlap.AddDynamic(this, &AMainCharacter::OnOverlapped);
		RefreshInventory(iteminfo.TypeTag);
		break;
	}
	case ETypeTag::Item:
	{
		registerform.Thumbnail_N = Helpers::LoadObjectFromPath<UTexture2D>(*Helpers::GetNormalThumbnailFromName(iteminfo.NameTag));
		registerform.Thumbnail_H = Helpers::LoadObjectFromPath<UTexture2D>(*Helpers::GetHoveredThumbnailFromName(iteminfo.NameTag));
		registerform.Thumbnail_S = Helpers::LoadObjectFromPath<UTexture2D>(*Helpers::GetSelectedThumbnailFromName(iteminfo.NameTag));
		Items.Add(registerform);
		RefreshInventory(iteminfo.TypeTag);
		break;
	}
	case ETypeTag::Cloth:
	{
		registerform.MeshComponent = NewObject<UStaticMeshComponent>(this, UStaticMeshComponent::StaticClass(), FName(iteminfo.NameTag + "Mesh"));
		registerform.Thumbnail_N = Helpers::LoadObjectFromPath<UTexture2D>(*Helpers::GetNormalThumbnailFromName(iteminfo.NameTag));
		registerform.Thumbnail_H = Helpers::LoadObjectFromPath<UTexture2D>(*Helpers::GetHoveredThumbnailFromName(iteminfo.NameTag));
		registerform.Thumbnail_S = Helpers::LoadObjectFromPath<UTexture2D>(*Helpers::GetSelectedThumbnailFromName(iteminfo.NameTag));
		registerform.MeshComponent->SetStaticMesh(Helpers::LoadObjectFromPath<UStaticMesh>(*Helpers::GetMeshFromName(iteminfo.NameTag)));
		FAttachmentTransformRules rule = { EAttachmentRule::SnapToTarget, true };
		registerform.MeshComponent->AttachToComponent(GetMesh(), rule, FName(iteminfo.NameTag + "_unEquip"));
		registerform.MeshComponent->RegisterComponent();
		Weapons.Add(registerform);
		RefreshInventory(iteminfo.TypeTag);
		break;
	}
	default:
		break;
	}
}

void AMainCharacter::DeleteItem(ETypeTag type, int index)
{
	if (type == ETypeTag::Cloth && index > 0)
	{
		Clothes.RemoveAt(index);
		Cloth_Now = Cloth_Now >= Clothes.Num() ? Clothes.Num() - 1 : Cloth_Now;
	}
	else if (type == ETypeTag::Weapon && index > 0)
	{
		Weapons[index].MeshComponent->DestroyComponent();
		Weapons.RemoveAt(index);
		Weapon_Now = Weapon_Now >= Weapons.Num() ? Weapons.Num() - 1 : Weapon_Now;
		Equip();
	}
	else if(index > 0)
	{
		Items.RemoveAt(index);
		Item_Now = Item_Now >= Items.Num() ? Items.Num() - 1 : Item_Now;
	}

	RefreshInventory(type);
	bQuickslotDirty = true;
}

void AMainCharacter::unEquip()
{
	FString Before = Weapons[Weapon_Before].ShortForm.NameTag;
	if (Before == "Fist")
	{

	}
	else
	{
		// Constraint between back & weapon + physics & Collision settings off
		Weapons[Weapon_Before].MeshComponent->AttachToComponent(GetMesh(), { EAttachmentRule::SnapToTarget, true }, FName(Before + "_unEquip"));
		Weapons[Weapon_Before].MeshComponent->SetSimulatePhysics(false);
		Weapons[Weapon_Before].MeshComponent->SetGenerateOverlapEvents(false);
		Weapons[Weapon_Before].MeshComponent->SetNotifyRigidBodyCollision(false);
	}
}

void AMainCharacter::Equip()
{
	FString Curr = Weapons[Weapon_Now].ShortForm.NameTag;
	if (Curr == "Fist")
	{

	}
	else
	{
		// Constraint between hand & weapon + physics & Collision Settings ON
		Weapons[Weapon_Now].MeshComponent->AttachToComponent(GetMesh(), { EAttachmentRule::SnapToTarget, true }, FName(Curr + "_Equip"));
		WeaponJoint->SetConstrainedComponents(GetMesh(), FName("hand_r"), Weapons[Weapon_Now].MeshComponent, FName(""));
		Weapons[Weapon_Now].MeshComponent->SetSimulatePhysics(true);
		Weapons[Weapon_Now].MeshComponent->SetEnableGravity(false);
		Weapons[Weapon_Now].MeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		Weapons[Weapon_Now].MeshComponent->SetCollisionProfileName("Weapon");
		Weapons[Weapon_Now].MeshComponent->SetGenerateOverlapEvents(true);
		Weapons[Weapon_Now].MeshComponent->SetNotifyRigidBodyCollision(true);
	}
	WeaponCode = Weapons[Weapon_Now].ShortForm.Code;
}

void AMainCharacter::Use()
{
	if (GetCurrentAction() == ECustomActionMode::IDLE)
	{
		FString Curr = Items[Item_Now].ShortForm.NameTag;

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
	if (WeaponCode != 0 && !Weapons[Weapon_Now].MeshComponent->IsSimulatingPhysics())
	{
		Weapons[Weapon_Now].MeshComponent->SetSimulatePhysics(true);
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