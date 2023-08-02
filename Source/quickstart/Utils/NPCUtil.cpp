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

FQuestDialogueLine FQuest::GetStartLine(FString NPCName)
{
	switch (Progress)
	{
	case EQuestProgress::Unavailable:
	{
		if(NPCName == Instigator) return DialogueTree_Unavailable.GetStartLine();
		break;
	}
	case EQuestProgress::Available:
	{
		if(NPCName == Instigator) return DialogueTree_Available.GetStartLine();
		break;
	}
	case EQuestProgress::InProgress:
	{
		if(NPCName == Instigator) return DialogueTree_InProgress.GetStartLine();
		break;
	}
	case EQuestProgress::Finished:
	{
		if (NPCName == Ender) return DialogueTree_Finished.GetStartLine();
		if (NPCName == Instigator) return DialogueTree_InProgress.GetStartLine();
		break;
	}
	default:
		return FQuestDialogueLine();
	}

	UE_LOG(LogTemp, Log, TEXT("Invalid quest dialogue request"));
	return FQuestDialogueLine();
}

bool FQuest::EndSingleTask()
{
	if (Type == EQuestType::Serial)
	{
		CurrPhase++;
		if (CurrPhase >= SubQuests.Num())
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
		CurrPhase--;
		Progress = EQuestProgress::InProgress;
	}
	else
	{
		Remains++;
		Progress = EQuestProgress::InProgress;
	}
}