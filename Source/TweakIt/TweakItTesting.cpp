#include "TweakItTesting.h"

#include "Logging/FTILog.h"

using namespace std;

UTweakItTesting::UTweakItTesting()
{
	LOG("Constructing TITesting")
	Delegate.BindUFunction(this, "Testing");
	Delegate.Execute();
	LOG("Done")
}

UTweakItTesting* UTweakItTesting::Get()
{
	static UTweakItTesting* Self = NewObject<UTweakItTesting>(GetTransientPackage(), StaticClass(), "TweakItTesting", RF_MarkAsRootSet);
	return Self;
}

void UTweakItTesting::Testing()
{
	LOG("TweakItTesting::Testing ran")
}
