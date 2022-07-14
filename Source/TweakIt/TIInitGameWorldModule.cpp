#include "TIInitGameWorldModule.h"

#include "Commands/TIRunAllScriptsCommand.h"
#include "Commands/TIRunScriptCommand.h"
#include "Lua/Scripting/TIScriptOrchestrator.h"

UTIInitGameWorldModule::UTIInitGameWorldModule()
{
#if !WITH_EDITOR
	bRootModule = true;
	mChatCommands = {ATIRunScriptCommand::StaticClass(), ATIRunAllScriptsCommand::StaticClass()};
#endif
}
