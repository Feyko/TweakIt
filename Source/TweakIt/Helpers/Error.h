#pragma once

class IError
{
public:
	virtual ~IError(){};
	virtual FString Error() = 0;

	FString operator*();
};

using FError = TSharedPtr<IError> ;

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
	explicit FWrapError(FError Wrapping, FError With);
	virtual ~FWrapError() override;
	virtual FString Error() override;
private:
	FError Wrapped;
	FError With;
};

namespace Errors
{
	template<typename... T>
	FError Newf(FString Error, T... Args);
	template<typename... T>
	FError Wrapf(FError Wrapping, FString With, T... Args);
}
