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

	// ĸ�� Collision Component
	auto Capsule = Cast<UCapsuleComponent>(RootComponent);
	Capsule->SetCollisionProfileName("Character");

	// ���� �޽�
	auto MainMesh = GetMesh();
	MainMesh->SetSkeletalMesh(Helpers::C_LoadObjectFromPath<USkeletalMesh>(TEXT("/Game/ShootingGame/Character/Main/Mesh/SK_Mannequin.SK_Mannequin")));
	Helpers::SetComponent<USkeletalMeshComponent>(&MainMesh, RootComponent, FVector(0.f, 0.f, -88.f), FRotator(0.0f, -90.0f, 0.f));
	MainMesh->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	MainMesh->SetRenderCustomDepth(true);
	MainMesh->SetCustomDepthStencilValue(1);
	MainMesh->SetGenerateOverlapEvents(true);
	MainMesh->SetCollisionProfileName("CharacterBody");

	// ī�޶�
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	Helpers::SetComponent(&Camera, RootComponent, FVector(-172.0f, 0.0f, 85.0f), FRotator(-20.0f, 0.0f, 0.0f));
	Camera->bUsePawnControlRotation = true;
	Camera->PostProcessSettings.AddBlendable(Helpers::C_LoadObjectFromPath<UMaterial>(TEXT("/Game/ShootingGame/Asset/Material/PostProcess/M_Highlight.M_Highlight")), 1);

	// �߻���
	FireAudio = CreateDefaultSubobject<UAudioComponent>(TEXT("Audio"));
	FireAudio->SetupAttachment(RootComponent);
	FireAudio->SetSound(Helpers::C_LoadObjectFromPath<USoundCue>(TEXT("/Game/ShootingGame/Audio/SoundEffect/SoundCue/Explosion_01_Cue.Explosion_01_Cue")));
	FireAudio->bAutoActivate = false;

	// ������ Physics Constraint
	WeaponJoint = CreateDefaultSubobject<UPhysicsConstraintComponent>(TEXT("WeaponJoint"));
	WeaponJoint->SetLinearXLimit(ELinearConstraintMotion::LCM_Locked, 0.0f);
	WeaponJoint->SetLinearYLimit(ELinearConstraintMotion::LCM_Locked, 0.0f);
	WeaponJoint->SetLinearZLimit(ELinearConstraintMotion::LCM_Locked, 0.0f);
	WeaponJoint->SetAngularSwing1Limit(EAngularConstraintMotion::ACM_Locked, 0.0f);
	WeaponJoint->SetAngularSwing2Limit(EAngularConstraintMotion::ACM_Locked, 0.0f);
	WeaponJoint->SetAngularTwistLimit(EAngularConstraintMotion::ACM_Locked, 0.0f);
	WeaponJoint->SetupAttachment(RootComponent);
	WeaponJoint->SetRelativeLocation(FVector(100.0f, 0.0f, 0.0f));

	// ���ڱ� �Ҹ� �ε� �� Audio Component ���
	RunningSound = Helpers::C_LoadObjectFromPath<USoundCue>(TEXT("/Game/ShootingGame/Character/Main/Audio/SoundCue/Footstep_Run.Footstep_Run"));
	WalkingSound = Helpers::C_LoadObjectFromPath<USoundCue>(TEXT("/Game/ShootingGame/Character/Main/Audio/SoundCue/Footstep_Walk.Footstep_Walk"));
	FootstepAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("Footstep Audio"));
	FootstepAudioComponent->SetupAttachment(RootComponent);
}

