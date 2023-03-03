#include "TweakItTesting.h"

#include "Configuration/ConfigManager.h"
#include "Logging/FTILog.h"

using namespace std;

UTweakItTesting::UTweakItTesting()
{
	LOG("Constructing TITesting")
	Delegate.BindUFunction(this, "TestingDelegate");
	This = this;
	// Delegate.Execute("IncredibleValue", 24);
	// LOG("Done")
}

UTweakItTesting* UTweakItTesting::Get()
{
	return Cast<UTweakItTesting>(StaticClass()->ClassDefaultObject);
}

int UTweakItTesting::Testing(ETIEnum EnumP, FString StringP)
{
	LOG("TweakItTesting::Testing ran")
	LOG(StringP)
	return int(EnumP);
}

void UTweakItTesting::TestingDelegate(FString ParmString)
{
	LOG("UTweakItTesting::TestingDelegate called")
	LOG(ParmString)
}

void UTweakItTesting::InvalidTestingDelegate(FText ParmString, int ParmInt)
{
	LOG("UTweakItTesting::InvalidTestingDelegate called")
}
