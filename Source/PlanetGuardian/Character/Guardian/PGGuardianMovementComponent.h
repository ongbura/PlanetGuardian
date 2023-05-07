// Copyright (c) 2023 Ongbular(Park Joo Hyeong). All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "PGGuardianMovementComponent.generated.h"


UCLASS(meta=(BlueprintSpawnableComponent))
class PLANETGUARDIAN_API UPGGuardianMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()

	UPROPERTY(Category="Character Movement: Walking", EditAnywhere, BlueprintReadWrite, meta=(ClampMin="0", UIMin="0", ForceUnits="cm/s"), meta=(AllowPrivateAccess="true"))
	float MaxSprintSpeed { 1000.f };

	bool bWantToSprint { false };

	bool bToggledSprint { false };
	
public:
	UPGGuardianMovementComponent();

	void Sprint();

	bool CanSprint() const;

	void StopSprinting();

protected:
	virtual FNetworkPredictionData_Client* GetPredictionData_Client() const override;

	virtual void UpdateFromCompressedFlags(uint8 Flags) override;

	virtual void OnMovementUpdated(float DeltaSeconds, const FVector& OldLocation, const FVector& OldVelocity) override;

public:
	void SetWantToSprint(const bool bNewWantToSprint) { bWantToSprint = bNewWantToSprint; }
	bool WantsToSprint() const { return bWantToSprint; }
};

class FSavedMove_Guardian final : public FSavedMove_Character
{
	uint32 bWantsToSprint : 1;

public:
	FSavedMove_Guardian();

protected:
	virtual bool CanCombineWith(const FSavedMovePtr& NewMove, ACharacter* InCharacter, float MaxDelta) const override;

	virtual void Clear() override;

	virtual uint8 GetCompressedFlags() const override;

	virtual void SetMoveFor(ACharacter* C, float InDeltaTime, FVector const& NewAccel, FNetworkPredictionData_Client_Character& ClientData) override;

	virtual void PrepMoveFor(ACharacter* C) override;
};

class FNetworkPredictionData_Client_Guardian final : public FNetworkPredictionData_Client_Character
{
public:
	explicit FNetworkPredictionData_Client_Guardian(const UCharacterMovementComponent& ClientMovement);

	virtual FSavedMovePtr AllocateNewMove() override;
};
