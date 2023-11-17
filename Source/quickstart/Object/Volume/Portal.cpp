#include "Portal.h"
#include "../../Utils/Helpers.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PawnMovementComponent.h"

APortal::APortal()
{
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	Volume = CreateDefaultSubobject<UBoxComponent>(TEXT("Volume"));
	Helpers::SetComponent<UBoxComponent>(&Volume, RootComponent, FVector(0.0f, 0.0f, 0.0f), FRotator(0.0f, 0.0f, 0.0f));
	Volume->SetBoxExtent(FVector(100.0f, 100.0f, 100.0f));
	Volume->SetCollisionProfileName("Volume");

	FXComponent = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("FX"));
	Helpers::SetComponent<UParticleSystemComponent>(&FXComponent, Volume, FVector(0.0f, 0.0f, 0.0f), FRotator(0.0f, 0.0f, 0.0f));

	Destination = CreateDefaultSubobject<UArrowComponent>(TEXT("Arrow"));
	Helpers::SetComponent<UArrowComponent>(&Destination, Volume, FVector(0.0f, 0.0f, 0.0f), FRotator(0.0f, 0.0f, 0.0f));
	Destination->SetWorldLocation(GetActorLocation() + FVector(Volume->GetScaledBoxExtent().X + 100.0f, 0.0f, 0.0f));
	Destination->SetMobility(EComponentMobility::Movable);

	Volume->OnComponentBeginOverlap.AddDynamic(this, &APortal::OnOverlapped_Teleport);
}

#if WITH_EDITOR
void APortal::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	auto prop = PropertyChangedEvent.Property;
	auto name = prop->GetName();

	if (name == "Pair")	// Pair 포탈이 바뀜
	{
		if (Pair == this)
		{
			Pair = Oldpair;	// 자기 자신을 선택했을 경우, 이전 Pair로 다시 되돌림
		}
		else if(Pair != Oldpair)	// Pair를 바꾼 경우 OR 삭제한 경우
		{
			if (Pair) Pair->UpdatePair(this);	// 새로운 Pair의 Pair를 이 포탈로 변경
			if (Oldpair) Oldpair->UpdatePair(nullptr);	// 이전 Pair의 Pair를 삭제

			if (Pair)
			{
				Connect();	// 새로운 Pair와 연결
			}
			else
			{
				Disconnect();	// Pair와의 연결을 끊음
			}

			Oldpair = Pair;
		}
	}
	else if (name == "bPairing")	// Pairing 상태가 변경
	{
		if (bPairing && Pair)	// 설정된 Pair와의 연결을 재개
		{
			Connect();
			Pair->bPairing = true;
			if(Pair->Pair) Pair->Connect();
		}
		else if(Pair)	// Pair와의 연결을 끊음
		{
			Disconnect();
			Pair->bPairing = false;
			Pair->Disconnect();
		}
	}
	else if (name == "PortalFX")	// 포탈의 FX가 변경
	{
		FXComponent->SetTemplate(PortalFX);
	}
}
#endif

void APortal::OnOverlapped_Teleport(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	auto main = UGameplayStatics::GetPlayerPawn(this, 0);
	if (OtherActor == main)
	{
		auto destination = Destination->GetComponentLocation();

		main->SetActorLocation(destination);
		main->GetController()->SetControlRotation(Destination->GetComponentRotation());
		main->GetMovementComponent()->Velocity = FVector::ZeroVector;
	}
}

void APortal::UpdatePair(APortal* newpair)
{
	if (newpair) // 새로운 pair로부터 pairing됨
	{
		if (!bPairing)
		{
			bPairing = true;
		}

		if (Pair)
		{
			Pair->UpdatePair(nullptr);
		}

		Pair = newpair;
		Oldpair = Pair;
		Connect();
	}
	else // 기존의 pair가 pairing을 끊음
	{
		Pair = nullptr;
		Oldpair = Pair;
		Disconnect();
	}
}

void APortal::Connect()
{
	Destination->AttachToComponent(nullptr, FAttachmentTransformRules(EAttachmentRule::KeepWorld, true));
	Destination->SetWorldLocation(Pair->GetActorLocation() + Pair->GetActorForwardVector() * Pair->Volume->GetScaledBoxExtent().Size());
	Destination->AttachToComponent(Pair->Volume, FAttachmentTransformRules(EAttachmentRule::KeepWorld, true));
	Destination->SetRelativeRotation(FRotator::ZeroRotator);
}

void APortal::Disconnect()
{
	Destination->AttachToComponent(nullptr, FAttachmentTransformRules(EAttachmentRule::KeepWorld, true));
	Destination->SetWorldLocation(GetActorLocation() + GetActorForwardVector() * Volume->GetScaledBoxExtent().Size());
	Destination->AttachToComponent(Volume, FAttachmentTransformRules(EAttachmentRule::KeepWorld, true));
	Destination->SetRelativeRotation(FRotator::ZeroRotator);
}