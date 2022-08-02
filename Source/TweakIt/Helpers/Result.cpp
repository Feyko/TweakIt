#include "Result.h"

#include "TweakIt/Lua/FTILuaFuncManager.h"

template <typename Val, typename ErrType>
bool TResult<Val, ErrType>::IsOk()
{
	Checked = true;
	return Ok;
}

template <typename Val, typename ErrType>
bool TResult<Val, ErrType>::IsErr()
{
	return !IsOk();
}

template <typename Val, typename ErrType>
TResult<Val, ErrType>::TResult(Val Value) : Ok(true), Checked(false), Value(Value)
{

}

template <typename Val, typename ErrType>
TResult<Val, ErrType>::TResult(ErrType Error) : Ok(false), Checked(false), Value(nullptr), Error(Error)
{
	
}

template <typename Val, typename ErrType>
TResult<Val, ErrType>::operator bool()
{
	return IsOk();
}

template <typename Val, typename ErrType>
Val TResult<Val, ErrType>::Unwrap()
{
	checkf(Checked, TEXT("Trying to unwrap an unchecked FResult"))
	checkf(Ok, TEXT("Trying to get the value of an ErrTypeored TResult"))
	return Value;
}

template <typename Val, typename ErrType>
Val TResult<Val, ErrType>::operator*()
{
	return Unwrap();
}

template <typename Val, typename ErrType>
Val* TResult<Val, ErrType>::operator->()
{
	checkf(Checked, TEXT("Trying to unwrap an unchecked FResult"))
	checkf(Ok, TEXT("Trying to get the value of an ErrTypeored TResult"))
	return &Value;
}

template <typename Val, typename ErrType>
ErrType TResult<Val, ErrType>::Err()
{
	checkf(Checked, TEXT("Trying to unwrap an unchecked FResult"))
	checkf(IsErr(), TEXT("Trying to get the ErrTypeor of a valid TResult"))
	return Error;
}

template <typename Val, typename ErrType>
bool TResult<Val, ErrType>::operator==(TResult<Val, ErrType> Other)
{
	return Ok && Other.Ok && Value == Other.Value || !Ok && !Other.Ok && Error == Other.Error;
}
