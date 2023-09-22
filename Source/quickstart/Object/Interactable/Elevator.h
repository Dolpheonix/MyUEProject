// Interactable - Elavator
// 승강기 오브젝트
// 문 여닫이 버튼, 상승/하강 버튼 구현
#pragma once

#include "CoreMinimal.h"
#include "Interactable.h"
#include "Components/PointLightComponent.h"
#include "Elevator.generated.h"

UCLASS()
class QUICKSTART_API AElevator : public AInteractable
{
	GENERATED_BODY()
	
public:	
	AElevator();

protected:
	virtual void BeginPlay() override;
	virtual void Interact() override;

public:	
	virtual void Tick(float DeltaTime) override;
	// 엘레베이터 몸체
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* ElevatorBody;
	// 엘레베이터 내부의 버튼 (승강/하강 버튼)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* ElevatorInternalSwitch;
	// 엘레베이터 외부의 버튼 (문 여닫이 버튼)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* ElevatorExternalSwitch;
	// 엘레베이터 문 메시
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* ElevatorDoor;
	// 엘레베이터 천장 메시
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* ElevatorCeiling;
	// 엘레베이터 조명
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UPointLightComponent* ElevatorLight;
	// 엘레베이터의 상승/하강 거리
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Distance;
	// 현재 윗층에 위치해있는가? (상승/하강 여부 판단용)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bElevated;
	// 상승 시작점
	float HeightStamp;
	// 플레이어가 내부에 위치해있는가?
	bool bInside = false;
	// 엘레베이터가 상승중인가?
	bool bElevating;
	// 엘레베이터가 하강중인가?
	bool bDropping;
	// 엘레베이터 문이 열리고 있는가?
	bool bOpening;
	// 엘레베이터 문이 닫히고 있는가?
	bool bClosing;
	// 
	bool bGetin;
	// 
	bool bGetout;
	// 
	float OpenedTime;
};
