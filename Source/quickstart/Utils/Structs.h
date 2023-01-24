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