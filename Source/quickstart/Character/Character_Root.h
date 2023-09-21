// Character - Character Root
// ĳ���� Ŭ�������� ���̽� Ŭ����
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

	// Hurt �ÿ� ȣ��Ǵ� �Լ�
	UFUNCTION()
	virtual void OnHurt();
	// �׾��� �� ȣ��Ǵ� �Լ�
	UFUNCTION()
	virtual void OnDead();

public:
	// ���ӿ��� ǥ�õǴ� �̸�
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Name")
	FString DisplayName;
	// ü��
	UPROPERTY(BlueprintReadWrite, Category = "Status")
	float HP = 100.0f;
	// �׾��°�?
	UPROPERTY(BlueprintReadWrite, Category = "Interaction")
	bool bDead = false;
	// ���� ���� �ܺ� ���� ������ �޴� ���ΰ�?
	UPROPERTY(BlueprintReadWrite)
	bool bForced = false;
	// Hurt ���� ���� �ð��� ǥ���ϴ� Ÿ�̸�
	float HurtTimer = -1.0f;
};
