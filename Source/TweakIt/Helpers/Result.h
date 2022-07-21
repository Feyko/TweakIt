#pragma once

#include "Error.h"

template<typename Val, typename Err = Err>
struct TResult
{
	TResult() = delete;
	TResult(Val InValue);
	TResult(Err InError);

	bool IsOk();
	bool IsErr();
	operator bool();

	Val Unwrap();
	Val operator*();
	Val operator->();

	Err UnwrapErr();
	
	bool operator==(TResult<Val, Err> Other);

	bool Ok;
	bool Checked;
	Val Value;
	Err Error;
};
