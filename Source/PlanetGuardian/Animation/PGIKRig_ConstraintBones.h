// Copyright (c) 2023 Ongbular(Park Joo Hyeong). All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "IKRigSolver.h"

#include "PGIKRig_ConstraintBones.generated.h"


USTRUCT(BlueprintType)
struct FPGBoneConstraint
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	FBoneReference Bone;

	UPROPERTY(EditAnywhere)
	FBoneReference TargetBone;
};

/**
 * 
 */
UCLASS()
class PLANETGUARDIAN_API UPGIKRig_ConstraintBones : public UIKRigSolver
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, meta=(AllowPrivateAccess="true"))
	TArray<FPGBoneConstraint> BoneConstraints;

protected:
	virtual void Initialize(const FIKRigSkeleton& IKRigSkeleton) override;
	
	virtual void Solve(FIKRigSkeleton& IKRigSkeleton, const FIKRigGoalContainer& Goals) override;

#if WITH_EDITOR
	virtual FText GetNiceName() const override;

	virtual bool IsBoneAffectedBySolver(const FName& BoneName, const FIKRigSkeleton& IKRigSkeleton) const override;

	virtual bool IsGoalConnected(const FName& GoalName) const override;

	virtual void AddGoal(const UIKRigEffectorGoal& NewGoal) override;

	virtual void RemoveGoal(const FName& GoalName) override;

	virtual void RenameGoal(const FName& OldName, const FName& NewName) override;

	virtual void SetGoalBone(const FName& GoalName, const FName& NewBoneName) override;
#endif
};
