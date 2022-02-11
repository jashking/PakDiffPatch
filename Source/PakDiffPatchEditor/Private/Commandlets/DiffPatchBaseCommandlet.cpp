#include "DiffPatchBaseCommandlet.h"

#include "HAL/PlatformFile.h"

#include "PakDiffPatchEditorLog.h"

#define SRC_PAK TEXT("SrcPak")
#define DEST_PAK TEXT("DestPak")
#define SRC_AES_KEY TEXT("SrcAesKey")
#define DEST_AES_KEY TEXT("DestAesKey")

UDiffPatchBaseCommandlet::UDiffPatchBaseCommandlet()
{

}

int32 UDiffPatchBaseCommandlet::Main(const FString& InCommandline)
{
	ParseCommandLine(*InCommandline, Tokens, Switches);

	for (int32 SwitchIdx = Switches.Num() - 1; SwitchIdx >= 0; --SwitchIdx)
	{
		FString& Switch = Switches[SwitchIdx];
		TArray<FString> SplitSwitch;
		if (2 == Switch.ParseIntoArray(SplitSwitch, TEXT("="), true))
		{
			Params.Add(SplitSwitch[0].ToUpper(), SplitSwitch[1].TrimQuotes());
			Switches.RemoveAt(SwitchIdx);
		}
	}

	FString ErrorMessage;
	if (!ValidParams({ SRC_PAK, DEST_PAK }, ErrorMessage))
	{
		UE_LOG(LogPakDiffPatchEditor, Error, TEXT("%s"), *ErrorMessage);
		return -1;
	}

	const FString SrcPakPath = GetParamAsString(SRC_PAK);
	const FString DestPakPath = GetParamAsString(DEST_PAK);
	const FString SrcAESKey = GetParamAsString(SRC_AES_KEY);
	const FString DestAESKey = GetParamAsString(DEST_AES_KEY);

	SrcPakFile = LoadPakFile(SrcPakPath, SrcAESKey);
	DestPakFile = LoadPakFile(DestPakPath, DestAESKey);

	return (SrcPakFile.IsValid() && DestPakFile.IsValid()) ? 0 : -1;
}

bool UDiffPatchBaseCommandlet::HasSwitch(const FString& InSwitch) const
{
	const FString* Switch = Switches.FindByPredicate([&InSwitch](const FString& InValue) {
		return InSwitch.Equals(InValue, ESearchCase::IgnoreCase);
		});

	return (Switch != nullptr);
}

bool UDiffPatchBaseCommandlet::HasParam(const FString& InParamName) const
{
	const FString* Value = Params.Find(InParamName.ToUpper());
	return (Value != nullptr);
}

FString UDiffPatchBaseCommandlet::GetParamAsString(const FString& InParamName, const FString& InDefaultValue /*= TEXT("")*/) const
{
	const FString* Value = Params.Find(InParamName.ToUpper());
	return Value ? *Value : InDefaultValue;
}

int32 UDiffPatchBaseCommandlet::GetParamAsInt(const FString& InParamName, int32 InDefaultValue /*= 0*/) const
{
	const FString* Value = Params.Find(InParamName.ToUpper());
	return Value ? FCString::Atoi(**Value) : InDefaultValue;
}

bool UDiffPatchBaseCommandlet::ValidParams(const TArray<FString>& InParamArray, FString& ErrorMessage)
{
	bool bValid = true;
	ErrorMessage = TEXT("");

	TArray<FString> MissingParams;
	for (const FString& ParamName : InParamArray)
	{
		if (!HasParam(ParamName))
		{
			MissingParams.Add(FString::Printf(TEXT("-%s"), *ParamName));
			bValid = false;
		}
	}

	if (!bValid)
	{
		ErrorMessage = FString::Printf(TEXT("Commandline invalid! Missing params in command line[%s]!"), *FString::Join(MissingParams, TEXT(" ")));
	}

	return bValid;
}

FPakFilePtr UDiffPatchBaseCommandlet::LoadPakFile(const FString& InPakPath, const FString& InAESKey)
{
	if (InPakPath.IsEmpty())
	{
		UE_LOG(LogPakDiffPatchEditor, Error, TEXT("Load pak file failed! Pak path is empty!"));
		return nullptr;
	}

	UE_LOG(LogPakDiffPatchEditor, Log, TEXT("Start load pak file: %s."), *InPakPath);

	IPlatformFile& PlatformFile = IPlatformFile::GetPlatformPhysical();
	if (!PlatformFile.FileExists(*InPakPath))
	{
		UE_LOG(LogPakDiffPatchEditor, Error, TEXT("Load pak file failed! Pak file not exists! Path: %s."), *InPakPath);
		return nullptr;
	}

	FPakFilePtr PakFile = new FPakFile(&PlatformFile, *InPakPath, false);
	if (!PakFile.IsValid())
	{
		UE_LOG(LogPakDiffPatchEditor, Error, TEXT("Load pak file failed! Create PakFile failed! Path: %s."), *InPakPath);
		return nullptr;
	}

	return PakFile;
}
