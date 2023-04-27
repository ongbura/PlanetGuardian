// Copyright (c) 2023 Ongbular(Park Joo Hyeong). All rights reserved.


#include "PGGuardianController.h"

APGGuardianController::APGGuardianController()
{
}

// void APGGuardianController::Jump(const FInputActionValue& Value)
// {
// 	RETURN_PAWN_IF_NULL();
//
// 	if (auto* Guardian = Cast<APGGuardian>(GetPawn()))
// 	{
// 		switch (Guardian->GetCharacterMovement()->MovementMode)
// 		{
// 		case MOVE_Walking:
// 			[[fallthrough]];
// 		case MOVE_NavWalking:
// 			Guardian->Jump();
// 			break;
// 		case MOVE_Falling:
// 			[[fallthrough]];
// 		case MOVE_Flying:
// 			Guardian->ToggleJetpack(false, true);
// 			break;
// 		default:
// 			checkNoEntry();
// 		}
// 		Guardian->Jump();
// 	}
// }
//
// void APGGuardianController::StopJumping(const FInputActionValue& Value)
// {
// 	RETURN_PAWN_IF_NULL();
//
// 	if (auto* Guardian = Cast<APGGuardian>(GetPawn()))
// 	{
// 		Guardian->StopJumping();
// 		Guardian->ToggleJetpack(false, false);
// 	}
// }

