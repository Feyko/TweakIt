#include "TIRunScriptCommand.h"


#include "FGPlayerController.h"
#include "Command/CommandSender.h"
#include "TweakIt/Lua/Scripting/TIScriptOrchestrator.h"

ATIRunScriptCommand::ATIRunScriptCommand()
{
	CommandName = TEXT("runscript");
	Usage = TEXT("/runscript [Script Name] - Runs a script");
	MinNumberOfArguments = 1;
	bOnlyUsableByPlayer = false;
	Aliases.Add(TEXT("rs"));
}

EExecutionStatus ATIRunScriptCommand::ExecuteCommand_Implementation(
	UCommandSender* Sender,
	const TArray<FString>& Arguments,
	const FString& Label
)
{
	if (!Sender->GetPlayer()->HasAuthority())
	{
		Sender->SendChatMessage("You do not have the sufficient rights to do this.");
		return EExecutionStatus::INSUFFICIENT_PERMISSIONS;
	}
	FScriptState State = FTIScriptOrchestrator::Get()->StartScript(Arguments[0]);
	if (State == FScriptState::Errored)
	{
		Sender->SendChatMessage("Failed to run the script. Check the log for more info");
		return EExecutionStatus::UNCOMPLETED;
	}
	Sender->SendChatMessage("Success !");
	return EExecutionStatus::COMPLETED;
}
