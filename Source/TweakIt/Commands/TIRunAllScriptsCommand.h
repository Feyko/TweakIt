#pragma once
#include "command/ChatCommandLibrary.h"
#include "TIRunAllScriptsCommand.generated.h"

UCLASS()
class ATIRunAllScriptsCommand : public AChatCommandInstance
{
	GENERATED_BODY()
public:
	ATIRunAllScriptsCommand();
	virtual EExecutionStatus ExecuteCommand_Implementation(
		UCommandSender* Sender,
		const TArray<FString>& Arguments,
		const FString& Label
	) override;
};
