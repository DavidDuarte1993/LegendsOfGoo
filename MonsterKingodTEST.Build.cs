using UnrealBuildTool;

public class MonsterKingodTEST : ModuleRules
{
	public MonsterKingodTEST(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine",
            "InputCore", "AIModule" });


        PrivateDependencyModuleNames.AddRange(new string[] {  });
	}
}
