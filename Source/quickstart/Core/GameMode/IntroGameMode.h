///////////////////Intro Game Mode//////////////
// 게임을 불러오기 전까지의 게임 모드
// 1. 인트로 위젯 관리

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
	// 인트로 위젯
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widget")
	TSubclassOf<UIntro> IntroWidgetClass;
	// 알림 위젯
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widget")
	TSubclassOf<UNotify> NotifyWidgetClass;

	UUserWidget* IntroUI;
};