void AMainCharacter::BeginPlay()
{
	Super::BeginPlay();

	bQuickslotDirty = true;	// �������� �ʱ⿡ ������Ʈ�ؾ� ��
	
	StartPos = GetActorLocation();	// �ʿ� ���� ��, ó�� ��ġ ����

	GameMode = (AMainGameMode*)UGameplayStatics::GetGameMode(this);

	UMainGameInstance* GI = Cast<UMainGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (GI)
	{
		GI->ApplyCharacterMemory(this);	// �ν��Ͻ��� ĳ���� �޸𸮸� �޾ƿ�
		LoadItemThumbnailAndMesh();	// �������� ����ϰ� �޽� �ε�
	}
	else
	{
		// �׽�Ʈ�� ���� �ν��Ͻ� ��� ��, �ʱ� �������� �ҷ���
#if WITH_EDITOR
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
#endif
	}

	Cast<UMainWidget>(GameMode->MainUI)->RefreshHPBar();	// HP �� �ʱ�ȭ
	Cast<UMainWidget>(GameMode->MainUI)->RefreshQuickslot();	// ���� UI�� ������ �ʱ�ȭ
}

void AMainCharacter::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

void AMainCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Falling ���� ������Ʈ
	if (GetCharacterMovement()->MovementMode == EMovementMode::MOVE_Falling) bFalling = true;
	else bFalling = false;

	// Hurt ��, 1�ʰ� ȭ�鿡 ���� ���� ����
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

	if (!bInteracting && InteractionFlag > 0)	// ��ȣ�ۿ� ���� ����
	{
		// ��ȣ�ۿ� Indicator UI ǥ��
		Cast<UMainWidget>(GameMode->MainUI)->NotifyInteractImage->SetVisibility(ESlateVisibility::Visible);
		Cast<UMainWidget>(GameMode->MainUI)->NotifyInteractTextBlock->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		// ��ȣ�ۿ� Indicator UI ����
		Cast<UMainWidget>(GameMode->MainUI)->NotifyInteractImage->SetVisibility(ESlateVisibility::Hidden);
		Cast<UMainWidget>(GameMode->MainUI)->NotifyInteractTextBlock->SetVisibility(ESlateVisibility::Hidden);
	}

	if(!bHurt) CheckEndMovement();	// Hurt ���� �ÿ��� Movement üũ�� �ʿ� ����
	CheckEndAction();

	CheckClimb();
}

FGenericTeamId AMainCharacter::GetGenericTeamId() const
{
	return FGenericTeamId(1);	// Enemy ���͵��� 2�� ��ȯ�ϹǷ�, Enemy Controller�� �÷��̾ ������ �ν���
}

void AMainCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("Move_Forward", this, &AMainCharacter::MoveForward);                       // W,S �����
	PlayerInputComponent->BindAxis("Move_Right", this, &AMainCharacter::MoveRight);                           // D,A �����
	PlayerInputComponent->BindAction("Move_Start", IE_Pressed, this, &AMainCharacter::MoveStart);			  // W,A,S,D ���� 
	PlayerInputComponent->BindAction("Move_End", IE_Released, this, &AMainCharacter::MoveEnd);				  // W,A,S,D ��

	PlayerInputComponent->BindAction("Run", IE_Pressed, this, &AMainCharacter::StartRun);                     // ���� Alt ����
	PlayerInputComponent->BindAction("Run", IE_Released, this, &AMainCharacter::StopRun);                     // ���� Alt ��
	PlayerInputComponent->BindAction("Walk", IE_Pressed, this, &AMainCharacter::StartWalk);                   // ���� Shift ����
	PlayerInputComponent->BindAction("Walk", IE_Released, this, &AMainCharacter::StopWalk);					  // ���� Shift ��

	PlayerInputComponent->BindAxis("Turn", this, &AMainCharacter::Turn);                                      // ���콺 X��
	PlayerInputComponent->BindAxis("LookUp", this, &AMainCharacter::LookUp);                                  // ���콺 Y��
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AMainCharacter::StartJump);                   // Spacebar ����
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &AMainCharacter::StopJump);                   // Spacebar ��

	PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &AMainCharacter::Interact);                // E ����
	PlayerInputComponent->BindAction("RollItems", IE_Pressed, this, &AMainCharacter::RollItems);			  // R ����
	PlayerInputComponent->BindAction("RollWeapons", IE_Pressed, this, &AMainCharacter::RollWeapons);          // T ����
	PlayerInputComponent->BindAction("Use", IE_Pressed, this, &AMainCharacter::Use);                          // F ����
	PlayerInputComponent->BindAction("Attack", IE_Pressed, this, &AMainCharacter::Attack);                    // ���콺 ���� ����
	PlayerInputComponent->BindAction("SubAttack", IE_Pressed, this, &AMainCharacter::SubAttack);              // ���콺 ������ ����
	PlayerInputComponent->BindAction("SubAttack", IE_Released, this, &AMainCharacter::unSubAttack);			  // ���콺 ������ ����

	PlayerInputComponent->BindAction("OpenShowroom", IE_Pressed, this, &AMainCharacter::OpenShowroom);        // I ����
	PlayerInputComponent->BindAction("OpenQuestUI", IE_Pressed, this, &AMainCharacter::OpenQuestUI);		  // Q ����
	PlayerInputComponent->BindAction("OpenMenu", IE_Pressed, this, &AMainCharacter::OpenMenu);				  // ESC ����
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
		// ��ٸ��� �Ŵ޷��� ���, Forward Axis�� ��ٸ� �������� �����Ѵ�
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
	FootstepAudioComponent->SetPitchMultiplier(1.7f);	// ���ڱ� �ӵ��� �ø�
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
	FootstepAudioComponent->SetPitchMultiplier(0.7f);	// ���ڱ� �ӵ��� ����
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
		if (LadderInfo.onLadder)	// ��ٸ��� ź ���¿��� ������ �ϸ� ��ٸ����� Ż��
		{
			LadderInfo.onLadder = false;
			LadderInfo.bDirty = true;
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
	// �������� ����/���� ������ ����
	Quickslots_Before[(uint8)type] = MathUtil::CircularMinus(Quickslots_Now[(uint8)type], Inventory[(uint8)type].ItemForms.Num());
	Quickslots_Next[(uint8)type] = MathUtil::CircularPlus(Quickslots_Now[(uint8)type], Inventory[(uint8)type].ItemForms.Num());
	Cast<UMainWidget>(GameMode->MainUI)->RefreshQuickslot();	// ���� UI�� �������� ������Ʈ
}

void AMainCharacter::Register(FItemShortForm iteminfo)
{
	FString name = iteminfo.NameTag;	// �������� �̸�
	// Ÿ�Կ� �´� �κ��丮���� name�� �´� ������ �ε��� ã��
	int index = Inventory[(uint8)iteminfo.TypeTag].ItemForms.IndexOfByPredicate([name](const FItemForm& itemform) {return itemform.ShortForm.NameTag == name; });

	if (index >= 0)	// �̹� �κ��丮�� �����Ѵٸ�, ������ �÷��� 
	{	
		Inventory[(uint8)iteminfo.TypeTag].ItemForms[index].ShortForm.Num += iteminfo.Num;
	}
	else			// ���� ����ؾ� ��
	{
		FItemForm registerform(iteminfo);		
		// ����� �ε�
		registerform.Thumbnail_N = Helpers::LoadObjectFromPath<UTexture2D>(*Helpers::GetNormalThumbnailFromName(iteminfo.NameTag));
		registerform.Thumbnail_H = Helpers::LoadObjectFromPath<UTexture2D>(*Helpers::GetHoveredThumbnailFromName(iteminfo.NameTag));
		registerform.Thumbnail_S = Helpers::LoadObjectFromPath<UTexture2D>(*Helpers::GetSelectedThumbnailFromName(iteminfo.NameTag));
		// ����, �ǻ��� �����ÿ� �Žø� �����ؾ� ��
		if (iteminfo.TypeTag != ETypeTag::Item)			
		{
			registerform.MeshComponent = NewObject<UStaticMeshComponent>(this, UStaticMeshComponent::StaticClass(), FName(iteminfo.NameTag + "Mesh"));
			registerform.MeshComponent->SetStaticMesh(Helpers::LoadObjectFromPath<UStaticMesh>(*Helpers::GetMeshFromName(iteminfo.NameTag)));
			FAttachmentTransformRules rule = { EAttachmentRule::SnapToTarget, true };
			registerform.MeshComponent->AttachToComponent(GetMesh(), rule, FName(iteminfo.NameTag + "_unEquip"));
			registerform.MeshComponent->RegisterComponent();
		}
		Inventory[(uint8)(iteminfo.TypeTag)].ItemForms.Add(registerform);
		// ���� �Žÿ� ��Ʈ �Լ� ���ε�
		if (iteminfo.TypeTag == ETypeTag::Weapon)		
		{
			Inventory[(uint8)(ETypeTag::Weapon)].ItemForms.Last().MeshComponent->OnComponentHit.AddDynamic(this, &AMainCharacter::OnHit);
			Inventory[(uint8)(ETypeTag::Weapon)].ItemForms.Last().MeshComponent->OnComponentBeginOverlap.AddDynamic(this, &AMainCharacter::OnOverlapped);
		}
	}
	ReportItem(name, iteminfo.Num);	// ������ ȹ�� ����Ʈ üũ
	RefreshInventory(iteminfo.TypeTag);	// �κ��丮 ������Ʈ
}

