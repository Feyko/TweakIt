#include "ScriptState.h"

bool FScriptState::IsCompleted()
{
	switch (V)
	{
	case Successful:
	case Errored:
		return true;
	default:
		return false;
	}
}