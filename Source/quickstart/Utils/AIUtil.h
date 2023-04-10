#pragma once

#include "CoreMinimal.h"
#include "NavigationSystem.h"
#include "NavigationData.h"

class AIHelper
{
public:
	static bool isValidPath(UWorld* World, FVector start, FVector end, ANavigationData* NavData = nullptr)
	{
		auto NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(World);

		if (!NavSys) return false;

		if (!NavData)
		{
			NavData = NavSys->GetDefaultNavDataInstance(FNavigationSystem::DontCreate);
		}

		FPathFindingQuery q;
		q.StartLocation = start;
		q.EndLocation = end;
		q.NavData = NavData;

		if (!q.NavData->IsA<ANavigationData>()) return false;

		FPathFindingResult r = NavSys->FindPathSync(q);
		return r.IsSuccessful();
	}
};