using System.IO;
using Tools.DotNETCommon;
using UnrealBuildTool;

public class TweakIt : ModuleRules
{
	public TweakIt(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
		var factoryGamePchPath = new DirectoryReference(Path.Combine(Target.ProjectFile.Directory.ToString(), "Source",
			"FactoryGame", "Public", "FactoryGame.h"));
		PrivatePCHHeaderFile = factoryGamePchPath.MakeRelativeTo(new DirectoryReference(ModuleDirectory));

		PublicDependencyModuleNames.AddRange(new[]
		{
			"Core", "CoreUObject",
			"Engine",
			"InputCore",
			"OnlineSubsystem", "OnlineSubsystemUtils", "OnlineSubsystemNULL",
			"SignificanceManager",
			"PhysX", "APEX", "PhysXVehicles", "ApexDestruction",
			"AkAudio",
			"ReplicationGraph",
			"UMG",
			"AIModule",
			"NavigationSystem",
			"AssetRegistry",
			"GameplayTasks",
			"AnimGraphRuntime",
			"Slate", "SlateCore",
			"Json"
		});


		if (Target.Type == TargetRules.TargetType.Editor)
			PublicDependencyModuleNames.AddRange(new[] { "OnlineBlueprintSupport", "AnimGraph" });

		PublicDependencyModuleNames.AddRange(new[] { "FactoryGame", "SML", "Projects" });
		PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "Lua/lib/lua54.lib"));
	}
}