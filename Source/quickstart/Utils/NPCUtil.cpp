#include "NPCUtil.h"

bool FSingleQuest::CheckCompletion()
{
	Completed = true;
	switch (Type)
	{
	case ESingleQuestType::Arrival:
		break;
	case ESingleQuestType::Hunt:
		for (int i = 0; i < HuntingLists.Num(); i++)
		{
			if (currAmounts[i] < HuntingLists[i].HuntAmount)
			{
				Completed = false;
				break;
			}
		}
		break;
	case ESingleQuestType::Item:
		for (int i = 0; i < ItemLists.Num(); i++)
		{
			if (currAmounts[i] < ItemLists[i].ItemAmount)
			{
				Completed = false;
				break;
			}
		}
		break;
	case ESingleQuestType::Action:
		break;
	default:
		break;
	}
	return Completed;
}