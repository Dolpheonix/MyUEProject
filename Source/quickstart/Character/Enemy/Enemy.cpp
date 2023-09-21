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
	// 캡슐 Collision Component
	auto Capsule = Cast<UCapsuleComponent>(RootComponent);
	Capsule->SetCollisionProfileName("Enemy");	// 콜리전 세팅 설정
	// 메인 Mesh Component
	auto MainMesh = GetMesh();
	MainMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	MainMesh->SetCollisionProfileName("NoCollision");
	MainMesh->SetGenerateOverlapEvents(true);
	// HP 바 Widget Component
	HPWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("HP Bar"));
	Helpers::SetComponent<UWidgetComponent>(&HPWidget, RootComponent, FVector(0.0f, 0.0f, 110.0f), FRotator(0.0f, 0.0f, 0.0f));
	// 물음표 Billboard Component
	QuestionMarkComponent = CreateDefaultSubobject<UBillboardComponent>(TEXT("Question Mark"));
	Helpers::SetComponent<UBillboardComponent>(&QuestionMarkComponent, RootComponent, FVector(0.0f, -35.0f, 100.0f), FRotator::ZeroRotator, FVector(0.04f ,0.04f, 0.04f));
	UTexture2D* QuestionMark = Helpers::C_LoadObjectFromPath<UTexture2D>(TEXT("/Game/ShootingGame/Image/InGameImage/QuestionMark.QuestionMark"));
	QuestionMarkComponent->SetSprite(QuestionMark);
	// 느낌표 Billboard Component
	ExclamationMarkComponent = CreateDefaultSubobject<UBillboardComponent>(TEXT("Exclamation Mark"));
	Helpers::SetComponent<UBillboardComponent>(&ExclamationMarkComponent, RootComponent, FVector(0.0f, -35.0f, 100.0f), FRotator::ZeroRotator, FVector(0.04f, 0.04f, 0.04f));
	UTexture2D* ExclamationMark = Helpers::C_LoadObjectFromPath<UTexture2D>(TEXT("/Game/ShootingGame/Image/InGameImage/ExclamationMark.ExclamationMark"));
	ExclamationMarkComponent->SetSprite(ExclamationMark);
	// Sound Cue 로드
	DoubtingSound = Helpers::C_LoadObjectFromPath<USoundCue>(TEXT("/Game/ShootingGame/Audio/Voice/HumanMaleA/SoundCue/voice_male_effort_grunt_02_Cue.voice_male_effort_grunt_02_Cue"));
	DetectingSound = Helpers::C_LoadObjectFromPath<USoundCue>(TEXT("/Game/ShootingGame/Audio/Voice/HumanMaleC/SoundCue/voice_male_c_attack_01_Cue.voice_male_c_attack_01_Cue"));
	// Audio Component에 Sound Cue 등록
	DetectionAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("Detection Audio"));
	DetectionAudioComponent->SetupAttachment(RootComponent);
	// AI 컨트롤러 설정
	AIControllerClass = AEnemyController::StaticClass();
	// 기본 라벨 추가
	Labels.Add("Enemy");
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

	// 물음표, 느낌표는 적 감지 시에만 보여줌
	QuestionMarkComponent->SetHiddenInGame(false);
	QuestionMarkComponent->SetVisibility(false);
	ExclamationMarkComponent->SetHiddenInGame(false);
	ExclamationMarkComponent->SetVisibility(false);
}

void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	// HP 바에 현재 HP 적용
	auto HPComp = Cast<UHPBar>(HPWidget->GetWidget());
	if(HPComp) HPComp->HP_ProgressBar->SetPercent(HP / 100.0f);
}

void AEnemy::OnHurt()
{
	// 원래 모드를 CacheMode로 임시 저장
	AIController->GetBlackboardComponent()->SetValueAsEnum("CacheMode", AIController->GetBlackboardComponent()->GetValueAsEnum("DetectionMode"));
	AIController->GetBlackboardComponent()->SetValueAsEnum("DetectionMode", uint8(EEnemyDetectionMode::HURT));
}

void AEnemy::OnDead()
{
	UBehaviorTreeComponent* bt = Cast<UBehaviorTreeComponent>(AIController->GetBrainComponent());
	if (bt)
	{
		bt->StopTree();	// 죽었을 시, 비헤이비어 트리를 종료
	}

	GetMesh()->PlayAnimation(Helpers::LoadObjectFromPath<UAnimSequence>("/Game/ShootingGame/Character/Main/Animations/FistAnim/Anim_Fist_Death.Anim_Fist_Death"), false);	// Dead 매니메이션 로드

	AMainCharacter* player = Cast<AMainCharacter>(UGameplayStatics::GetPlayerPawn(this, 0));

	if (player)
	{
		player->ReportKill(Labels);	// 플레이어에게 죽인 Enemy의 라벨을 전달
	}

	FTimerHandle destroyhandle;
	GetWorld()->GetTimerManager().SetTimer(destroyhandle, [this]() { this->Destroy(); }, 0.5f, false, 2.0f);	// 2초 후에 액터 삭제
}

void AEnemy::PlayDetectSound(bool isDoubt)
{
	isDoubt ? DetectionAudioComponent->SetSound(DoubtingSound) : DetectionAudioComponent->SetSound(DetectingSound);
	DetectionAudioComponent->Play();
}