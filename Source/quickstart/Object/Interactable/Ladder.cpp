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
		LadderMesh->SetRenderCustomDepth(true);	// ��ȣ�ۿ� ������ �� �ܰ����� ǥ�� (Custom Depth ������ �ܰ��� ���͸����� �׸�)
		LadderMesh->SetCustomDepthStencilValue(0);
	}
	else
	{
		LadderMesh->SetRenderCustomDepth(false);
	}
}

void ALadder::Interact()
{
	FVector middlePoint = GetActorLocation() - 0.5f * Width * Left + FVector(0.0f, 0.0f, 88.0f);	// ��ٸ��� �߾�
	FVector forcePoint = middlePoint + 30.0f * InteractForward;	// �÷��̾ ��ٸ��� �����ϴ� ��ġ
	FVector Look = middlePoint - forcePoint;
	Look.Normalize();
	float YawDeg = UKismetMathLibrary::Acos(FVector::DotProduct(Look, Player->GetActorForwardVector()));
	float Yaw = UKismetMathLibrary::RadiansToDegrees(YawDeg);

	// �÷��̾ ��ٸ� ���� �̵�
	Player->SetActorLocation(forcePoint);
	Player->GetController()->SetControlRotation(UKismetMathLibrary::MakeRotFromX(-InteractForward));

	// �÷��̾�� ���� Ÿ���ִ� ��ٸ��� ���� ����
	Player->LadderInfo.onLadder = true;
	Player->LadderInfo.Slope = Slope;
	Player->LadderInfo.Width = Width;
	Player->LadderInfo.bDirty = true;
}