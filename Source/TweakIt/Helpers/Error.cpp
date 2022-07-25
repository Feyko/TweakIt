#include "Error.h"

#include "TweakIt/Logging/FTILog.h"

FStringError::~FStringError()
{}

FString IError::operator*()
{
	return Error();
}

FStringError::FStringError(FString Error) : ErrorString(Error)
{}

FString FStringError::Error()
{
	return ErrorString;
}

FWrapError::FWrapError(FError Wrapping, FError With) : Wrapped(Wrapping), With(With)
{}

FWrapError::~FWrapError()
{}

FString FWrapError::Error()
{
	return With->Error() + ": " + Wrapped->Error();
}

template<typename... T>
FError Errors::Newf(FString Error, T... Args)
{
	return MakeShared<FStringError>(FStringConv::Printf(Error, Args...));
}

template<typename... T>
FError Errors::Wrapf(FError Wrapping, FString With, T... Args)
{
	return MakeShared<FWrapError>(Wrapping, Newf(With, Args...));
}
