using UnrealBuildTool;
using System.Collections.Generic;

public class MonsterKingodTESTTarget : TargetRules
{
	public MonsterKingodTESTTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;

		ExtraModuleNames.AddRange( new string[] { "MonsterKingodTEST" } );
	}
}
