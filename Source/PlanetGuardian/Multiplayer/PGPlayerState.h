// Copyright (c) 2023 Ongbular(Park Joo Hyeong). All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/PlayerState.h"
#include "PGPlayerState.generated.h"

class UPGAbilitySystemComponent;

UCLASS()
class PLANETGUARDIAN_API APGPlayerState : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category="Ability", meta=(AllowPrivateAccess="true"))
	TObjectPtr<UPGAbilitySystemComponent> AbilitySystem;

public:
	APGPlayerState();

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	UPGAbilitySystemComponent* GetPGAbilitySystemComponent() const { return AbilitySystem;}
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
};
