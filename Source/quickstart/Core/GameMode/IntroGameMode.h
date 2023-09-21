///////////////////Intro Game Mode//////////////
// ������ �ҷ����� �������� ���� ���
// 1. ��Ʈ�� ���� ����

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "../../UI/Intro.h"
#include "../../UI/Notify.h"
#include "IntroGameMode.generated.h"

/**
 * 
 */
UCLASS()
class QUICKSTART_API AIntroGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AIntroGameMode();

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(EEndPlayReason::Type EndPlayReason) override;

public:
	// ��Ʈ�� ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widget")
	TSubclassOf<UIntro> IntroWidgetClass;
	// �˸� ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widget")
	TSubclassOf<UNotify> NotifyWidgetClass;

	UUserWidget* IntroUI;
};
