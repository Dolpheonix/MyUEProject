#include "Ladder.h"
#include <Kismet/KismetMathLibrary.h>


ALadder::ALadder()
{
	PrimaryActorTick.bCanEverTick = true;

	LadderMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LadderMesh"));
	LadderMesh->SetStaticMesh(Helpers::C_LoadObjectFromPath<UStaticMesh>(TEXT("/Game/ShootingGame/Asset/Prop/StaticMesh/Ladder.Ladder")));
	LadderMesh->SetupAttachment(RootComponent);
}

void ALadder::BeginPlay()
{
	Super::BeginPlay();

	InteractForward = GetActorRightVector();

	Left = GetActorForwardVector();

	Slope = GetActorRotation().Roll + 75.0f;

	Width = GetActorScale3D().X * 50.0f;
}

void ALadder::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bInteractable)
	{
		LadderMesh->SetRenderCustomDepth(true);	// 상호작용 가능할 시 외곽선을 표시 (Custom Depth 맵으로 외곽선 머터리얼을 그림)
		LadderMesh->SetCustomDepthStencilValue(0);
	}
	else
	{
		LadderMesh->SetRenderCustomDepth(false);
	}
}

void ALadder::Interact()
{
	FVector middlePoint = GetActorLocation() - 0.5f * Width * Left + FVector(0.0f, 0.0f, 88.0f);	// 사다리의 중앙
	FVector forcePoint = middlePoint + 30.0f * InteractForward;	// 플레이어가 사다리에 안착하는 위치
	FVector Look = middlePoint - forcePoint;
	Look.Normalize();
	float YawDeg = UKismetMathLibrary::Acos(FVector::DotProduct(Look, Player->GetActorForwardVector()));
	float Yaw = UKismetMathLibrary::RadiansToDegrees(YawDeg);

	// 플레이어를 사다리 위로 이동
	Player->SetActorLocation(forcePoint);
	Player->GetController()->SetControlRotation(UKismetMathLibrary::MakeRotFromX(-InteractForward));

	// 플레이어에게 현재 타고있는 사다리의 정보 전달
	Player->LadderInfo.onLadder = true;
	Player->LadderInfo.Slope = Slope;
	Player->LadderInfo.Width = Width;
	Player->LadderInfo.bDirty = true;
}