#pragma once
#include "command/ChatCommandLibrary.h"
#include "TIRunScriptCommand.generated.h"

UCLASS()
class ATIRunScriptCommand : public AChatCommandInstance
{
	GENERATED_BODY()
public:
	ATIRunScriptCommand();
	virtual EExecutionStatus ExecuteCommand_Implementation(
		UCommandSender* Sender,
		const TArray<FString>& Arguments,
		const FString& Label
	) override;
};
