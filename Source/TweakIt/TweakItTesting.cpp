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

int UTweakItTesting::Testing(ETIEnum arg)
{
	LOG("TweakItTesting::Testing ran")
	// FString out;
	// StaticEnum<ETIEnum>()->FindNameStringByValue(out, static_cast<int64>(arg));
	return 0;
}
