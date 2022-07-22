#include "StringConv.h"

FString FStringConv::ToFString(int Int)
{
	return FString::FromInt(Int);
}

FString FStringConv::ToFString(int64 Int)
{
	return FString::Printf(TEXT("%lld"), Int);
}

FString FStringConv::ToFString(size_t Size)
{
	return FString::Printf(TEXT("%llu"), Size);
}

FString FStringConv::ToFString(const char* String)
{
	return FString(String);
}

FString FStringConv::ToFString(FString String)
{
	return String;
}

FString FStringConv::ToFString(const TCHAR* String)
{
	return FString(String);
}

FString FStringConv::ToFString(std::string String)
{
	return FString(String.c_str());
}

FString FStringConv::ToFString(FName Name)
{
	return Name.ToString();
}

FString FStringConv::ToFString(EFunctionFlags Flags)
{
	FString Out;
	const TCHAR* FunctionFlags[32] =
	{
		TEXT("Final"),
		TEXT("0x00000002"),
		TEXT("BlueprintAuthorityOnly"),
		TEXT("BlueprintCosmetic"),
		TEXT("CheatBoard"),
		TEXT("0x00000020"),
		TEXT("Net"),
		TEXT("NetReliable"),
		TEXT("NetRequest"),
		TEXT("Exec"),
		TEXT("Native"),
		TEXT("Event"),
		TEXT("NetResponse"),
		TEXT("Static"),
		TEXT("NetMulticast"),
		TEXT("UbergraphFunction"),
		TEXT("MulticastDelegate"),
		TEXT("Public"),
		TEXT("Private"),
		TEXT("Protected"),
		TEXT("Delegate"),
		TEXT("NetServer"),
		TEXT("HasOutParms"),
		TEXT("HasDefaults"),
		TEXT("NetClient"),
		TEXT("DLLImport"),
		TEXT("BlueprintCallable"),
		TEXT("BlueprintEvent"),		
		TEXT("BlueprintPure"),			
		TEXT("EditorOnly"),
		TEXT("Const"),
		TEXT("NetValidate"),
	};
	
	for (int32 i = 0; i < 32; ++i)
	{
		const uint32 Mask = 1U << i;
		if ((Flags & Mask) != 0)
		{
			Out += FunctionFlags[i];
			Out += "|";
		}
	}
	Out.RemoveFromEnd("|");
	return Out;
}

FString FStringConv::ToFString(EPropertyFlags Flags)
{
	FString Out;
	TArray<const TCHAR*> FlagStrings = ParsePropertyFlags(Flags);
	for (auto String : FlagStrings)
	{
		Out += String;
		Out += "|";
	}
	Out.RemoveFromEnd("|");
	return Out;
}

FString FStringConv::ToFString(EObjectFlags Flags)
{
	FString Out;
	const TCHAR* ObjectFlags[32] =
	{
		TEXT("Final"),
		TEXT("0x00000002"),
		TEXT("BlueprintAuthorityOnly"),
		TEXT("BlueprintCosmetic"),
		TEXT("CheatBoard"),
		TEXT("0x00000020"),
		TEXT("Net"),
		TEXT("NetReliable"),
		TEXT("NetRequest"),
		TEXT("Exec"),
		TEXT("Native"),
		TEXT("Event"),
		TEXT("NetResponse"),
		TEXT("Static"),
		TEXT("NetMulticast"),
		TEXT("UbergraphFunction"),
		TEXT("MulticastDelegate"),
		TEXT("Public"),
		TEXT("Private"),
		TEXT("Protected"),
		TEXT("Delegate"),
		TEXT("NetServer"),
		TEXT("HasOutParms"),
		TEXT("HasDefaults"),
		TEXT("NetClient"),
		TEXT("DLLImport"),
		TEXT("BlueprintCallable"),
		TEXT("BlueprintEvent"),		
		TEXT("BlueprintPure"),			
		TEXT("EditorOnly"),
		TEXT("Const"),
		TEXT("NetValidate"),
	};
	
	for (int32 i = 0; i < 32; ++i)
	{
		const uint32 Mask = 1U << i;
		if ((Flags & Mask) != 0)
		{
			Out += ObjectFlags[i];
			Out += "|";
		}
	}
	Out.RemoveFromEnd("|");
	return Out;
}

template <typename ... T>
FString FStringConv::Printf(FString String, T... Args)
{
	const TCHAR (*AsArray)[] = reinterpret_cast<const TCHAR(*)[]>(*String);
	return FString::Printf(*AsArray, Args...);
}

template <typename V, typename E>
FString FStringConv::ToFString(TResult<V, E> Result)
{
	if(Result)
	{
		return FStringConv::ToFString(*Result);
	}
	return "Error: " + Result.UnwrapErr()->Error();
}
