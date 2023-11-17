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
	// 상호작용 범위 관련 초기화
	InteractPoint = FVector::ZeroVector;
	InteractForward = GetActorForwardVector();
	InteractRadius = 200.0f;
	InteractRange = 90.0f;
	
	// 범위 시각화
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

	InteractPoint += GetActorLocation();	// 월드 스페이스로 기준점 이동
}

void AInteractable::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector P = Player->GetActorLocation();
	FVector BP = P - InteractPoint;
	float dist = BP.Size();
	BP.Normalize();
	FVector FP = Player->GetActorForwardVector();

	if (bActive && dist < InteractRadius && FMath::Acos(FVector::DotProduct(BP, InteractForward)) < (InteractRange / 2)) // 상호작용이 가능
	{
		if (!bInteractable) // 처음 범위에 들어왔다면
		{
			Player->InteractionFlag++;
			bInteractable = true;
		}

		if (Player->GetCurrentAction() == ECustomActionMode::INTERACT) // 플레이어가 상호작용을 걸었다면
		{
			Interact();
		}
	}
	else if(bInteractable)	// 플레이어가 범위에서 빠져나옴
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
	if (Name == TEXT("X") || Name == TEXT("Y") || Name == TEXT("Z"))	// 벡터 프로퍼티가 변경됨
	{
		FString VectorName = PropertyChangedEvent.MemberProperty->GetName();
		if (VectorName == TEXT("InteractPoint"))	// 변경 내용에 맞게 Arrow Component 변경
		{
			InteractionForwardArrow->SetRelativeLocation(InteractPoint);
			InteractionRangeArrow_L->SetRelativeLocation(InteractPoint);
			InteractionRangeArrow_R->SetRelativeLocation(InteractPoint);
		}
		else if (VectorName == TEXT("InteractForward"))	// 변경 내용에 맞게 Arrow Component 변경
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
	else if (Name == TEXT("InteractRange"))	// 바뀐 범위각에 맞게 Arrow Component 변경
	{
		FVector right = InteractForward.RotateAngleAxis(90.0f, FVector(0.0f, 0.0f, 1.0f));
		FVector axis = FVector::CrossProduct(InteractForward, right);
		FRotator forward = InteractForward.Rotation();
		FRotator range_L = InteractForward.RotateAngleAxis(InteractRange / 2, axis).Rotation();
		FRotator range_R = InteractForward.RotateAngleAxis(-InteractRange / 2, axis).Rotation();

		InteractionRangeArrow_L->SetRelativeRotation(range_L);
		InteractionRangeArrow_R->SetRelativeRotation(range_R);
	}
	else if (Name == TEXT("InteractRadius"))	// 바뀐 반경에 맞게 Arrow Component 변경
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