void AMainCharacter::DeleteItem(ETypeTag type, int index)
{
	if (index > 0)
	{
		if (type == ETypeTag::Weapon)	// ����� �޽� ������Ʈ���� �����ؾ� ��
		{
			Inventory[(uint8)type].ItemForms[index].MeshComponent->DestroyComponent();
		}
		Inventory[(uint8)type].ItemForms.RemoveAt(index);
		// ���� ������ ������Ʈ
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
	FItemForm* toTakeOff = &Inventory[(uint8)ETypeTag::Weapon].ItemForms[Quickslots_Before[(uint8)ETypeTag::Weapon]];	// ���� ������ ������
	FString Before = toTakeOff->ShortForm.NameTag;
	if (Before == "Fist")
	{

	}
	else
	{
		// �޽ø� unEquip �������� ������, ������ ����
		toTakeOff->MeshComponent->AttachToComponent(GetMesh(), { EAttachmentRule::SnapToTarget, true }, FName(Before + "_unEquip"));
		toTakeOff->MeshComponent->SetSimulatePhysics(false);
		toTakeOff->MeshComponent->SetGenerateOverlapEvents(false);
		toTakeOff->MeshComponent->SetNotifyRigidBodyCollision(false);
	}
}

void AMainCharacter::Equip()
{
	FItemForm* toTakeOn = &Inventory[(uint8)ETypeTag::Weapon].ItemForms[Quickslots_Now[(uint8)ETypeTag::Weapon]];	// ������ ������
	FString Curr = toTakeOn->ShortForm.NameTag;
	if (Curr == "Fist")
	{

	}
	else
	{
		// Equip ���Ͽ� ���� �� ������ ����
		toTakeOn->MeshComponent->AttachToComponent(GetMesh(), { EAttachmentRule::SnapToTarget, true }, FName(Curr + "_Equip"));
		WeaponJoint->SetConstrainedComponents(GetMesh(), FName("hand_r"), toTakeOn->MeshComponent, FName(""));
		toTakeOn->MeshComponent->SetSimulatePhysics(true);
		toTakeOn->MeshComponent->SetEnableGravity(false);
		toTakeOn->MeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		toTakeOn->MeshComponent->SetCollisionProfileName("Weapon");
		toTakeOn->MeshComponent->SetGenerateOverlapEvents(true);
		toTakeOn->MeshComponent->SetNotifyRigidBodyCollision(true);
	}
	WeaponCode = toTakeOn->ShortForm.Code;	// ���� �ڵ� ������Ʈ
}

void AMainCharacter::Use()
{
	if (!bHurt)	// Hurt �����϶� ������ ��� �Ұ���
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
	if (!bHurt)	// Hurt ������ �� ������ �� �� ����
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

void AMainCharacter::CheckClimb()
{
	if (LadderInfo.bDirty)	// ��ٸ��� Ÿ�� �����߰ų�, �� ���� ����
	{
		if (LadderInfo.onLadder)	// ��ٸ��� Ÿ�� ����
		{
			GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Flying);	// �߷��� ����
			GetMesh()->SetRelativeRotation(FRotator(0.0f, -90.0f, 90.0f - LadderInfo.Slope));	// ��ٸ��� ��翡 �°� ĳ������ Roll ȸ�� ����
		}
		else						// ��ٸ��� �� ����
		{
			GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
			GetMesh()->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));
		}

		LadderInfo.bDirty = false;
	}
	else
	{
		if (LadderInfo.onLadder)	// ��ٸ��� Ÿ�� �ִ� ���� ==> ƽ���� ��ٸ����� ��� �������� Ȯ��
		{
			FVector Tstart = GetActorLocation() - FVector(0.0f, 0.0f, 100.0f) + (GetActorRightVector() * LadderInfo.Width * 0.5f);	// Trace ���� ������
			FVector Tend = Tstart + GetActorForwardVector() * 100.0f;	// Trace ���� ����

			FHitResult hitres;
			bool Ishit = UKismetSystemLibrary::LineTraceSingle(this, Tstart, Tend, ETraceTypeQuery::TraceTypeQuery1, false, TArray<AActor*>(), EDrawDebugTrace::ForDuration, hitres, true, FLinearColor::Transparent, FLinearColor::Transparent, 0.0f);
			if (!Ishit)	// Trace ���ο� ��ٸ��� �ɸ��� ���� ==> ��ٸ����� ����
			{
				LadderInfo.onLadder = false;
				LadderInfo.Width = 0.0f;
				LadderInfo.bDirty = true;
				AddActorWorldOffset(GetActorForwardVector() * 100.0f + FVector(0.0f, 0.0f, 40.0f));	// ��ٸ��� �ɸ��� �ʰ� ��¦ ���� �����
			}
		}
	}
}

