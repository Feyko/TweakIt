#include "Result.h"

template <typename Val, typename Err>
bool TResult<Val, Err>::IsOk()
{
	Checked = true;
	return Ok;
}

template <typename Val, typename Err>
bool TResult<Val, Err>::IsErr()
{
	return !IsOk();
}

template <typename Val, typename Err>
TResult<Val, Err>::TResult(Val InValue) : Value(InValue), Ok(true), Checked(false)
{
	
}

template <typename Val, typename Err>
TResult<Val, Err>::TResult(Err InError) : Error(InError), Ok(false), Checked(false)
{
	
}

template <typename Val, typename Err>
TResult<Val, Err>::operator bool()
{
	return IsOk();
}

template <typename Val, typename Err>
Val TResult<Val, Err>::Unwrap()
{
	checkf(Checked, TEXT("Trying to unwrap an unchecked FResult"))
	checkf(Ok, TEXT("Trying to get the value of an errored TResult"))
	return Value;
}

template <typename Val, typename Err>
Val TResult<Val, Err>::operator*()
{
	return Unwrap();
}

template <typename Val, typename Err>
Val TResult<Val, Err>::operator->()
{
	return Unwrap();
}

template <typename Val, typename Err>
Err TResult<Val, Err>::UnwrapErr()
{
	checkf(Checked, TEXT("Trying to unwrap an unchecked FResult"))
	checkf(IsErr(), TEXT("Trying to get the error of a valid TResult"))
	return Error;
}

template <typename Val, typename Err>
bool TResult<Val, Err>::operator==(TResult<Val, Err> Other)
{
	return Ok && Other.Ok && Value == Other.Value || !Ok && !Other.Ok && Error == Other.Error;
}
