// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class StealthCPP : ModuleRules
{
	public StealthCPP(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] {
			"Core",
			"CoreUObject",
			"Engine",
			"InputCore",
			"EnhancedInput",
			"AIModule",
			"StateTreeModule",
			"GameplayStateTreeModule",
			"UMG",
            "MotionWarping",
            "AIModule",
            "GameplayTasks"
		});

		PrivateDependencyModuleNames.AddRange(new string[] { });

		PublicIncludePaths.AddRange(new string[] {
			"StealthCPP",
			"StealthCPP/Variant_Platforming",
			"StealthCPP/Variant_Combat",
			"StealthCPP/Variant_Combat/AI",
			"StealthCPP/Variant_SideScrolling",
			"StealthCPP/Variant_SideScrolling/Gameplay",
			"StealthCPP/Variant_SideScrolling/AI"
		});

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
