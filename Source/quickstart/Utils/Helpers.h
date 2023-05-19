#pragma once

#include "CoreMinimal.h"
#include "Components/StaticMeshComponent.h"
#include "Camera/CameraComponent.h"
#include "Structs.h"
#include "../Data/DataTables.h"
#include "UObject/Object.h"


#define ITEM_MESH_PATH TEXT("/Game/ShootingGame/Asset/")
#define ITEM_THUMBNAIL_PATH "/Game/ShootingGame/Image/WidgetImage/"

class Helpers
{
public:
	// Runtime Object Loader
	template<typename T>
	static FORCEINLINE T* LoadObjectFromPath(const FName& path)
	{
		if (path == NAME_None) return nullptr;
		return Cast<T>(StaticLoadObject(UObject::StaticClass(), NULL, *path.ToString()));
	}

	template <typename T>
	static FORCEINLINE void SetComponent(T** comp, USceneComponent* parent, FVector relLoc, FRotator relRot, FVector relScale = FVector(1.0f, 1.0f, 1.0f))
	{
		if(parent) (*comp)->SetupAttachment(parent);
		(*comp)->SetRelativeLocation(relLoc);
		(*comp)->SetRelativeRotation(relRot);
		(*comp)->SetRelativeScale3D(relScale);
	}

	// Constructor
	template <typename T>
	static FORCEINLINE T* C_LoadObjectFromPath(const FName& path)
	{
		ConstructorHelpers::FObjectFinder<T> finder(*path.ToString());
		if (finder.Succeeded()) return finder.Object;
		else return nullptr;
	}

	// WidgetHelpers
	static FORCEINLINE FButtonStyle SetButtonStyle(UTexture2D* NormalTexture, float Hover, float )
	{

	}

	static FORCEINLINE FString GetMeshFromName(FString name)
	{
		FString path = "/Game/ShootingGame/Asset/" + name + "/" + name + "." + name;
		return path;
	}

	static FORCEINLINE FString GetNormalThumbnailFromName(FString name)
	{
		FString normalpath = "/Game/ShootingGame/Image/WidgetImage/Normal/" + name + "_Normal." + name + "_Normal";
		return normalpath;
	}

	static FORCEINLINE FString GetHoveredThumbnailFromName(FString name)
	{
		FString path = "/Game/ShootingGame/Image/WidgetImage/Hovered/" + name + "_Hovered." + name + "_Hovered";
		return path;
	}

	static FORCEINLINE FString GetSelectedThumbnailFromName(FString name)
	{
		FString path = "/Game/ShootingGame/Image/WidgetImage/Selected/" + name + "_Selected." + name + "_Selected";
		return path;
	}

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
};