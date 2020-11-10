#include "TIRunScriptCommand.h"

#include "TweakItGameSubsystem.h"

ATIRunScriptCommand::ATIRunScriptCommand()
{
	ModId = TEXT("TweakIt");
	CommandName = TEXT("runscript");
	Usage = TEXT("/runscript [Script Name] - Runs a script");
	MinNumberOfArguments = 1;
	bOnlyUsableByPlayer = false;
	Aliases.Add(TEXT("rs"));
}

EExecutionStatus ATIRunScriptCommand::ExecuteCommand_Implementation(UCommandSender* Sender, const TArray<FString>& Arguments, const FString& Label)
{
	if(Sender->GetPlayer()->GetLocalPlayer()->GetControllerId() != 0)
	{
		Sender->SendChatMessage("You do not have the sufficient rights to do this.");
		return EExecutionStatus::INSUFFICIENT_PERMISSIONS;
	}
	bool result = UTweakItGameSubsystem::Get(this)->RunScript(Arguments[0]);
	if(!result)
	{
		Sender->SendChatMessage("Failed to run the script. Check the log for more info");
		return EExecutionStatus::UNCOMPLETED;
	}
	Sender->SendChatMessage("Success !");
	return EExecutionStatus::COMPLETED;
}
