#pragma once

class FScriptState
{
public:
	enum EScriptState
	{
		NotRan,
		Running,
		Waiting,
		Successful,
		Errored,
	};

	FString Payload;
	EScriptState V;

	bool IsCompleted();

	// Bullshit needed for this to work as an enum
	FScriptState() = default;

	FScriptState(EScriptState State) : V(State)
	{
	}

	explicit constexpr operator EScriptState() const { return V; }
	explicit operator bool() const = delete;
	bool operator==(FScriptState Other) const { return V == Other.V; }
	bool operator!=(FScriptState Other) const { return V != Other.V; }
	bool operator==(EScriptState Other) const { return V == Other; }
	bool operator!=(EScriptState Other) const { return V != Other; }
};
