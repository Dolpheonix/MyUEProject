// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"
#include "EnemyController.h"
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
	MainMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	MainMesh->SetCollisionProfileName("NoCollision");
	MainMesh->SetGenerateOverlapEvents(true);

	HPWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("HP Bar"));
	Helpers::SetComponent<UWidgetComponent>(&HPWidget, RootComponent, FVector(0.0f, 0.0f, 110.0f), FRotator(0.0f, 0.0f, 0.0f));

	QuestionMarkComponent = CreateDefaultSubobject<UBillboardComponent>(TEXT("Question Mark"));
	Helpers::SetComponent<UBillboardComponent>(&QuestionMarkComponent, RootComponent, FVector(0.0f, -35.0f, 100.0f), FRotator::ZeroRotator, FVector(0.04f ,0.04f, 0.04f));
	UTexture2D* QuestionMark = Helpers::C_LoadObjectFromPath<UTexture2D>(TEXT("/Game/ShootingGame/Image/InGameImage/QuestionMark.QuestionMark"));
	QuestionMarkComponent->SetSprite(QuestionMark);

	ExclamationMarkComponent = CreateDefaultSubobject<UBillboardComponent>(TEXT("Exclamation Mark"));
	Helpers::SetComponent<UBillboardComponent>(&ExclamationMarkComponent, RootComponent, FVector(0.0f, -35.0f, 100.0f), FRotator::ZeroRotator, FVector(0.04f, 0.04f, 0.04f));
	UTexture2D* ExclamationMark = Helpers::C_LoadObjectFromPath<UTexture2D>(TEXT("/Game/ShootingGame/Image/InGameImage/ExclamationMark.ExclamationMark"));
	ExclamationMarkComponent->SetSprite(ExclamationMark);

	DoubtingSound = Helpers::C_LoadObjectFromPath<USoundCue>(TEXT("/Game/ShootingGame/Audio/Voice/HumanMaleA/SoundCue/voice_male_effort_grunt_02_Cue.voice_male_effort_grunt_02_Cue"));
	DetectingSound = Helpers::C_LoadObjectFromPath<USoundCue>(TEXT("/Game/ShootingGame/Audio/Voice/HumanMaleC/SoundCue/voice_male_c_attack_01_Cue.voice_male_c_attack_01_Cue"));

	DetectionAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("Detection Audio"));
	DetectionAudioComponent->SetupAttachment(RootComponent);

	AIControllerClass = AEnemyController::StaticClass();
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

	QuestionMarkComponent->SetHiddenInGame(false);
	QuestionMarkComponent->SetVisibility(false);

	ExclamationMarkComponent->SetHiddenInGame(false);
	ExclamationMarkComponent->SetVisibility(false);
}

void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	auto HPComp = Cast<UHPBar>(HPWidget->GetWidget());
	if(HPComp) HPComp->HP_ProgressBar->SetPercent(HP / 100.0f);
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

void AEnemy::PlayDetectSound(bool isDoubt)
{
	isDoubt ? DetectionAudioComponent->SetSound(DoubtingSound) : DetectionAudioComponent->SetSound(DetectingSound);
	DetectionAudioComponent->Play();
}