// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"
#include "Components/CapsuleComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "../Main/MainCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "../../Utils/Helpers.h"

AEnemy::AEnemy()
{
	PrimaryActorTick.bCanEverTick = true;

	auto Capsule = Cast<UCapsuleComponent>(RootComponent);
	Capsule->SetCollisionProfileName("Enemy");

	auto MainMesh = GetMesh();
	MainMesh->SetSkeletalMesh(Helpers::C_LoadObjectFromPath<USkeletalMesh>(TEXT("/Game/ShootingGame/Character/Main/Mesh/SK_Mannequin.SK_Mannequin")));
	Helpers::SetComponent<USkeletalMeshComponent>(&MainMesh, RootComponent, FVector(0.f, 0.f, -88.f), FRotator(0.0f, -90.0f, 0.f));
	MainMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	MainMesh->SetCollisionProfileName("NoCollision");
	MainMesh->SetGenerateOverlapEvents(true);

	HPWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("HP Bar"));
	Helpers::SetComponent<UWidgetComponent>(&HPWidget, RootComponent, FVector(0.0f, 0.0f, 110.0f), FRotator(0.0f, 0.0f, 0.0f));

	QuestionMarkComponent = CreateDefaultSubobject<UBillboardComponent>(TEXT("Question Mark"));
	Helpers::SetComponent<UBillboardComponent>(&QuestionMarkComponent, RootComponent, FVector(0.0f, -35.0f, 100.0f), FRotator::ZeroRotator, FVector(0.04f ,0.04f, 0.04f));
	UTexture2D* QuestionMark = Helpers::C_LoadObjectFromPath<UTexture2D>(TEXT("/Game/ShootingGame/Image/QuestionMark.QuestionMark"));
	QuestionMarkComponent->SetSprite(QuestionMark);
	QuestionMarkComponent->SetHiddenInGame(false);
	QuestionMarkComponent->SetVisibility(false);

	ExclamationMarkComponent = CreateDefaultSubobject<UBillboardComponent>(TEXT("Exclamation Mark"));
	Helpers::SetComponent<UBillboardComponent>(&ExclamationMarkComponent, RootComponent, FVector(0.0f, -35.0f, 100.0f), FRotator::ZeroRotator, FVector(0.04f, 0.04f, 0.04f));
	UTexture2D* ExclamationMark = Helpers::C_LoadObjectFromPath<UTexture2D>(TEXT("/Game/ShootingGame/Image/ExclamationMark.ExclamationMark"));
	ExclamationMarkComponent->SetSprite(ExclamationMark);
	ExclamationMarkComponent->SetHiddenInGame(false);
	ExclamationMarkComponent->SetVisibility(false);

	FireSound = Helpers::C_LoadObjectFromPath<USoundCue>(TEXT("/Game/StarterContent/Audio/Explosion_Cue.Explosion_Cue"));
	DoubtingSound = Helpers::C_LoadObjectFromPath<USoundCue>(TEXT("/Game/HumanVocalizations/HumanMaleA/Cues/voice_male_effort_grunt_02_Cue.voice_male_effort_grunt_02_Cue"));
	DetectingSound = Helpers::C_LoadObjectFromPath<USoundCue>(TEXT("/Game/HumanVocalizations/HumanMaleC/Cues/voice_male_c_attack_01_Cue.voice_male_c_attack_01_Cue"));
	
	FireAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("Fire Audio"));
	FireAudioComponent->SetupAttachment(RootComponent);
	FireAudioComponent->SetSound(FireSound);

	DetectionAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("Detection Audio"));
	DetectionAudioComponent->SetupAttachment(RootComponent);



	WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Rifle"));
	WeaponMesh->SetStaticMesh(Helpers::C_LoadObjectFromPath<UStaticMesh>(*Helpers::GetMeshFromName("Rifle")));
	WeaponMesh->AttachToComponent(GetMesh(), { EAttachmentRule::SnapToTarget, true }, FName("Rifle_Equip"));
	WeaponMesh->SetCollisionProfileName("Weapon");
}

void AEnemy::BeginPlay()
{
	Super::BeginPlay();

	AIController = Cast<AAIController>(GetController());

	if (!HPWidgetClass)
	{
		HPWidgetClass = UHPBar::StaticClass();
	}
	HPWidget->SetWidgetClass(HPWidgetClass);
	HPWidget->SetDrawSize(FVector2D(100.0f, 10.0f));
}

void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	auto HPComp = Cast<UHPBar>(HPWidget->GetWidget());
	if(HPComp) HPComp->HP_ProgressBar->SetPercent(HP / 100.0f);
}

void AEnemy::Fire()
{
	FVector MuzzleLocation = GetMesh()->GetSocketLocation(TEXT("Rifle_Muzzle"));
	UWorld* World = GetWorld();
	if (World)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		SpawnParams.Instigator = GetInstigator(); // instigator : spawn을 trigger한 주체
		ABullet* Projectile = World->SpawnActor<ABullet>(ProjectileClass, MuzzleLocation, FRotator(0, 0, 0), SpawnParams); // world에 actor를 스폰
		if (Projectile)
		{
			FVector LaunchDirection = GetControlRotation().Vector();
			Projectile->FireInDirection(LaunchDirection); // 발사체 velocity 결정

			FireAudioComponent->Play();
		}
	}
}

void AEnemy::PlayDetectSound(bool isDoubt)
{
	isDoubt ? DetectionAudioComponent->SetSound(DoubtingSound) : DetectionAudioComponent->SetSound(DetectingSound);
	DetectionAudioComponent->Play();
}

void AEnemy::OnHurt()
{
	AIController->GetBlackboardComponent()->SetValueAsEnum("CacheMode", AIController->GetBlackboardComponent()->GetValueAsEnum("DetectionMode"));
	AIController->GetBlackboardComponent()->SetValueAsEnum("DetectionMode", uint8(EEnemyDetectionMode::HURT));
}

void AEnemy::OnDead()
{
	UBehaviorTreeComponent* bt = Cast<UBehaviorTreeComponent>(AIController->GetBrainComponent());
	if (bt)
	{
		bt->StopTree();
	}

	GetMesh()->PlayAnimation(Helpers::LoadObjectFromPath<UAnimSequence>("/Game/ShootingGame/Character/Main/Animations/FistAnim/Anim_Fist_Death.Anim_Fist_Death"), false);

	AMainCharacter* player = Cast<AMainCharacter>(UGameplayStatics::GetPlayerPawn(this, 0));

	if (player)
	{
		player->ReportKill(StaticClass());
	}

	FTimerHandle destroyhandle;
	GetWorld()->GetTimerManager().SetTimer(destroyhandle, [this]() { this->Destroy(); }, 0.5f, false, 2.0f);
}