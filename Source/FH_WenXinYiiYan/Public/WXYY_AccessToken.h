#pragma once

#include "CoreMinimal.h"
#include "HttpModule.h"
#include "Interfaces/IHttpRequest.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "WXYY_AccessToken.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FResponseAccessTokenDelegate, FString, Result);

UCLASS()
class FH_WENXINYIIYAN_API UGetAccessToken : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable)
	FResponseAccessTokenDelegate OnSuccess;

	UPROPERTY(BlueprintAssignable)
	FResponseAccessTokenDelegate OnFail;
	
	UFUNCTION(BlueprintCallable, Category="FH|WXYY", meta=(BlueprintInternalUseOnly="true"))
	static UGetAccessToken* GetAccessToken(FString API, FString Secret);

private:
	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> RequestToken = FHttpModule::Get().CreateRequest();
	
	void OnHttpsRequestToken(const FString& API, const FString& Secret);

	void OnDeserializeResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, const bool Success);

	FORCEINLINE bool IsRequestProcessing() const {return RequestToken->GetStatus() == EHttpRequestStatus::Processing;}
};