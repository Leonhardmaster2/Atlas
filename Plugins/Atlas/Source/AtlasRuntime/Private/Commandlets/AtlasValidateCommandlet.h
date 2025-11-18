// Copyright Atlas Team. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Commandlets/Commandlet.h"
#include "AtlasValidateCommandlet.generated.h"

/**
 * Commandlet for running Atlas validation from CI/CD pipelines
 * Usage: -run=AtlasValidateCommandlet
 * Returns: 0 if OK, 1 if warnings, 2 if errors
 */
UCLASS()
class UAtlasValidateCommandlet : public UCommandlet
{
	GENERATED_BODY()

public:
	UAtlasValidateCommandlet();

	//~ Begin UCommandlet Interface
	virtual int32 Main(const FString& Params) override;
	//~ End UCommandlet Interface
};
