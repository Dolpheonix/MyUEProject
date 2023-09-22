// UI - Preview Actor
// �κ��丮�� ǥ���� ������ ����
// ���� �Ⱥ��̴� ������ ��ġ����, �κ��丮 UI���� Main Character�� ������ ���� �̹����� ������
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../Character/Main/MainCharacter.h"
#include "Components/SceneCaptureComponent2D.h"
#include "Engine/TextureRenderTarget2D.h"
#include "PreviewActor.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FShowroomEvent);

UCLASS()
class QUICKSTART_API APreviewActor : public AActor
{
	GENERATED_BODY()
	
public:	
	APreviewActor();

protected:
	virtual void BeginPlay() override;

public:	
	// Main Character�� ������ ������ ������ ���Ϳ� ����
	UFUNCTION()
	void Sync_to_Character();
	// ������ ���� ��, ������ ���Ϳ� ����
	UFUNCTION()
	void Delete_and_Sync(ETypeTag type, FString tag);
	// ����ȭ �̺�Ʈ
	FShowroomEvent SyncEvent;



public:
	// ���� �޽�
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Mesh")
	USkeletalMeshComponent* MainMesh;
	// ������� ī�޶�
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Camera")
	USceneCaptureComponent2D* PreviewCamera;
	// ���� ���õ� ����
	UPROPERTY(BlueprintReadWrite)
	int Weapon_Now;
	// ���� ���õ� ������ �ڵ�
	UPROPERTY(BlueprintReadOnly)
	int32 WeaponCode;
	// ���� ���õ� �ǻ�
	int Cloth_Now;
	// ������ ���Ͱ� ǥ���� ���� ���
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Test")
	TMap<FString, UStaticMeshComponent*> PreviewWeapons;
	// ������ ���Ͱ� ǥ���� �ǻ� ���
	TMap<FString, UStaticMeshComponent*> PreviewClothes;

	// ����ȭ�� Main Character
	AMainCharacter* Player;
};
