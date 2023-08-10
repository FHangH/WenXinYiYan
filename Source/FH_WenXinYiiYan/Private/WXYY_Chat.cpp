#include "WXYY_Chat.h"
#include "HttpManager.h"
#include "HttpModule.h"
#include "Interfaces/IHttpResponse.h"

// Send Chat Message
USendChatMessage* USendChatMessage::SendChatMessage(FChatMessage ChatMessage)
{
	const auto RequestObject = NewObject<USendChatMessage>();
	RequestObject->OnHttpsRequestChatMessage(ChatMessage);
	SendChatHandler = RequestObject;
	return RequestObject;
}

void USendChatMessage::OnHttpsRequestChatMessage(const FChatMessage& ChatMessage)
{
	AddToRoot();

	const auto URL =
		FString::Printf(
			TEXT("https://aip.baidubce.com/rpc/2.0/ai_custom/v1/wenxinworkshop/chat/eb-instant?access_token=%s"),
			*UWXYY::AccessToken);

	RequestChat = FHttpModule::Get().CreateRequest();
	RequestChat->SetVerb(TEXT("POST"));
	RequestChat->SetURL(URL);
	RequestChat->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
	RequestChat->SetHeader(TEXT("Accept"), TEXT("application/json"));
	RequestChat->SetContentAsString(
		FString::Printf(
			TEXT("{\"messages\": [{\"role\":\"%s\",\"content\":\"%s\"}]}"),
			*UWXYY::ConverToStr(ChatMessage.Role), *ChatMessage.Content));

	RequestChat->OnProcessRequestComplete().BindUObject(this, &USendChatMessage::OnDeserializeResponse);

	if (RequestChat->ProcessRequest() || IsRequestProcessing())
	{
		UE_LOG(LogWenXin, Warning, TEXT("====== WenXinYiYan Start ======"));
		UE_LOG(LogWenXin, Warning, TEXT("Request Process"));
		FHttpModule::Get().GetHttpManager().Tick(0.f);

		if (RequestChat->GetStatus() == EHttpRequestStatus::Failed)
		{
			FChatMessage Message;
			Message.Role = ERole::ER_Assistant;
			Message.Content = "";
			OnFail.Broadcast(Message);
			UE_LOG(LogWenXin, Warning, TEXT("Request Fail"));
			UE_LOG(LogWenXin, Warning, TEXT("====== WenXinYiYan End ======"));
		}
	}
	UE_LOG(LogWenXin, Warning, TEXT("Request Complete"));
}

void USendChatMessage::OnDeserializeResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, const bool Success)
{
	FChatMessage ChatMessage;
	
	if (Success && Response->GetResponseCode() == EHttpResponseCodes::Ok)
	{
		UE_LOG(LogWenXin, Warning, TEXT("Response Success"));
		const auto ResponseContent = Response->GetContentAsString();
		const auto JsonReader = TJsonReaderFactory<>::Create(ResponseContent);
    
		if (TSharedPtr<FJsonObject> JsonObject; FJsonSerializer::Deserialize(JsonReader, JsonObject))
		{
			UE_LOG(LogWenXin, Warning, TEXT("Deserialize Response"));
			ChatMessage.Role = ERole::ER_Assistant;
			ChatMessage.Content = JsonObject->GetStringField("result");
			if (ChatMessage.Content == "")
			{
				OnFail.Broadcast(ChatMessage);
				UE_LOG(LogWenXin, Warning, TEXT("Request Result = %s : null"), *UWXYY::ConverToStr(ChatMessage.Role));
				UE_LOG(LogWenXin, Warning, TEXT("====== WenXinYiYan End ======"));
				
				RemoveFromRoot();
				return;
			}
			OnCompleted.Broadcast(ChatMessage);
			UE_LOG(LogWenXin, Warning, TEXT("Request Result = %s : %s"), *UWXYY::ConverToStr(ChatMessage.Role), *ChatMessage.Content);
			UE_LOG(LogWenXin, Warning, TEXT("====== WenXinYiYan End ======"));
		}
	}
	else
	{
		ChatMessage.Role = ERole::ER_Assistant;
		ChatMessage.Content = "";
		OnFail.Broadcast(ChatMessage);
		UE_LOG(LogWenXin, Warning, TEXT("Request Result = %s : null"), *UWXYY::ConverToStr(ChatMessage.Role));
		UE_LOG(LogWenXin, Warning, TEXT("====== WenXinYiYan End ======"));
	}
    
	RemoveFromRoot();
}

bool USendChatMessage::IsRequestProcessing()
{
	if (!SendChatHandler || !SendChatHandler->RequestChat) return false;
	return SendChatHandler->RequestChat->GetStatus() == EHttpRequestStatus::Processing;
}

void USendChatMessage::CancelMessageRequest()
{
	if (!SendChatHandler || !SendChatHandler->RequestChat) return;
	if (SendChatHandler->RequestChat->GetStatus() == EHttpRequestStatus::Processing)
	{
		SendChatHandler->RequestChat->CancelRequest();
		UE_LOG(LogWenXin, Warning, TEXT("Request Cancle"))
	}
}

// Send Chat Message By Stream
USendChatMessageByStream* USendChatMessageByStream::SendChatMessageByStream(FChatMessage ChatMessage)
{
	const auto RequestObject = NewObject<USendChatMessageByStream>();
	RequestObject->OnHttpsRequestChatMessage(ChatMessage);
	SendStreamChatHandler = RequestObject;
	return RequestObject;
}

