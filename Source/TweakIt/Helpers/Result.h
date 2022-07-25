#pragma once

#include "Error.h"

template<typename ValType, typename ErrType = FError>
struct TResult
{
	TResult() = delete;
	TResult(ValType InValue);
	TResult(ErrType InError);

	bool IsOk();
	bool IsErr();
	operator bool();

	ValType Unwrap();
	ValType operator*();
	ValType* operator->();

	ErrType Err();
	
	bool operator==(TResult<ValType, ErrType> Other);
private:
	bool Ok;
	bool Checked;
	ValType Value;
	ErrType Error;
};
