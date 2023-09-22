// ���� ���̴� ����ü ����
#pragma once

#include "CoreMinimal.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/DataTable.h"
#include "Structs.generated.h"

// ������ Ÿ��
UENUM(BlueprintType)
enum class ETypeTag : uint8
{
	Weapon,
	Item,
	Cloth,
};

// ������ ������ ��� ����ü. ������ ������ ����, ���� ����/�ε� �� ���
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

// ������ ������ ����� �̹����� ������ �־�, ���� �÷��� �� ���� ����ϴ� ������ ����ü
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

// TArray�� 2���� �迭�� �Ұ����ؼ� �ѹ� �ѷ��δ� ���·� ���� ����ü
USTRUCT(BlueprintType)
struct FWrappedItemForm
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(BlueprintReadOnly)
		TArray<FItemForm> ItemForms;
};