void USendChatMessageByStream::OnHttpsRequestChatMessage(const FChatMessage& ChatMessage)
{
	AddToRoot();

	AllResult = "";
	TempResponse = "";
	AllResponse = "";
	
	const auto URL =
		FString::Printf(
			TEXT("https://aip.baidubce.com/rpc/2.0/ai_custom/v1/wenxinworkshop/chat/eb-instant?access_token=%s"),
			*UWXYY::AccessToken);

	RequestChat = FHttpModule::Get().CreateRequest();
	RequestChat->SetVerb(TEXT("POST"));
	RequestChat->SetURL(URL);
	RequestChat->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
	RequestChat->SetHeader(TEXT("Accept"), TEXT("application/json"));
	RequestChat->SetContentAsString(
		FString::Printf(
			TEXT("{\"messages\": [{\"role\":\"%s\",\"content\":\"%s\"}],\"stream\":true}"),
			*UWXYY::ConverToStr(ChatMessage.Role), *ChatMessage.Content));

	RequestChat->OnRequestProgress().BindUObject(this, &USendChatMessageByStream::OnUpdateResponse);
	RequestChat->OnProcessRequestComplete().BindUObject(this, &USendChatMessageByStream::OnCompletedResponse);

	if (RequestChat->ProcessRequest() || IsRequestProcessing())
	{
		UE_LOG(LogWenXin, Warning, TEXT("====== WenXinYiYan Start ======"));
		UE_LOG(LogWenXin, Warning, TEXT("Request Stream Process"));
		FHttpModule::Get().GetHttpManager().Tick(0.f);

		if (RequestChat->GetStatus() == EHttpRequestStatus::Failed)
		{
			FChatMessage Message;
			Message.Role = ERole::ER_Assistant;
			Message.Content = "";
			OnFail.Broadcast(Message);
			UE_LOG(LogWenXin, Warning, TEXT("Request Stream Fail"));
			UE_LOG(LogWenXin, Warning, TEXT("====== WenXinYiYan End ======"));
		}
	}
	UE_LOG(LogWenXin, Warning, TEXT("Request Stream Complete"));
}

void USendChatMessageByStream::OnUpdateResponse(FHttpRequestPtr HttpRequest, int32 BytesSent, int32 BytesReceived)
{
	UE_LOG(LogWenXin, Warning, TEXT("Request Process By Stream"));
	const auto Response = HttpRequest->GetResponse()->GetContentAsString();

	if (AllResponse.IsEmpty())
	{
		AllResponse = TempResponse = Response;
		OnDeserializeResponseUpdate();
	}
	else
	{
		TempResponse = Response;
		TempResponse.RemoveFromStart(AllResponse);
		OnDeserializeResponseUpdate();
		AllResponse = Response;
	}
}

void USendChatMessageByStream::OnDeserializeResponseUpdate()
{
	if (TempResponse.StartsWith("data:"))
	{
		auto JsonData = TempResponse.RightChop(6);
		JsonData.TrimStartAndEndInline();

		const auto JsonReader = TJsonReaderFactory<>::Create(JsonData);
		if (TSharedPtr<FJsonObject> JsonObject; FJsonSerializer::Deserialize(JsonReader, JsonObject))
		{
			UE_LOG(LogWenXin, Warning, TEXT("Deserialize Stream Response"));
			FChatMessage ChatMessage;
			ChatMessage.Role = ERole::ER_Assistant;
			ChatMessage.Content = JsonObject->GetStringField("result");
			OnUpdate.Broadcast(ChatMessage);
			AllResult += ChatMessage.Content;
			UE_LOG(LogWenXin, Warning, TEXT("Stream Result = %s"), *ChatMessage.Content);
		}
	}
}

void USendChatMessageByStream::OnCompletedResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, const bool Success)
{
	FChatMessage ChatMessage;
	if (Success && Response->GetResponseCode() == EHttpResponseCodes::Ok)
	{
		UE_LOG(LogWenXin, Warning, TEXT("Response Stream Success"));
		ChatMessage.Role = ERole::ER_Assistant;
		ChatMessage.Content = AllResult;
		if (ChatMessage.Content == "")
		{
			OnFail.Broadcast(ChatMessage);
			UE_LOG(LogWenXin, Warning, TEXT("Stream Result = %s : null"), *UWXYY::ConverToStr(ChatMessage.Role));
			UE_LOG(LogWenXin, Warning, TEXT("====== WenXinYiYan End ======"));

			RemoveFromRoot();
			return;
		}
		OnCompleted.Broadcast(ChatMessage);
		UE_LOG(LogWenXin, Warning, TEXT("Request Stream Result = %s : %s"), *UWXYY::ConverToStr(ChatMessage.Role), *ChatMessage.Content);
		UE_LOG(LogWenXin, Warning, TEXT("====== WenXinYiYan End ======"));
	}
	else
	{
		ChatMessage.Role = ERole::ER_Assistant;
		ChatMessage.Content = "";
		OnFail.Broadcast(ChatMessage);
		UE_LOG(LogWenXin, Warning, TEXT("Request Stream Result = %s : null"), *UWXYY::ConverToStr(ChatMessage.Role));
		UE_LOG(LogWenXin, Warning, TEXT("====== WenXinYiYan End ======"));
	}

	RemoveFromRoot();
}

bool USendChatMessageByStream::IsRequestProcessing()
{
	if (!SendStreamChatHandler || !SendStreamChatHandler->RequestChat) return false;
	return SendStreamChatHandler->RequestChat->GetStatus() == EHttpRequestStatus::Processing;
}

void USendChatMessageByStream::CancelStreamMessageRequest()
{
	if (!SendStreamChatHandler || !SendStreamChatHandler->RequestChat) return;
	if (SendStreamChatHandler->RequestChat->GetStatus() == EHttpRequestStatus::Processing)
	{
		SendStreamChatHandler->RequestChat->CancelRequest();
		UE_LOG(LogWenXin, Warning, TEXT("Request Cancle"))
	}
}
