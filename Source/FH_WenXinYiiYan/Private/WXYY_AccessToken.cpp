#include "WXYY_AccessToken.h"
#include "HttpManager.h"
#include "HttpModule.h"
#include "Interfaces/IHttpResponse.h"
#include "WXYY.h"

UGetAccessToken* UGetAccessToken::GetAccessToken(FString API, FString Secret)
{
	const auto RequestToken = NewObject<UGetAccessToken>();
	RequestToken->OnHttpsRequestToken(API, Secret);
	return RequestToken;
}

void UGetAccessToken::OnHttpsRequestToken(const FString& API, const FString& Secret)
{
	AddToRoot();
	
	const auto URL =
		FString::Printf(
			TEXT("https://aip.baidubce.com/oauth/2.0/token?client_id=%s&client_secret=%s&grant_type=client_credentials"),
			*API, *Secret);

	const auto Request = FHttpModule::Get().CreateRequest();
	Request->SetVerb(TEXT("POST"));
	Request->SetURL(URL);
	Request->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
	Request->SetHeader(TEXT("Accept"), TEXT("application/json"));

	Request->OnProcessRequestComplete().BindUObject(this, &UGetAccessToken::OnDeserializeResponse);

	if (Request->ProcessRequest())
	{
		UE_LOG(LogWenXin, Warning, TEXT("====== WenXinYiYan Start ======"));
		UE_LOG(LogWenXin, Warning, TEXT("Request Process"));
		FHttpModule::Get().GetHttpManager().Tick(0.f);
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
