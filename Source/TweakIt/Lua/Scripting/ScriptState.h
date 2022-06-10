#pragma once

class FScriptState
{
public:
	enum EScriptState
	{
		Running,
		Waiting,
		Successful,
		Errored,
	};

	FString Payload;

	bool IsCompleted();
private:
	EScriptState V;

	// Bullshit needed for this to work as an enum
	public:
	FScriptState() = default;
	FScriptState(EScriptState State) : V(State) { }
	explicit constexpr operator EScriptState() const { return V; }
	explicit operator bool() const = delete;    
	bool operator==(FScriptState Other) const { return V == Other.V; }
	bool operator!=(FScriptState Other) const { return V != Other.V; }
	bool operator==(EScriptState Other) const { return V == Other; }
	bool operator!=(EScriptState Other) const { return V != Other; }
};
