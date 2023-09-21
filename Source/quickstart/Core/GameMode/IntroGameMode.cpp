#include "IntroGameMode.h"

AIntroGameMode::AIntroGameMode()
{

}

void AIntroGameMode::BeginPlay()
{
	Super::BeginPlay();
	IntroUI = CreateWidget(GetWorld(), IntroWidgetClass);

	IntroUI->AddToViewport();	// ��Ʈ�� UI ȭ�鿡 �߰� (��Ʈ�δ� �� �ʿ� UI ȭ�鸸 ������)
}

void AIntroGameMode::EndPlay(EEndPlayReason::Type EndPlayReason)
{
	IntroUI->RemoveFromViewport();	// ���� ���� �÷��� ���� ���� ����

	Super::EndPlay(EndPlayReason);
}

