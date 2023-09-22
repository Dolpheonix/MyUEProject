// 자주 쓰이는 구조체 모음
#pragma once

#include "CoreMinimal.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/DataTable.h"
#include "Structs.generated.h"

// 아이템 타입
UENUM(BlueprintType)
enum class ETypeTag : uint8
{
	Weapon,
	Item,
	Cloth,
};

// 아이템 정보를 담는 구조체. 에디터 내에서 수정, 게임 저장/로드 시 사용
USTRUCT(Atomic, BlueprintType)
struct FItemShortForm
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere)
	ETypeTag TypeTag;			// 타입
	UPROPERTY(EditAnywhere)
	FString NameTag = "";		// 이름
	UPROPERTY(VisibleAnywhere)	
	FString InfoTag = "";		// 정보
	UPROPERTY(BlueprintReadOnly)
	int32 Code = 0;				// 무기의 코드
	UPROPERTY(EditAnywhere)
	int32 Num = 1;				// 보유 개수
	UPROPERTY(EditAnywhere)
	bool bIsSellable;			// 판매 가능한가?

	FItemShortForm() : TypeTag(ETypeTag::Cloth), NameTag(""), InfoTag(""), Code(0), Num(1), bIsSellable(true) {}
	FItemShortForm(ETypeTag type, FString name) : TypeTag(type), NameTag(name), InfoTag(""), Code(0), Num(1), bIsSellable(true) {}
};

// 아이템 정보와 썸네일 이미지를 가지고 있어, 게임 플레이 중 직접 사용하는 아이템 구조체
USTRUCT(Atomic, BlueprintType)
struct FItemForm
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(BlueprintReadOnly)
	UStaticMeshComponent* MeshComponent = nullptr;	// 사용할 매시

	UPROPERTY(BlueprintReadOnly)
	UTexture2D* Thumbnail_N = nullptr;				// 일반 썸네일
	UPROPERTY(BlueprintReadOnly)		
	UTexture2D* Thumbnail_H = nullptr;				// Hovered 썸네일
	UPROPERTY(BlueprintReadOnly)
	UTexture2D* Thumbnail_S = nullptr;				// 선택 썸네일

	UPROPERTY(BlueprintReadOnly)
	FItemShortForm ShortForm;

	FItemForm() : MeshComponent(nullptr), Thumbnail_N(nullptr), Thumbnail_H(nullptr), Thumbnail_S(nullptr), ShortForm(FItemShortForm()) {}
	FItemForm(FItemShortForm sf) : MeshComponent(nullptr), Thumbnail_N(nullptr), Thumbnail_H(nullptr), Thumbnail_S(nullptr), ShortForm(sf) {}
};

// TArray는 2차원 배열이 불가능해서 한번 둘러싸는 형태로 만든 구조체
USTRUCT(BlueprintType)
struct FWrappedItemForm
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(BlueprintReadOnly)
		TArray<FItemForm> ItemForms;
};