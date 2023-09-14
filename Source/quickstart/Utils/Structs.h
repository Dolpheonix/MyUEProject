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
	ETypeTag TypeTag;			// Ÿ��
	UPROPERTY(EditAnywhere)
	FString NameTag = "";		// �̸�
	UPROPERTY(VisibleAnywhere)	
	FString InfoTag = "";		// ����
	UPROPERTY(BlueprintReadOnly)
	int32 Code = 0;				// ������ �ڵ�
	UPROPERTY(EditAnywhere)
	int32 Num = 1;				// ���� ����
	UPROPERTY(EditAnywhere)
	bool bIsSellable;			// �Ǹ� �����Ѱ�?

	FItemShortForm() : TypeTag(ETypeTag::Cloth), NameTag(""), InfoTag(""), Code(0), Num(1), bIsSellable(true) {}
	FItemShortForm(ETypeTag type, FString name) : TypeTag(type), NameTag(name), InfoTag(""), Code(0), Num(1), bIsSellable(true) {}
};


USTRUCT(Atomic, BlueprintType)
struct FItemForm
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(BlueprintReadOnly)
	UStaticMeshComponent* MeshComponent = nullptr;	// ����� �Ž�

	UPROPERTY(BlueprintReadOnly)
	UTexture2D* Thumbnail_N = nullptr;				// �Ϲ� �����
	UPROPERTY(BlueprintReadOnly)		
	UTexture2D* Thumbnail_H = nullptr;				// Hovered �����
	UPROPERTY(BlueprintReadOnly)
	UTexture2D* Thumbnail_S = nullptr;				// ���� �����

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