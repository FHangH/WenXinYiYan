#include "WXYY_AccessToken.h"
#include "HttpManager.h"
#include "HttpModule.h"
#include "Interfaces/IHttpResponse.h"
#include "WXYY.h"

UGetAccessToken* UGetAccessToken::GetAccessToken(FString API, FString Secret)
{
	const auto RequestTokenObject = NewObject<UGetAccessToken>();
	RequestTokenObject->OnHttpsRequestToken(API, Secret);
	return RequestTokenObject;
}

void UGetAccessToken::OnHttpsRequestToken(const FString& API, const FString& Secret)
{
	AddToRoot();
	
	const auto URL =
		FString::Printf(
			TEXT("https://aip.baidubce.com/oauth/2.0/token?client_id=%s&client_secret=%s&grant_type=client_credentials"),
			*API, *Secret);

	RequestToken = FHttpModule::Get().CreateRequest();
	RequestToken->SetVerb(TEXT("POST"));
	RequestToken->SetURL(URL);
	RequestToken->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
	RequestToken->SetHeader(TEXT("Accept"), TEXT("application/json"));

	RequestToken->OnProcessRequestComplete().BindUObject(this, &UGetAccessToken::OnDeserializeResponse);

	if (RequestToken->ProcessRequest() || IsRequestProcessing())
	{
		UE_LOG(LogWenXin, Warning, TEXT("====== WenXinYiYan Start ======"));
		UE_LOG(LogWenXin, Warning, TEXT("Request Process"));
		FHttpModule::Get().GetHttpManager().Tick(0.f);
		
		if (RequestToken->GetStatus() == EHttpRequestStatus::Failed)
		{
			OnFail.Broadcast("null");
			UE_LOG(LogWenXin, Warning, TEXT("Request Fail"));
			UE_LOG(LogWenXin, Warning, TEXT("====== WenXinYiYan End ======"));
		}
	
		RemoveFromRoot();
	}
	UE_LOG(LogWenXin, Warning, TEXT("Request Complete"));
}

void UGetAccessToken::OnDeserializeResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, const bool Success)
{
	FString Result;
	if (Success && Response->GetResponseCode() == EHttpResponseCodes::Ok)
	{
		UE_LOG(LogWenXin, Warning, TEXT("Response Success"));
		const auto ResponseContent = Response->GetContentAsString();
		const auto JsonReader = TJsonReaderFactory<>::Create(ResponseContent);
    
		if (TSharedPtr<FJsonObject> JsonObject; FJsonSerializer::Deserialize(JsonReader, JsonObject))
		{
			UE_LOG(LogWenXin, Warning, TEXT("Deserialize Response"));
			Result = JsonObject->GetStringField("access_token");
			OnSuccess.Broadcast(Result);
			UWXYY::SetAccessToken(Result);
			UE_LOG(LogWenXin, Warning, TEXT("Request Result = %s"), *Result);
			UE_LOG(LogWenXin, Warning, TEXT("====== WenXinYiYan End ======"));
		}
	}
	else
	{
		OnFail.Broadcast("null");
		UE_LOG(LogWenXin, Warning, TEXT("Request Result = %s"), *Result);
		UE_LOG(LogWenXin, Warning, TEXT("====== WenXinYiYan End ======"));
	}

	RemoveFromRoot();
}
