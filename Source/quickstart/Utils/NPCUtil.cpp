#include "NPCUtil.h"

bool FSingleQuest::CheckCompletion()
{
	Completed = true;
	switch (Type)
	{
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
	default:
		break;
	}
	return Completed;
}

FQuestDialogueLine FQuest::GetStartLine()
{
	switch (Progress)
	{
	case EQuestProgress::Unavailable:
		return DialogueTree_Unavailable.GetStartLine();
	case EQuestProgress::Available:
		return DialogueTree_Available.GetStartLine();
	case EQuestProgress::InProgress:
		return DialogueTree_InProgress.GetStartLine();
	case EQuestProgress::Finished:
		return DialogueTree_Finished.GetStartLine();
	default:
		return FQuestDialogueLine();
	}
}

bool FQuest::EndSingleTask()
{
	if (Type == EQuestType::Serial)
	{
		currPhase++;
		if (currPhase >= SubQuests.Num())
		{
			Progress = EQuestProgress::Finished;
			return true;
		}
		else return false;
	}
	else
	{
		Remains--;
		if (Remains <= 0)
		{
			Progress = EQuestProgress::Finished;
			return true;
		}
		else return false;
	}
}

void FQuest::UndoSingleTask() // Undo the completion. Especially in Item collecting quest.(When you throw away the quest items)
{
	if (Type == EQuestType::Serial)
	{
		currPhase--;
		Progress = EQuestProgress::InProgress;
	}
	else
	{
		Remains++;
		Progress = EQuestProgress::InProgress;
	}
}