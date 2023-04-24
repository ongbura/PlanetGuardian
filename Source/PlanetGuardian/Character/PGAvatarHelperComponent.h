// Copyright (c) 2023 Ongbular(Park Joo Hyeong). All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/PawnComponent.h"
#include "PGAvatarHelperComponent.generated.h"

class UPGAbilitySystemComponent;

UCLASS(meta=(BlueprintSpawnableComponent))
class PLANETGUARDIAN_API UPGAvatarHelperComponent : public UPawnComponent
{
	GENERATED_BODY()

	DECLARE_EVENT(UPGAvatarHelperComponent, FOnAbilitySystemInitialized)
	DECLARE_EVENT(UPGAvatarHelperComponent, FOnAbilitySystemUninitalized)

	bool bSetController;
	bool bSetPlayerState;

public:
	FOnAbilitySystemInitialized OnAbilitySystemInitialized;
	FOnAbilitySystemUninitalized OnAbilitySystemUninitialized;

private:
	TWeakObjectPtr<UPGAbilitySystemComponent> AbilitySystem;

public:
	UPGAvatarHelperComponent(const FObjectInitializer& ObjectInitializer);

	void InitializeAbilitySystem(UPGAbilitySystemComponent* InASC, AActor* InOwnerActor);

	void UninitializeAbilitySystem();

	void OnAvatarControllerChanged();

	void OnAvatarPlayerStateChanged();
	
protected:
	virtual void BeginPlay() override;
	
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
};
