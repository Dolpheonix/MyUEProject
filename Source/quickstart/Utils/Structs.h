#pragma once

#include "CoreMinimal.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/DataTable.h"
#include "Structs.generated.h"

USTRUCT(Atomic, BlueprintType)
struct FItemForm
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(BlueprintReadOnly)
	UStaticMeshComponent* MeshComponent = nullptr;

	UPROPERTY(BlueprintReadOnly)
	UTexture2D* Thumbnail_N = nullptr;
	UPROPERTY(BlueprintReadOnly)
	UTexture2D* Thumbnail_H = nullptr;
	UPROPERTY(BlueprintReadOnly)
	UTexture2D* Thumbnail_S = nullptr;

	UPROPERTY(BlueprintReadOnly)
	FString NameTag = "";
	UPROPERTY(BlueprintReadOnly)
	FString ItemInfo = "";
	UPROPERTY(BlueprintReadOnly)
	int32 Code=0;

	FItemForm() : MeshComponent(nullptr), Thumbnail_N(nullptr), Thumbnail_H(nullptr), Thumbnail_S(nullptr), NameTag(""), ItemInfo(""), Code(0) {}
	FItemForm(FString name, FString info) : MeshComponent(nullptr), Thumbnail_N(nullptr), Thumbnail_H(nullptr), Thumbnail_S(nullptr), NameTag(name), ItemInfo(info), Code(0) {}
};

UENUM(BlueprintType)
enum class ETypeTag : uint8
{
	Weapon,
	Item,
	Cloth,
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