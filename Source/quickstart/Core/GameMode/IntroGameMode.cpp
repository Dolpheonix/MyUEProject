#include "IntroGameMode.h"

AIntroGameMode::AIntroGameMode()
{

}

void AIntroGameMode::BeginPlay()
{
	Super::BeginPlay();
	IntroUI = CreateWidget(GetWorld(), IntroWidgetClass);

	IntroUI->AddToViewport();	// 인트로 UI 화면에 추가 (인트로는 빈 맵에 UI 화면만 존재함)
}

void AIntroGameMode::EndPlay(EEndPlayReason::Type EndPlayReason)
{
	IntroUI->RemoveFromViewport();	// 메인 게임 플레이 전에 위젯 제거

	Super::EndPlay(EndPlayReason);
}

