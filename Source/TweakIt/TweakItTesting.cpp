#include "TweakItTesting.h"

#include "Logging/FTILog.h"

using namespace std;

UTweakItTesting::UTweakItTesting()
{
	// LOG("Constructing TITesting")
	Delegate.BindUFunction(this, "Testing");
	This = this;
	// Delegate.Execute();
	// LOG("Done")
}

UTweakItTesting* UTweakItTesting::Get()
{
	return Cast<UTweakItTesting>(StaticClass()->ClassDefaultObject);
}

void UTweakItTesting::Testing(int i)
{
	LOG("TweakItTesting::Testing ran")
	LOG(i)
}
