#include "MakeBinaryPatchCommandlet.h"

#include "PakDiffPatchEditorLog.h"

UMakeBinaryPatchCommandlet::UMakeBinaryPatchCommandlet()
{

}

int32 UMakeBinaryPatchCommandlet::Main(const FString& InCommandline)
{
	const int32 BaseInitResult = Super::Main(InCommandline);
	if (BaseInitResult <= 0)
	{
		return BaseInitResult;
	}

	return 0;
}
