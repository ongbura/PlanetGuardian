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

    $nativeGameplayTagsHMembers += "PLANETGUARDIAN_API UE_DECLARE_GAMEPLAY_TAG_EXTERN($memberName);"
    $nativeGameplayTagsCPPAddTags += "UE_DEFINE_GAMEPLAY_TAG_COMMENT($memberName, `"$tagName`", `"$tagDevComment`");"
}

$nativeGameplayTagsHContent = @"
// Copyright (c) 2023 Ongbular (Park Joo-Hyeong). All rights reserved.

#pragma once

#include "NativeGameplayTags.h"

namespace PGGameplayTags
{
    $($nativeGameplayTagsHMembers -join "`r`n`t")
}
"@

$nativeGameplayTagsCPPContent = @"
// Copyright (c) 2023 Ongbular (Park Joo-Hyeong). All rights reserved.

#include "PGNativeGameplayTags.h"

namespace PGGameplayTags
{
    $($nativeGameplayTagsCPPAddTags -join "`r`n`t")
}
"@

Set-Content -Path "..\Source\PlanetGuardian\PGNativeGameplayTags.h" -Value $nativeGameplayTagsHContent
Set-Content -Path "..\Source\PlanetGuardian\PGNativeGameplayTags.cpp" -Value $nativeGameplayTagsCPPContent
