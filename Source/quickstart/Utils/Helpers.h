#pragma once

#include "CoreMinimal.h"
#include "Structs.h"
#include "NPCUtil.h"
#include "../Data/DataTables.h"
#include "UObject/Object.h"

#define ITEM_MESH_PATH TEXT("/Game/ShootingGame/Asset/")
#define ITEM_THUMBNAIL_PATH "/Game/ShootingGame/Image/WidgetImage/"

// ����, ������Ʈ ���� Helper Function ����
class Helpers
{
public:
	// �ش� ���۷����� ����/������Ʈ�� �ε�
	template<typename T>
	static FORCEINLINE T* LoadObjectFromPath(const FName& path)
	{
		if (path == NAME_None) return nullptr;
		T* obj = Cast<T>(StaticLoadObject(UObject::StaticClass(), NULL, *path.ToString()));
		if (!obj) UE_LOG(LogTemp, Log, TEXT("No object in path %s"), *path.ToString());

		return obj;
	}

	// ������Ʈ�� ��ġ, ȸ��, ũ��, ����ġ��Ʈ ����
	template <typename T>
	static FORCEINLINE void SetComponent(T** comp, USceneComponent* parent, FVector relLoc, FRotator relRot, FVector relScale = FVector(1.0f, 1.0f, 1.0f))
	{
		if(parent) (*comp)->SetupAttachment(parent);
		(*comp)->SetRelativeLocation(relLoc);
		(*comp)->SetRelativeRotation(relRot);
		(*comp)->SetRelativeScale3D(relScale);
	}

	// ������ ������ ���� �ε�
	template <typename T>
	static FORCEINLINE T* C_LoadObjectFromPath(const FName& path)
	{
		ConstructorHelpers::FObjectFinder<T> finder(*path.ToString());
		if (finder.Succeeded()) return finder.Object;
		else
		{
			UE_LOG(LogTemp, Log, TEXT("No object in path %s"), *path.ToString());
			return nullptr;
		}
	}

	// ������ �޽ø� �ε�
	static FORCEINLINE FString GetMeshFromName(FString name)
	{
		FString path = "/Game/ShootingGame/Asset/Item/StaticMesh/" + name + "." + name;
		return path;
	}
	// �Ϲ� ����� �ε�
	static FORCEINLINE FString GetNormalThumbnailFromName(FString name)
	{
		FString normalpath = "/Game/ShootingGame/Image/WidgetImage/Normal/" + name + "_Normal." + name + "_Normal";
		return normalpath;
	}
	// Hovered ����� �ε�
	static FORCEINLINE FString GetHoveredThumbnailFromName(FString name)
	{
		FString path = "/Game/ShootingGame/Image/WidgetImage/Hovered/" + name + "_Hovered." + name + "_Hovered";
		return path;
	}
	// ���� ����� �ε�
	static FORCEINLINE FString GetSelectedThumbnailFromName(FString name)
	{
		FString path = "/Game/ShootingGame/Image/WidgetImage/Selected/" + name + "_Selected." + name + "_Selected";
		return path;
	}

	// �������� Ÿ�԰� �̸��� ���� ������ ���
	static FString FindInfo(UDataTable* data, ETypeTag type, FString name, int& out_code)
	{
		if (type == ETypeTag::Cloth || type == ETypeTag::Item)
		{
			FItemTableRow* row = data->FindRow<FItemTableRow>(*name, "");
			if (row)
			{
				return row->Info;
			}
			else return "Invalid Item, Check data table";
		}
		else if (type == ETypeTag::Weapon)
		{
			FWeaponTableRow* row = data->FindRow<FWeaponTableRow>(*name, "");
			if (row)
			{
				out_code = row->Code;
				return row->Info;
			}
			else return "Invalid Item, Check data table";
		}
		else return "Invaild Type";
	}

	// ����� ������ Rich Text�� ���
	static FORCEINLINE FString GetRichText(FString str, FString header)
	{
		return FString("<" + header + ">" + str + "</>");
	}
};

class SaveHelpers
{
public:
	// ������������ ������ ����� ����ü�� ����
	static FORCEINLINE void SaveNPCShopItems(TArray<FShopItemForm> itemlist, TArray<FShopItemShortForm>& saver)
	{
		saver.Empty();
		for (int i = 0; i < itemlist.Num(); i++)
		{
			FShopItemShortForm shortform;
			shortform.ItemInfo = itemlist[i].ItemForm.ShortForm;
			shortform.Price = itemlist[i].Price;
			saver.Add(shortform);
		}
	}
};