// Fill out your copyright notice in the Description page of Project Settings.


#include "BTDecorator_SetSpeed.h"
#include "AIController.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

UBTDecorator_SetSpeed::UBTDecorator_SetSpeed()
{
	bNotifyActivation = true;
}

void UBTDecorator_SetSpeed::OnNodeActivation(FBehaviorTreeSearchData& SearchData)
{
	auto Controller = SearchData.OwnerComp.GetAIOwner();
	if (Controller)
	{
		auto owner = Controller->GetCharacter();
		auto movement = owner->GetCharacterMovement();
		movement->MaxWalkSpeed = Speed;
	}
}
