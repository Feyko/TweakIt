#include "Error.h"

#include "TweakIt/Logging/FTILog.h"

FStringError::~FStringError()
{}

FStringError::FStringError(FString Error) : ErrorString(Error)
{}

FString FStringError::Error()
{
	return ErrorString;
}

FWrapError::FWrapError(Err Wrapping, Err With) : Wrapped(Wrapping), With(With)
{}

FWrapError::~FWrapError()
{}

FString FWrapError::Error()
{
	return With->Error() + ": " + Wrapped->Error();
}

template<typename... T>
Err Errors::Newf(FString Error, T... Args)
{
	return MakeShared<FStringError>(FStringConv::Printf(Error, Args...));
}

template<typename... T>
Err Errors::Wrapf(Err Wrapping, FString With, T... Args)
{
	return MakeShared<FWrapError>(Wrapping, Newf(With, Args...));
}
