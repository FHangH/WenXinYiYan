#pragma once

#include "CoreMinimal.h"
#include "HttpModule.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "WXYY.h"
#include "Interfaces/IHttpRequest.h"
#include "WXYY_Chat.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FResponseChatMessage, FChatMessage, ChatMessage);

UCLASS()
class FH_WENXINYIIYAN_API USendChatMessage : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable)
	FResponseChatMessage OnSuccess;

	UPROPERTY(BlueprintAssignable)
	FResponseChatMessage OnFail;

	UFUNCTION(BlueprintCallable, Category="FH|WXYY", meta=(BlueprintInternalUseOnly="true"))
	static USendChatMessage* SendChatMessage(FChatMessage ChatMessage);
	
	UFUNCTION(BlueprintCallable, Category="FH|WXYY")
	static void CancelRequest();

private:
	static inline TSharedRef<IHttpRequest, ESPMode::ThreadSafe> RequestChat = FHttpModule::Get().CreateRequest();;
	
	void OnHttpsRequestChatMessage(const FChatMessage& ChatMessage);
	
	void OnDeserializeResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, const bool Success);
};

UCLASS()
class FH_WENXINYIIYAN_API USendChatMessageByStream : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable)
	FResponseChatMessage OnUpdate;
	
	UPROPERTY(BlueprintAssignable)
	FResponseChatMessage OnCompleted;

	UPROPERTY(BlueprintAssignable)
	FResponseChatMessage OnFail;

	UFUNCTION(BlueprintCallable, Category="FH|WXYY", meta=(BlueprintInternalUseOnly="true"))
	static USendChatMessageByStream* SendChatMessageByStream(FChatMessage ChatMessage);

	UFUNCTION(BlueprintCallable, Category="FH|WXYY")
	static void CancelRequestStream();

private:
	FString AllResult;
	FString AllResponse;
	FString TempResponse;
	static inline TSharedRef<IHttpRequest, ESPMode::ThreadSafe> RequestChat = FHttpModule::Get().CreateRequest();;
	
	void OnHttpsRequestChatMessage(const FChatMessage& ChatMessage);

	void OnUpdateResponse(FHttpRequestPtr HttpRequest, int32 BytesSent, int32 BytesReceived);

	void OnDeserializeResponseUpdate();
	
	void OnCompletedResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, const bool Success);
};