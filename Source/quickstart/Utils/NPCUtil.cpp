#include "NPCUtil.h"

bool FSingleQuest::CheckCompletion()
{
	Completed = true;
	switch (Type)	// 획득/처치 수가 할당량에 모자르면 미완료 처리
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
		if (NPCName == Instigator) return DialogueTree_InProgress.GetStartLine();	// 완료할 수 있는 상태지만, Instigator에게 말을 걸면 완료할 수 없기 때문에 InProgress 다이얼로그를 반환
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
		if (CurrPhase >= SubQuests.Num())	// 모든 서브퀘스트를 완료
		{
			Progress = EQuestProgress::Finished;
			return true;
		}
		else return false;
	}
	else
	{
		Remains--;
		if (Remains <= 0)	// 모든 서브퀘스트를 완료
		{
			Progress = EQuestProgress::Finished;
			return true;
		}
		else return false;
	}
}

void FQuest::UndoSingleTask()
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