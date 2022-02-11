#pragma once

#include "CoreMinimal.h"
#include "Commandlets/Commandlet.h"
#include "DiffPatchBaseCommandlet.h"

#include "MakeBinaryPatchCommandlet.generated.h"

UCLASS()
class UMakeBinaryPatchCommandlet : public UDiffPatchBaseCommandlet
{
	GENERATED_BODY()

public:
	UMakeBinaryPatchCommandlet();

	virtual int32 Main(const FString& InCommandline) override;
};
