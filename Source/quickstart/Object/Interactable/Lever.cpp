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
			bLeverDown = false;	// 레버가 다 내려감 ==> 다시 올라가야 함
		}
		else
		{
			LeverBody->AddRelativeRotation(FRotator(-1.0f, 0.0f, 0.0f));	// 레버를 내림
		}
	}
	else if (bWait)
	{
		if (LeverBody->GetRelativeRotation().Pitch >= 40.0f)
		{
			bWait = false;	// 레버가 내려갔다 올라옴 ===> 다시 상호작용할 준비 완료
		}
		else
		{
			LeverBody->AddRelativeRotation(FRotator(1.0f, 0.0f, 0.0f));	// 레버를 올림
		}
	}

	if (bInteractable)
	{
		// 윤곽선 머터리얼 표시
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
	bLeverDown = true;	// 레버가 내려간 상태
	bWait = true;		// 다시 올라올 때까지 기다려야 함

	Lever_On();
	OnLeverTriggered.Broadcast();
}