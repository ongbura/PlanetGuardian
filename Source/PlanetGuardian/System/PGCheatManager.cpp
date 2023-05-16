// Copyright (c) 2023 Ongbular(Park Joo Hyeong). All rights reserved.


#include "PGCheatManager.h"

#include "Character/Guardian/PGGuardian.h"
#include "Character/Guardian/Animation/PGGuardianAnimInstance.h"

void UPGCheatManager::PGDebug(const FName DebugType) const
{
	const auto* PC = GetPlayerController();
	if (PC == nullptr)
	{
		return;
	}
	
	if (DebugType == TEXT("Guardian_Animation"))
	{
		const auto* Guardian = Cast<APGGuardian>(PC->GetPawn());
		if (Guardian == nullptr)
		{
			return;
		}

		auto* AnimInstance = Cast<UPGGuardianAnimInstance>(Guardian->GetMesh()->GetAnimInstance());
		if (AnimInstance == nullptr)
		{
			return;
		}

		AnimInstance->ToggleShowDebug();
	}
}
