#pragma once

class IError
{
public:
	virtual ~IError(){};
	virtual FString Error() = 0;
};

using Err = TSharedPtr<IError> ;

class FStringError : public IError
{
public:
	explicit FStringError(FString Error);
	virtual ~FStringError() override;
	virtual FString Error() override;

private:
	FString ErrorString;
};

class FWrapError : public IError
{
public:
	explicit FWrapError(Err Wrapping, Err With);
	virtual ~FWrapError() override;
	virtual FString Error() override;
private:
	Err Wrapped;
	Err With;
};

namespace Errors
{
	template<typename... T>
	Err Newf(FString Error, T... Args);
	template<typename... T>
	Err Wrapf(Err Wrapping, FString With, T... Args);
}
