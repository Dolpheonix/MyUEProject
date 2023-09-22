#include "Elevator.h"
#include <Kismet/KismetMathLibrary.h>

AElevator::AElevator()
{
	PrimaryActorTick.bCanEverTick = true;

	ElevatorBody = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ElevatorBody"));
	ElevatorBody->SetStaticMesh(Helpers::C_LoadObjectFromPath<UStaticMesh>(TEXT("/Game/ShootingGame/Asset/Prop/StaticMesh/Elevator_Body.Elevator_Body")));
	ElevatorBody->SetupAttachment(RootComponent);

	ElevatorInternalSwitch = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ElevatorInternalSwitch"));
	ElevatorInternalSwitch->SetStaticMesh(Helpers::C_LoadObjectFromPath<UStaticMesh>(TEXT("/Game/ShootingGame/Asset/Prop/StaticMesh/Elevator_Switch.Elevator_Switch")));
	Helpers::SetComponent<UStaticMeshComponent>(&ElevatorInternalSwitch, RootComponent, FVector(80.0f, 15.0f, 150.0f), FRotator(0.0f, -90.0f, 0.0f), FVector(0.7f, 0.7f, 0.7f));

	ElevatorExternalSwitch = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ElevatorExternalSwitch"));
	ElevatorExternalSwitch->SetStaticMesh(Helpers::C_LoadObjectFromPath<UStaticMesh>(TEXT("/Game/ShootingGame/Asset/Prop/StaticMesh/Elevator_Switch.Elevator_Switch")));
	Helpers::SetComponent<UStaticMeshComponent>(&ElevatorExternalSwitch, RootComponent, FVector(80.0f, -15.0f, 150.0f), FRotator(0.0f, 90.0f, 0.0f), FVector(0.7f, 0.7f, 0.7f));

	ElevatorDoor = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ElevatorDoor"));
	ElevatorDoor->SetStaticMesh(Helpers::C_LoadObjectFromPath<UStaticMesh>(TEXT("/Game/ShootingGame/Asset/Prop/StaticMesh/Elevator_Door.Elevator_Door")));
	Helpers::SetComponent<UStaticMeshComponent>(&ElevatorDoor, RootComponent, FVector(140.0f, 0.0f, 0.0f), FRotator(0.0f, 0.0f, 0.0f));

	ElevatorCeiling = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ElevatorCeiling"));
	ElevatorCeiling->SetStaticMesh(Helpers::C_LoadObjectFromPath<UStaticMesh>(TEXT("/Game/ShootingGame/Asset/Architecture/StaticMesh/Floor_400x400.Floor_400x400")));
	Helpers::SetComponent<UStaticMeshComponent>(&ElevatorCeiling, RootComponent, FVector(0.0f, 10.0f, 420.0f), FRotator(0.0f, 0.0f, 0.0f));

	ElevatorLight = CreateDefaultSubobject<UPointLightComponent>(TEXT("ElevatorLight"));
	ElevatorLight->SetupAttachment(RootComponent);
	ElevatorLight->SetRelativeLocation(FVector(200.0f, 200.0f, 380.0f));
	ElevatorLight->SetIntensity(8.0f);
	ElevatorLight->SetAttenuationRadius(1000.0f);
	ElevatorLight->SetMobility(EComponentMobility::Movable);
}

void AElevator::BeginPlay()
{
	Super::BeginPlay();

	InteractPoint += ElevatorExternalSwitch->GetRelativeLocation();	// 상호작용 위치를 외부 스위치 앞으로 설정
	InteractForward = -ElevatorExternalSwitch->GetForwardVector();

	HeightStamp = GetActorLocation().Z;
}

void AElevator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector playerLocal = UKismetMathLibrary::InverseTransformLocation(GetTransform(), Player->GetActorLocation());	// 플레이어 위치를 엘레베이터의 로컬 좌표계로 변환

	if (!bInside && playerLocal.Y > 10.0f)	// 내부로 들어옴
	{
		bInside = true;
		InteractPoint = GetActorLocation() + ElevatorInternalSwitch->GetRelativeLocation();	// 상호작용 위치를 내부 스위치 앞으로 전환
		InteractForward = -InteractForward;
	}
	else if(bInside && playerLocal.Y < -10.0f)	// 외부로 나감
	{
		bInside = false;
		InteractPoint = GetActorLocation() + ElevatorExternalSwitch->GetRelativeLocation();
		InteractForward = -InteractForward;
	}

	if (bElevating)	// 엘레베이터가 상승중
	{
		if (HeightStamp + Distance > GetActorLocation().Z)	// 아직 도착하지 않음
		{
			AddActorWorldOffset(FVector(0.0f, 0.0f, 1.0f));
		}
		else
		{
			bElevating = false;
			bElevated = true;
			HeightStamp = GetActorLocation().Z;

			bOpening = true;	// 문을 엶
		}
	}
	else if (bDropping)	// 엘레베이터가 하강중
	{
		if (HeightStamp - Distance < GetActorLocation().Z)
		{
			AddActorWorldOffset(FVector(0.0f, 0.0f, -1.0f));
		}
		else
		{
			bDropping = false;
			bElevated = false;
			HeightStamp = GetActorLocation().Z;

			bOpening = true;
		}
	}
	else if (bOpening)	// 엘레베이터 문을 여는중
	{
		if (ElevatorDoor->GetRelativeLocation().X > 10.0f)
		{
			ElevatorDoor->AddRelativeLocation(FVector(-1.0f, 0.0f, 0.0f));
		}
		else if (OpenedTime < 3.0f)
		{
			OpenedTime += DeltaTime;	// 열리는 시간을 측정
		}
		else
		{
			OpenedTime = 0.0f;
			bOpening = false;
			bClosing = true;	// 문을 닫음
		}
	}
	else if (bClosing)	// 엘레베이터 문을 닫는중
	{
		if (ElevatorDoor->GetRelativeLocation().X < 140.0f)
		{
			ElevatorDoor->AddRelativeLocation(FVector(1.0f, 0.0f, 0.0f));
		}
		else
		{
			bClosing = false;
			bWait = false;
		}
	}

	if (bInteractable)
	{
		if (bInside)
		{
			ElevatorInternalSwitch->SetRenderCustomDepth(true);
			ElevatorExternalSwitch->SetRenderCustomDepth(false);
			ElevatorInternalSwitch->SetCustomDepthStencilValue(0);
		}
		else
		{
			ElevatorExternalSwitch->SetRenderCustomDepth(true);
			ElevatorInternalSwitch->SetRenderCustomDepth(false);
			ElevatorExternalSwitch->SetCustomDepthStencilValue(0);
		}
	}
	else
	{
		ElevatorExternalSwitch->SetRenderCustomDepth(false);
		ElevatorInternalSwitch->SetRenderCustomDepth(false);
	}

}

void AElevator::Interact()
{
	if (!bInside)
	{
		bWait = true;
		bOpening = true;
	}
	else // Elevator 내부에선 문이 완전히 닫힌 상태에서만 조작이 가능
	{
		if (bElevated)
		{
			bWait = true;
			bDropping = true;
		}
		else
		{
			bWait = true;
			bElevating = true;
		}
	}
}

