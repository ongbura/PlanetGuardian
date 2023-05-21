// Copyright (c) 2023 Ongbular(Park Joo Hyeong). All rights reserved.


#include "PGIKRig_ConstraintBones.h"

#include "PGLogChannels.h"

#define LOCTEXT_NAMESPACE "UPGIKRig_BoneConstrainer"

void UPGIKRig_ConstraintBones::Initialize(const FIKRigSkeleton& IKRigSkeleton)
{
	for (auto& Elem : BoneConstraints)
	{
		Elem.Bone.BoneIndex = IKRigSkeleton.GetBoneIndexFromName(Elem.Bone.BoneName);
		Elem.TargetBone.BoneIndex = IKRigSkeleton.GetBoneIndexFromName(Elem.TargetBone.BoneName);
	}
}

void UPGIKRig_ConstraintBones::Solve(FIKRigSkeleton& IKRigSkeleton, const FIKRigGoalContainer& Goals)
{
	if (BoneConstraints.Num() == 0)
	{
		return;
	}

	for (const auto& Elem : BoneConstraints)
	{
		if (Elem.Bone.BoneIndex == INDEX_NONE || Elem.TargetBone.BoneIndex == INDEX_NONE)
		{
			UE_LOG(PGIKRig, Warning, TEXT("Bone index is invalid. Bone: %s, TargetBone: %s"), *Elem.Bone.BoneName.ToString(), *Elem.TargetBone.BoneName.ToString());
			continue;
		}
		
		IKRigSkeleton.CurrentPoseGlobal[Elem.Bone.BoneIndex] = IKRigSkeleton.CurrentPoseGlobal[Elem.TargetBone.BoneIndex];
		IKRigSkeleton.PropagateGlobalPoseBelowBone(Elem.Bone.BoneIndex);
	}
}

#if WITH_EDITOR
FText UPGIKRig_ConstraintBones::GetNiceName() const
{
	return { LOCTEXT("SolverName", "Constraint Bones") };
}

bool UPGIKRig_ConstraintBones::IsBoneAffectedBySolver(const FName& BoneName, const FIKRigSkeleton& IKRigSkeleton) const
{
	return true;
}

bool UPGIKRig_ConstraintBones::IsGoalConnected(const FName& GoalName) const
{
	return true;
}

void UPGIKRig_ConstraintBones::AddGoal(const UIKRigEffectorGoal& NewGoal)
{
}

void UPGIKRig_ConstraintBones::RemoveGoal(const FName& GoalName)
{
}

void UPGIKRig_ConstraintBones::RenameGoal(const FName& OldName, const FName& NewName)
{
}

void UPGIKRig_ConstraintBones::SetGoalBone(const FName& GoalName, const FName& NewBoneName)
{
}
#endif

#undef LOCTEXT_NAMESPACE
