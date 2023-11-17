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

	if (name == "Pair")	// Pair ��Ż�� �ٲ�
	{
		if (Pair == this)
		{
			Pair = Oldpair;	// �ڱ� �ڽ��� �������� ���, ���� Pair�� �ٽ� �ǵ���
		}
		else if(Pair != Oldpair)	// Pair�� �ٲ� ��� OR ������ ���
		{
			if (Pair) Pair->UpdatePair(this);	// ���ο� Pair�� Pair�� �� ��Ż�� ����
			if (Oldpair) Oldpair->UpdatePair(nullptr);	// ���� Pair�� Pair�� ����

			if (Pair)
			{
				Connect();	// ���ο� Pair�� ����
			}
			else
			{
				Disconnect();	// Pair���� ������ ����
			}

			Oldpair = Pair;
		}
	}
	else if (name == "bPairing")	// Pairing ���°� ����
	{
		if (bPairing && Pair)	// ������ Pair���� ������ �簳
		{
			Connect();
			Pair->bPairing = true;
			if(Pair->Pair) Pair->Connect();
		}
		else if(Pair)	// Pair���� ������ ����
		{
			Disconnect();
			Pair->bPairing = false;
			Pair->Disconnect();
		}
	}
	else if (name == "PortalFX")	// ��Ż�� FX�� ����
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
	if (newpair) // ���ο� pair�κ��� pairing��
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
	else // ������ pair�� pairing�� ����
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