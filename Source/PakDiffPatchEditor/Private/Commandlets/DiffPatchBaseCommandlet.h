#pragma once

#include "CoreMinimal.h"
#include "Commandlets/Commandlet.h"
#include "IPlatformFilePak.h"
#include "Launch/Resources/Version.h"

#include "DiffPatchBaseCommandlet.generated.h"

#if ENGINE_MAJOR_VERSION >= 5 || ENGINE_MINOR_VERSION >= 27
typedef TRefCountPtr<FPakFile> FPakFilePtr;
#else
typedef TSharedPtr<FPakFile> FPakFilePtr;
#endif // ENGINE_MAJOR_VERSION >= 5 || ENGINE_MINOR_VERSION >= 27

UCLASS()
class UDiffPatchBaseCommandlet : public UCommandlet
{
	GENERATED_BODY()

public:
	UDiffPatchBaseCommandlet();

	virtual int32 Main(const FString& InCommandline) override;

protected:
	bool HasSwitch(const FString& InSwitch) const;
	bool HasParam(const FString& InParamName) const;
	FString GetParamAsString(const FString& InParamName, const FString& InDefaultValue = TEXT("")) const;
	int32 GetParamAsInt(const FString& InParamName, int32 InDefaultValue = 0) const;
	bool ValidParams(const TArray<FString>& InParamArray, FString& ErrorMessage);

	FPakFilePtr LoadPakFile(const FString& InPakPath, const FString& InAESKey);

protected:
	TArray<FString> Tokens;
	TArray<FString> Switches;
	TMap<FString, FString> Params;

	FPakFilePtr SrcPakFile;
	FPakFilePtr DestPakFile;
};
