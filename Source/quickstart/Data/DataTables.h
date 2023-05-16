#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "../Utils/Structs.h"
#include "DataTables.generated.h"

USTRUCT(BlueprintType)
struct FItemTableRow : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	ETypeTag Type;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	FString Info;
};

USTRUCT(BlueprintType)
struct FWeaponTableRow : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	ETypeTag Type;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	FString Info;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	int32 Code;
};
