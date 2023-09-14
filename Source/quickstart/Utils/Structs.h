#pragma once

#include "CoreMinimal.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/DataTable.h"
#include "Structs.generated.h"

UENUM(BlueprintType)
enum class ETypeTag : uint8
{
	Weapon,
	Item,
	Cloth,
};

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


USTRUCT(BlueprintType)
struct FWrappedItemForm
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(BlueprintReadOnly)
		TArray<FItemForm> ItemForms;
};

struct CapturedSlot_Info
{
	ETypeTag ItemType;
	int index;
	FString tag;
};

UENUM(BlueprintType)
enum class EShapeCollision : uint8
{
	Box,
	Sphere,
	Capsule,
	Convex,
};

struct AnimationAssetPhases
{
public:
	static constexpr float Fist = 1.0f;
	static constexpr float Fire = 1.1f;
	static constexpr float Wield[4] = { 1.0f, 2.0f, 3.0f, 4.8f };
	static constexpr float Interact = 0.8f;
	static constexpr float Jump = 0.9f;
};