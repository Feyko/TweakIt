#include "TIGameWorldModule.h"

#include "TweakIt/Commands/TIRunAllScriptsCommand.h"
#include "TweakIt/Commands/TIRunScriptCommand.h"

UTIGameWorldModule::UTIGameWorldModule()
{
#if !WITH_EDITOR
	bRootModule = true;
	mChatCommands = {ATIRunScriptCommand::StaticClass(), ATIRunAllScriptsCommand::StaticClass()};
#endif
}
