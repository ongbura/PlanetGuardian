# ReflectNativeGameplayTags.ps1

$csvData = Import-Csv -Path "..\Config\NativeGameplayTags.csv"

$sortedData = $csvData | Sort-Object TagName

$sortedData | Export-Csv -Path "..\Config\NativeGameplayTags.csv" -NoTypeInformation

$nativeGameplayTagsHMembers = @()
$nativeGameplayTagsCPPAddTags = @()

$previousHierarchy = ""

foreach ($row in $sortedData) {
    $tagName = $row.TagName
    $tagDevComment = $row.TagDevComment
    $memberName = $tagName.Replace(".", "_")

    $currentHierarchy = ($tagName -split "\." | Select-Object -First 2) -join "."

    if ($currentHierarchy -ne $previousHierarchy) {
        if ($previousHierarchy) {
            $nativeGameplayTagsHMembers += ""
            $nativeGameplayTagsCPPAddTags += ""
        }
        $nativeGameplayTagsHMembers += "// $currentHierarchy"
        $nativeGameplayTagsCPPAddTags += "// $currentHierarchy"
        $previousHierarchy = $currentHierarchy
    }

    $nativeGameplayTagsHMembers += "FGameplayTag $memberName{};"
    
    if ($tagDevComment) {
        $nativeGameplayTagsCPPAddTags += "$memberName = GameplayTagsManager.AddNativeGameplayTag(`"$tagName`", `"$tagDevComment`");"
    } else {
        $nativeGameplayTagsCPPAddTags += "$memberName = GameplayTagsManager.AddNativeGameplayTag(`"$tagName`");"
    }
}

$nativeGameplayTagsHContent = @"
// Copyright (c) 2023 Ongbular (Park Joo-Hyeong). All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"

static struct FPGNativeGameplayTags : FGameplayTagNativeAdder
{
    $($nativeGameplayTagsHMembers -join "`r`n`t")

    virtual void AddTags() override;
} GTags;
"@

$nativeGameplayTagsCPPContent = @"
// Copyright (c) 2023 Ongbular (Park Joo-Hyeong). All rights reserved.

#include "PGNativeGameplayTags.h"
#include "NativeGameplayTags.h"

void FPGNativeGameplayTags::AddTags()
{
    auto& GameplayTagsManager = UGameplayTagsManager::Get();

    $($nativeGameplayTagsCPPAddTags -join "`r`n`t")

    GameplayTagsManager.OnLastChanceToAddNativeTags().RemoveAll(this);
}
"@

Set-Content -Path "..\Source\PlanetGuardian\PGNativeGameplayTags.h" -Value $nativeGameplayTagsHContent
Set-Content -Path "..\Source\PlanetGuardian\PGNativeGameplayTags.cpp" -Value $nativeGameplayTagsCPPContent
