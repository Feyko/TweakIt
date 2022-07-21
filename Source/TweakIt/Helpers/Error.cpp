#include "Error.h"

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

Err Errors::New(FString Error)
{
	return MakeShared<FStringError>(Error);
}

Err Errors::Wrap(Err Wrapping, FString With)
{
	return MakeShared<FWrapError>(Wrapping, New(With));
}
