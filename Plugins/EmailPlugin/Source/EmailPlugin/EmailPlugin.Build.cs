// Copyright 2019 DownToCode. All Rights Reserved.

using UnrealBuildTool;
using System.IO;

public class EmailPlugin : ModuleRules
{
    private string ModulePath
    {
        get { return ModuleDirectory; }
    }

    private string PluginPath
    {
        get { return Path.Combine(PluginDirectory, "Source/"); }
    }

    private string ThirdPartyPath
    {
        get { return Path.GetFullPath(Path.Combine(PluginPath, "EmailPlugin/ThirdParty/")); }
    }

    private string ThirdPartyIncludePath
    {
        get { return Path.GetFullPath(Path.Combine(PluginPath, "ThirdParty/include")); }
    }

    public EmailPlugin(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

        bEnableExceptions = true;
        PublicDefinitions.Add("CURL_STATICLIB");

        PublicIncludePaths.AddRange(
            new string[] {
                Path.Combine(PluginPath, "EmailPlugin/Public"),
                ThirdPartyIncludePath,
			}
            );


        PrivateIncludePaths.AddRange(
            new string[] {
                "EmailPlugin/Private",
			}
            );


        PublicDependencyModuleNames.AddRange(
            new string[]
            {
                "Core",
			}
        );


        PrivateDependencyModuleNames.AddRange(
            new string[]
            {
                "CoreUObject",
                "Engine",
                "Core",
                "OpenSSL"
            }
        );

        if (Target.Platform == UnrealTargetPlatform.Win64)
        {
            PublicDefinitions.Add("_WINSOCK_DEPRECATED_NO_WARNINGS");
            PublicAdditionalLibraries.Add(ThirdPartyPath + "lib/Windows/CSmtp.lib");
        }
        else if (Target.Platform == UnrealTargetPlatform.Android)
        {
            PublicAdditionalLibraries.Add("stdc++");
             
            AdditionalPropertiesForReceipt.Add("AndroidPlugin", Path.Combine(PluginPath, "EmailPlugin/EmailPlugin_APL.xml"));
            PublicAdditionalLibraries.Add(ThirdPartyPath + "lib/Android/arm64-v8a/libcSmtp.so");
        }
    }
}

