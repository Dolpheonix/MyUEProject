// Character - Character Root
// 캐릭터 클래스들의 베이스 클래스
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Character_Root.generated.h"

UCLASS()
class QUICKSTART_API ACharacter_Root : public ACharacter
{
	GENERATED_BODY()

public:
	ACharacter_Root();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	// Hurt 시에 호출되는 함수
	UFUNCTION()
	virtual void OnHurt();
	// 죽었을 때 호출되는 함수
	UFUNCTION()
	virtual void OnDead();

public:
	// 게임에서 표시되는 이름
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Name")
	FString DisplayName;
	// 체력
	UPROPERTY(BlueprintReadWrite, Category = "Status")
	float HP = 100.0f;
	// 죽었는가?
	UPROPERTY(BlueprintReadWrite, Category = "Interaction")
	bool bDead = false;
	// 역장 등의 외부 힘에 영향을 받는 중인가?
	UPROPERTY(BlueprintReadWrite)
	bool bForced = false;
	// Hurt 시의 경직 시간을 표시하는 타이머
	float HurtTimer = -1.0f;
};
