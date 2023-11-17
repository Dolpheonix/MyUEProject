#include "Interactable.h"
#include "Components/ArrowComponent.h"
#include "../../Utils/Helpers.h"
#include "../../Core/GameMode/MainGameMode.h"
#include "Blueprint/WidgetTree.h"
#include <Kismet/GameplayStatics.h>

AInteractable::AInteractable()
{
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	// ��ȣ�ۿ� ���� ���� �ʱ�ȭ
	InteractPoint = FVector::ZeroVector;
	InteractForward = GetActorForwardVector();
	InteractRadius = 200.0f;
	InteractRange = 90.0f;
	
	// ���� �ð�ȭ
	FVector axis = InteractForward.RotateAngleAxis(90.0f, FVector(1.0f, 0.0f, 0.0f));
	FRotator forward = InteractForward.Rotation();
	FRotator range_L = InteractForward.RotateAngleAxis(InteractRange / 2, axis).Rotation();
	FRotator range_R = InteractForward.RotateAngleAxis(-InteractRange / 2, axis).Rotation();

	InteractionForwardArrow = CreateDefaultSubobject<UArrowComponent>(TEXT("ForwardArrow"));
	InteractionRangeArrow_L = CreateDefaultSubobject<UArrowComponent>(TEXT("RangeArrow_L"));
	InteractionRangeArrow_R = CreateDefaultSubobject<UArrowComponent>(TEXT("RangeArrow_R"));

	Helpers::SetComponent<UArrowComponent>(&InteractionForwardArrow, RootComponent, InteractPoint, forward);
	Helpers::SetComponent<UArrowComponent>(&InteractionRangeArrow_L, RootComponent, InteractPoint, range_L);
	Helpers::SetComponent<UArrowComponent>(&InteractionRangeArrow_R, RootComponent, InteractPoint, range_R);

	InteractionForwardArrow->ArrowColor = FColor::Black;
	InteractionRangeArrow_L->ArrowColor = FColor::Blue;
	InteractionRangeArrow_R->ArrowColor = FColor::Blue;

	InteractionForwardArrow->ArrowLength = InteractRadius;
	InteractionRangeArrow_L->ArrowLength = InteractRadius;
	InteractionRangeArrow_R->ArrowLength = InteractRadius;
}

void AInteractable::BeginPlay()
{
	Super::BeginPlay();

	Player = (AMainCharacter*)UGameplayStatics::GetPlayerPawn(this, 0);

	InteractPoint += GetActorLocation();	// ���� �����̽��� ������ �̵�
}

void AInteractable::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector P = Player->GetActorLocation();
	FVector BP = P - InteractPoint;
	float dist = BP.Size();
	BP.Normalize();
	FVector FP = Player->GetActorForwardVector();

	if (bActive && dist < InteractRadius && FMath::Acos(FVector::DotProduct(BP, InteractForward)) < (InteractRange / 2)) // ��ȣ�ۿ��� ����
	{
		if (!bInteractable) // ó�� ������ ���Դٸ�
		{
			Player->InteractionFlag++;
			bInteractable = true;
		}

		if (Player->GetCurrentAction() == ECustomActionMode::INTERACT) // �÷��̾ ��ȣ�ۿ��� �ɾ��ٸ�
		{
			Interact();
		}
	}
	else if(bInteractable)	// �÷��̾ �������� ��������
	{
		Player->InteractionFlag--;
		bInteractable = false;
	}
}
#if WITH_EDITOR
void AInteractable::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	FString Name = PropertyChangedEvent.Property->GetName();
	if (Name == TEXT("X") || Name == TEXT("Y") || Name == TEXT("Z"))	// ���� ������Ƽ�� �����
	{
		FString VectorName = PropertyChangedEvent.MemberProperty->GetName();
		if (VectorName == TEXT("InteractPoint"))	// ���� ���뿡 �°� Arrow Component ����
		{
			InteractionForwardArrow->SetRelativeLocation(InteractPoint);
			InteractionRangeArrow_L->SetRelativeLocation(InteractPoint);
			InteractionRangeArrow_R->SetRelativeLocation(InteractPoint);
		}
		else if (VectorName == TEXT("InteractForward"))	// ���� ���뿡 �°� Arrow Component ����
		{
			InteractForward.Normalize();
			FVector right = InteractForward.RotateAngleAxis(90.0f, FVector(0.0f, 0.0f, 1.0f));
			FVector axis = FVector::CrossProduct(InteractForward, right);

			FRotator forward = InteractForward.Rotation();
			FRotator range_L = InteractForward.RotateAngleAxis(InteractRange / 2, axis).Rotation();
			FRotator range_R = InteractForward.RotateAngleAxis(-InteractRange / 2, axis).Rotation();

			InteractionForwardArrow->SetRelativeRotation(forward);
			InteractionRangeArrow_L->SetRelativeRotation(range_L);
			InteractionRangeArrow_R->SetRelativeRotation(range_R);
		}
	}
	else if (Name == TEXT("InteractRange"))	// �ٲ� �������� �°� Arrow Component ����
	{
		FVector right = InteractForward.RotateAngleAxis(90.0f, FVector(0.0f, 0.0f, 1.0f));
		FVector axis = FVector::CrossProduct(InteractForward, right);
		FRotator forward = InteractForward.Rotation();
		FRotator range_L = InteractForward.RotateAngleAxis(InteractRange / 2, axis).Rotation();
		FRotator range_R = InteractForward.RotateAngleAxis(-InteractRange / 2, axis).Rotation();

		InteractionRangeArrow_L->SetRelativeRotation(range_L);
		InteractionRangeArrow_R->SetRelativeRotation(range_R);
	}
	else if (Name == TEXT("InteractRadius"))	// �ٲ� �ݰ濡 �°� Arrow Component ����
	{
		InteractionForwardArrow->ArrowLength = InteractRadius;
		InteractionRangeArrow_L->ArrowLength = InteractRadius;
		InteractionRangeArrow_R->ArrowLength = InteractRadius;

		InteractionForwardArrow->MarkRenderStateDirty();
		InteractionRangeArrow_L->MarkRenderStateDirty();
		InteractionRangeArrow_R->MarkRenderStateDirty();
	}
}
#endif