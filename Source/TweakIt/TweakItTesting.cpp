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

int UTweakItTesting::Testing(ETIEnum arg, FText Textparm)
{
	LOG("TweakItTesting::Testing ran")
	LOG(int(arg))
	LOG(Textparm.ToString())
	
	// FString out;
	// StaticEnum<ETIEnum>()->FindNameStringByValue(out, static_cast<int64>(arg));
	return 0;
}

void UTweakItTesting::TestingDelegate(FString ParmString)
{
	LOG("UTweakItTesting::TestingDelegate called")
	LOG(ParmString)
	// LOG(ParmInt)
	// LOG(ParmText)
	// LOG(ParmName)
}

void UTweakItTesting::InvalidTestingDelegate(FText ParmString, int ParmInt)
{
	LOG("UTweakItTesting::InvalidTestingDelegate called")
}
