// UI - Preview Actor
// 인벤토리에 표시할 프리뷰 액터
// 맵의 안보이는 공간에 위치시켜, 인벤토리 UI에서 Main Character의 아이템 착용 이미지를 보여줌
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
	// Main Character의 아이템 정보를 프리뷰 액터에 적용
	UFUNCTION()
	void Sync_to_Character();
	// 아이템 삭제 시, 프리뷰 액터에 적용
	UFUNCTION()
	void Delete_and_Sync(ETypeTag type, FString tag);
	// 동기화 이벤트
	FShowroomEvent SyncEvent;



public:
	// 메인 메시
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Mesh")
	USkeletalMeshComponent* MainMesh;
	// 프리뷰용 카메라
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Camera")
	USceneCaptureComponent2D* PreviewCamera;
	// 현재 선택된 무기
	UPROPERTY(BlueprintReadWrite)
	int Weapon_Now;
	// 현재 선택된 무기의 코드
	UPROPERTY(BlueprintReadOnly)
	int32 WeaponCode;
	// 현재 선택된 의상
	int Cloth_Now;
	// 프리뷰 액터가 표시할 무기 목록
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Test")
	TMap<FString, UStaticMeshComponent*> PreviewWeapons;
	// 프리뷰 액터가 표시할 의상 목록
	TMap<FString, UStaticMeshComponent*> PreviewClothes;

	// 동기화된 Main Character
	AMainCharacter* Player;
};
