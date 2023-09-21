// Data Tables
// ������ ������ ��� Data Table ���
#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "../Utils/Structs.h"
#include "DataTables.generated.h"

// ���� ���� ������ ������ ��� Row
USTRUCT(BlueprintType)
struct FItemTableRow : public FTableRowBase
{
	GENERATED_BODY()

public:
	// �������� Ÿ��
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	ETypeTag Type;
	// �������� ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	FString Info;
};

// ���� �������� ������ ��� Row
USTRUCT(BlueprintType)
struct FWeaponTableRow : public FTableRowBase
{
	GENERATED_BODY()

public:
	// �������� Ÿ��
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	ETypeTag Type;
	// �������� ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	FString Info;
	// ���� �ڵ�
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	int32 Code;
};
