// Copyright Simone Campitelli 2019
#pragma once

// ~ UE4 Versioning
#include "Runtime/Launch/Resources/Version.h"
#include "Modules/ModuleManager.h"

class FEFMModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};