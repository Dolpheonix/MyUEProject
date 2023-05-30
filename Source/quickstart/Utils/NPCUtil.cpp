#include "NPCUtil.h"

bool FSingleQuest::CheckCompletion()
{
	bool completed = true;
	switch (Type)
	{
	case ESingleQuestType::Arrival:
		break;
	case ESingleQuestType::Hunt:
		for (int i = 0; i < Huntees.Num(); i++)
		{
			if (currAmounts[i] < HuntAmounts[i])
			{
				completed = false;
				break;
			}
		}
		break;
	case ESingleQuestType::Item:
		for (int i = 0; i < ItemNames.Num(); i++)
		{
			if (currAmounts[i] < ItemAmounts[i])
			{
				completed = false;
				break;
			}
		}
		break;
	case ESingleQuestType::Action:
		break;
	default:
		break;
	}
	return completed;
}