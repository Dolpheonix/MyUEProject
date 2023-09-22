// Interactable - Elavator
// �°��� ������Ʈ
// �� ������ ��ư, ���/�ϰ� ��ư ����
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
	// ���������� ��ü
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* ElevatorBody;
	// ���������� ������ ��ư (�°�/�ϰ� ��ư)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* ElevatorInternalSwitch;
	// ���������� �ܺ��� ��ư (�� ������ ��ư)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* ElevatorExternalSwitch;
	// ���������� �� �޽�
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* ElevatorDoor;
	// ���������� õ�� �޽�
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* ElevatorCeiling;
	// ���������� ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UPointLightComponent* ElevatorLight;
	// ������������ ���/�ϰ� �Ÿ�
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Distance;
	// ���� ������ ��ġ���ִ°�? (���/�ϰ� ���� �Ǵܿ�)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bElevated;
	// ��� ������
	float HeightStamp;
	// �÷��̾ ���ο� ��ġ���ִ°�?
	bool bInside = false;
	// ���������Ͱ� ������ΰ�?
	bool bElevating;
	// ���������Ͱ� �ϰ����ΰ�?
	bool bDropping;
	// ���������� ���� ������ �ִ°�?
	bool bOpening;
	// ���������� ���� ������ �ִ°�?
	bool bClosing;
	// 
	bool bGetin;
	// 
	bool bGetout;
	// 
	float OpenedTime;
};
