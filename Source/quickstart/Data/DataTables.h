// Data Tables
// 아이템 정보를 담는 Data Table 양식
#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "../Utils/Structs.h"
#include "DataTables.generated.h"

// 무기 외의 아이템 정보를 담는 Row
USTRUCT(BlueprintType)
struct FItemTableRow : public FTableRowBase
{
	GENERATED_BODY()

public:
	// 아이템의 타입
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	ETypeTag Type;
	// 아이템의 설명
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	FString Info;
};

// 무기 아이템의 정보를 담는 Row
USTRUCT(BlueprintType)
struct FWeaponTableRow : public FTableRowBase
{
	GENERATED_BODY()

public:
	// 아이템의 타입
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	ETypeTag Type;
	// 아이템의 설명
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	FString Info;
	// 무기 코드
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	int32 Code;
};
