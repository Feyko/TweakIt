#include "TIRunAllScriptsCommand.h"

#include "TIRunScriptCommand.h"


#include "FGPlayerController.h"
#include "Command/CommandSender.h"
#include "TweakIt/Subsystem/TweakItSubsystem.h"

ATIRunAllScriptsCommand::ATIRunAllScriptsCommand() {
	CommandName = TEXT("runallscripts");
	Usage = TEXT("/runallscripts - Runs every script in the TweakIt folder");
	bOnlyUsableByPlayer = false;
	Aliases.Add(TEXT("ras"));
}

EExecutionStatus ATIRunAllScriptsCommand::ExecuteCommand_Implementation(
	UCommandSender* Sender,
	const TArray<FString>& Arguments,
	const FString& Label
) {
	if (!Sender->GetPlayer()->HasAuthority()) {
		Sender->SendChatMessage("You do not have the sufficient rights to do this.");
		return EExecutionStatus::INSUFFICIENT_PERMISSIONS;
	}
	bool OK = ATweakItSubsystem::Get(this)->RunAllScripts();
	if (!OK) {
		Sender->SendChatMessage("Failed to run the scripts. Check the log for more info");
		return EExecutionStatus::UNCOMPLETED;
	}
	Sender->SendChatMessage("Success !");
	return EExecutionStatus::COMPLETED;
}
