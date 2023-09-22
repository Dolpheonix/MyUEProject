#include "Lever.h"

ALever::ALever()
{
	LeverCase = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LeverCase"));
	LeverCase->SetStaticMesh(Helpers::C_LoadObjectFromPath<UStaticMesh>(TEXT("/Game/ShootingGame/Asset/Prop/StaticMesh/Lever_Case.Lever_Case")));
	LeverCase->SetupAttachment(RootComponent);

	LeverBody = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LeverBody"));
	LeverBody->SetStaticMesh(Helpers::C_LoadObjectFromPath<UStaticMesh>(TEXT("/Game/ShootingGame/Asset/Prop/StaticMesh/Lever_Body.Lever_Body")));
	Helpers::SetComponent(&LeverBody, RootComponent, FVector(10.0f, 0.0f, 10.0f), FRotator(0.0f, 0.0f, 0.0f), FVector(1.5f, 1.0f, 1.0f));
}

void ALever::BeginPlay()
{
	Super::BeginPlay();

	LeverBody->SetRelativeRotation(FRotator(40.0f, 0.0f, 0.0f));
	InteractForward = GetActorForwardVector();
}

void ALever::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bLeverDown)
	{
		if (LeverBody->GetRelativeRotation().Pitch <= -40.0f)
		{
			bLeverDown = false;	// ������ �� ������ ==> �ٽ� �ö󰡾� ��
		}
		else
		{
			LeverBody->AddRelativeRotation(FRotator(-1.0f, 0.0f, 0.0f));	// ������ ����
		}
	}
	else if (bWait)
	{
		if (LeverBody->GetRelativeRotation().Pitch >= 40.0f)
		{
			bWait = false;	// ������ �������� �ö�� ===> �ٽ� ��ȣ�ۿ��� �غ� �Ϸ�
		}
		else
		{
			LeverBody->AddRelativeRotation(FRotator(1.0f, 0.0f, 0.0f));	// ������ �ø�
		}
	}

	if (bInteractable)
	{
		// ������ ���͸��� ǥ��
		LeverBody->SetRenderCustomDepth(true);
		LeverCase->SetRenderCustomDepth(true);
		LeverBody->SetCustomDepthStencilValue(0);
		LeverCase->SetCustomDepthStencilValue(0);
	}
	else
	{
		LeverBody->SetRenderCustomDepth(false);
		LeverCase->SetRenderCustomDepth(false);
	}
}

void ALever::Interact()
{
	bLeverDown = true;	// ������ ������ ����
	bWait = true;		// �ٽ� �ö�� ������ ��ٷ��� ��

	Lever_On();
	OnLeverTriggered.Broadcast();
}