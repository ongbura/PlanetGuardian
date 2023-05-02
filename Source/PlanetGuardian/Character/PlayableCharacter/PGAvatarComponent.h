// Copyright (c) 2023 Ongbular(Park Joo Hyeong). All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "PGPawnAssistantComponent.h"
#include "Controller/PlayerController/PGPlayerController.h"
#include "PGAvatarComponent.generated.h"

class UGameplayEffect;
class UPGGameplayAbility;
class UPGAbilitySystemComponent;

UCLASS(meta=(BlueprintSpawnableComponent))
class PLANETGUARDIAN_API UPGAvatarComponent : public UPGPawnAssistantComponent
{
	GENERATED_BODY()

	DECLARE_EVENT_OneParam(UPGAvatarComponent, FOnAbilitySystemInitialized, UPGAbilitySystemComponent* /* InASC */)
	DECLARE_EVENT(UPGAvatarComponent, FOnAbilitySystemUninitalized)
	DECLARE_EVENT_OneParam(UPGAvatarComponent, FOnControllerAssigned, APGPlayerController* /* InController */)

public:
	FOnAbilitySystemInitialized OnAbilitySystemInitialized;
	FOnAbilitySystemUninitalized OnAbilitySystemUninitialized;
	FOnControllerAssigned OnPlayerControllerAssigned;

private:	
	bool bControllerAssigned;
	bool bPlayerStateAssigned;
	bool bSpawnedCompletely;
	
	TWeakObjectPtr<UPGAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(AllowPrivateAccess="true"))
	TArray<TSubclassOf<UPGGameplayAbility>> DefaultAbilities;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(AllowPrivateAccess="true"))
	TArray<TSubclassOf<UGameplayEffect>> StartupEffects;

public:
	UPGAvatarComponent();

	UPGAbilitySystemComponent* GetPGAbilitySystemComponent() const { return AbilitySystemComponent.Get(); }

	void InitializeAbilitySystem(UPGAbilitySystemComponent* InASC, AActor* InOwnerActor);

	void UninitializeAbilitySystem();
	
	void HandlePlayerControllerAssigned();

	void HandlePlayerStateAssigned();

	UFUNCTION(Server, Reliable)
	void GrantDefaultAbilitiesAndApplyStartupEffects();
	
protected:
	virtual void BeginPlay() override;
	
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:
	void GrantDefaultAbilities();

	void ApplyStartupEffects();	
};
