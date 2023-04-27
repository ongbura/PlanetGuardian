// Copyright (c) 2023 Ongbular(Park Joo Hyeong). All rights reserved.

#pragma once

#include "CoreMinimal.h"

#include "PGPawnAssistantComponent.generated.h"


UCLASS(meta=(BlueprintSpawnableComponent))
class PLANETGUARDIAN_API UPGPawnAssistantComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	/** Gets the pawn that owns the component, this will always be valid during gameplay but can return null in the editor */
	template <class T>
	T* GetPawn() const
	{
		static_assert(TPointerIsConvertibleFromTo<T, APawn>::Value, "'T' template parameter to GetPawn must be derived from APawn");
		return Cast<T>(GetOwner());
	}

	template <class T>
	T* GetPawnChecked() const
	{
		static_assert(TPointerIsConvertibleFromTo<T, APawn>::Value, "'T' template parameter to GetPawnChecked must be derived from APawn");
		return CastChecked<T>(GetOwner());
	}

	//////////////////////////////////////////////////////////////////////////////
	// Pawn accessors, only valid if called during gameplay
	//////////////////////////////////////////////////////////////////////////////

	/** Gets the player state that owns the component, this can return null on clients for player pawns that are still being replicated */
	template <class T>
	T* GetPlayerState() const
	{
		static_assert(TPointerIsConvertibleFromTo<T, APlayerState>::Value, "'T' template parameter to GetPlayerState must be derived from APlayerState");
		return GetPawnChecked<APawn>()->GetPlayerState<T>();
	}

	/** Gets the controller that owns the component, this will usually be null on clients */
	template <class T>
	T* GetController() const
	{
		static_assert(TPointerIsConvertibleFromTo<T, AController>::Value, "'T' template parameter to GetController must be derived from AController");
		return GetPawnChecked<APawn>()->GetController<T>();
	}
};