void AMainCharacter::RegisterQuest(FQuest& quest)
{
	quest.Progress = EQuestProgress::InProgress;	// ����Ʈ�� ������ ���·� ����
	QuestList.WorkingQuests.Add(&quest);			// ����Ʈ�� ���
	for (int i = 0; i < quest.SubQuests.Num(); i++)
	{
		FSingleQuest* subquest = &quest.SubQuests[i];
		subquest->Owner = &quest;
		switch (subquest->Type)
		{
		case ESingleQuestType::Hunt:
			subquest->currAmounts.SetNum(subquest->HuntingLists.Num());	// ���� óġ ���� 0���� �ʱ�ȭ
			break;
		case ESingleQuestType::Item:
			subquest->currAmounts.SetNum(subquest->ItemLists.Num());	// ���� ȹ�� ���� 0���� �ʱ�ȭ
			break;
		default:
			break;
		}
	}

	if (quest.Type == EQuestType::Serial)	// Serial ����Ʈ : ù��° ��������Ʈ�� ���
	{
		quest.CurrPhase = 0;
		RegisterSubQuest(quest.SubQuests[quest.CurrPhase]);
	}
	else									// Parallel ����Ʈ : ��� ��������Ʈ ���
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
		RegisterDestinationFlagVolume(&subquest);	// ������ Indicator ���� ��ġ
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
			// �̹� �����ϴ� �������� ���, ȹ�� �� ������Ʈ
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
	quest.Progress = EQuestProgress::AlreadyDone;	// ����Ʈ ���൵�� �Ϸ�� ����

	FQuest* qptr = &quest;
	int questindex = QuestList.WorkingQuests.IndexOfByPredicate([qptr](const FQuest* ptr) {return qptr == ptr; });	// �Ϸ��� ����Ʈ�� �ε���

	if (questindex < 0)
	{
		UE_LOG(ErrQuest, Log, TEXT("Quest that want to remove does not exist"));	// �Ϸ��� ����Ʈ�� �÷��̾�� ��ϵ��� ����
	}
	else
	{
		// ���� ����Ʈ���� ��� ����
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

		// �����带 �����ϰ�, Notify Queue�� �߰�
		for (FReward reward : qptr->Rewards)
		{
			switch (reward.Type)
			{
			case ERewardType::EXP:
				// TODO : EXP �ý���
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

		// ����Ʈ ��� ����
		QuestList.WorkingQuests.RemoveAt(questindex);
	}
}

void AMainCharacter::OpenQuestUI()
{
	GameMode->ChangeCurrentWidget(GameMode->QuestUI);
	Cast<UQuestTable>(GameMode->QuestUI)->InitQuestUI(this);
}

void AMainCharacter::OpenMenu()
{
	GameMode->ChangeCurrentWidget(GameMode->InGameMenuUI);
}

void AMainCharacter::OpenShowroom()
{
	GameMode->ChangeCurrentWidget(GameMode->ShowroomUI);
	UGameplayStatics::GetPlayerController(this, 0)->SetInputMode(FInputModeUIOnly());
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
	// Action ���°� Idle || Attack �̰�, �������� ���°� �ƴϾ�� �� (������ ����)
	return (GetCurrentAction() == ECustomActionMode::IDLE || GetCurrentAction() == ECustomActionMode::ATTACK) && (!bFalling || GetCurrentMovement() == ECustomMovementMode::JUMP);
}

void AMainCharacter::OnHurt()
{
	bHurt = true;
}

void AMainCharacter::OnDead()
{
	// Death �ִϸ��̼� ���
	GetMesh()->PlayAnimation(Helpers::LoadObjectFromPath<UAnimSequence>("/Game/ShootingGame/Character/Main/Animations/FistAnim/Anim_Fist_Death.Anim_Fist_Death"), false);
	// ó�� ������ġ�� �̵�
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
	// �������� óġ ����Ʈ�� ��� üũ
	for (int i = 0; i < QuestList.HuntingQuests.Num(); i++)
	{
		int idx = QuestList.HuntingQuests[i]->HuntingLists.IndexOfByPredicate([labels](const FHuntingQuestForm& item) {
			for (auto label : labels)
			{
				if (label == item.Huntee) return true;	// ���� ��ġ�Ѵٸ� �ε��� ��ȯ
			}
			return false;
			});

		if (idx >= 0)
		{
			QuestList.HuntingQuests[i]->currAmounts[idx]++; // óġ Ƚ�� ����

			if (!QuestList.HuntingQuests[i]->Completed)	// ����Ʈ�� ���� ������ �ʾҴٸ�, �Ϸ� ���� ������Ʈ
			{
				if (QuestList.HuntingQuests[i]->CheckCompletion())	// ���� ����Ʈ�� �Ϸ�Ǿ��� ���
				{
					FQuest* Ownerquest = QuestList.HuntingQuests[i]->Owner;

					bool finished = Ownerquest->EndSingleTask();

					if (!finished && Ownerquest->Type == EQuestType::Serial)	// ���� ��������Ʈ ���
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

			if (num > 0 && !QuestList.ItemQuests[i]->Completed)	// item�� ���� ����̹Ƿ�, ������ �� ��Ҵ� item quest�� �ǵ帱 �ʿ� ����
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
			else if (num < 0 && QuestList.ItemQuests[i]->Completed) // item�� ���� ��� ==> ������ Complete ó���� SubQuest�� �ٽ� �˻�
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

	// �������� Beacon ������Ʈ�� ����
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
		// Notify �˾� UI�� ����
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
	Cast<UMainWidget>(GameMode->MainUI)->RefreshHPBar();	// HP �ٸ� ������Ʈ

	return Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
}