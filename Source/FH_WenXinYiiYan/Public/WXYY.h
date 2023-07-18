#pragma once

#include "WXYY.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogWenXin, Log, All);
DEFINE_LOG_CATEGORY(LogWenXin);

UENUM(BlueprintType)
enum class ERole : uint8
{
	ER_User			UMETA(Displayname="user"),
	ER_Assistant	UMETA(Displayname="assistant")
};

USTRUCT(BlueprintType)
struct FChatMessage
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(AllowPrivateAccess=true))
	ERole Role;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(AllowPrivateAccess=true))
	FString Content;

	FChatMessage() : Role(ERole::ER_User), Content(""){}

	explicit FChatMessage(const ERole NewRole)
	{
		Role = NewRole;
		Content = "";
	}
};

USTRUCT(BlueprintType)
struct FWXConfigKey
{
	GENERATED_USTRUCT_BODY()

	FString AppID;
	FString AppSecret;

	FWXConfigKey() : AppID(""), AppSecret(""){}
};

UCLASS()
class UWXYY : public UObject
{
	GENERATED_BODY()
	
public:
	static FString AccessToken;
	
	static FORCEINLINE void SetAccessToken(const FString& Token)
	{
		AccessToken = Token;
	}

	UFUNCTION(BlueprintPure, Category="FH|WXYY")
	static FORCEINLINE FString ConverToStr(const ERole& Role)
	{
		return Role == ERole::ER_User ? FString("user") : Role == ERole::ER_Assistant ? FString("assistant") : FString("user");
	}
};

FString UWXYY::AccessToken{};