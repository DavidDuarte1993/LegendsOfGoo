using UnrealBuildTool;
using System.Collections.Generic;

public class MonsterKingodTESTEditorTarget : TargetRules
{
	public MonsterKingodTESTEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;

		ExtraModuleNames.AddRange( new string[] { "MonsterKingodTEST" } );
	}
}
