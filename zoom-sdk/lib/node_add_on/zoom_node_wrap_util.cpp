#include "zoom_node_wrap_util.h"

v8::Local<v8::String> NodeWrapUtil::ToV8String(v8::Isolate* isolate, const std::string& str) {
	return v8::String::NewFromUtf8(isolate, str.c_str(), v8::NewStringType::kInternalized).ToLocalChecked();
}

v8::Local<v8::Object> NodeWrapUtil::CreateUserInfoObject(v8::Isolate* isolate, const ZNUserInfomation& zn_userInfo) {
	v8::HandleScope scope(isolate);
	auto context = isolate->GetCurrentContext();
	v8::Local<v8::Object> node = v8::Object::New(isolate);

	node->Set(context, ToV8String(isolate, "userName"), ToV8String(isolate, zs2s(zn_userInfo.userName)));
	node->Set(context, ToV8String(isolate, "isHost"), v8::Boolean::New(isolate, zn_userInfo.isHost));
	node->Set(context, ToV8String(isolate, "userID"), v8::Integer::New(isolate, zn_userInfo.userID));
	node->Set(context, ToV8String(isolate, "avatarPath"), ToV8String(isolate, zs2s(zn_userInfo.avatarPath)));
	node->Set(context, ToV8String(isolate, "persistentId"), ToV8String(isolate, zs2s(zn_userInfo.persistentId)));
	node->Set(context, ToV8String(isolate, "customerKey"), ToV8String(isolate, zs2s(zn_userInfo.customerKey)));
	node->Set(context, ToV8String(isolate, "isVideoOn"), v8::Boolean::New(isolate, zn_userInfo.isVideoOn));
	node->Set(context, ToV8String(isolate, "isAudioMuted"), v8::Boolean::New(isolate, zn_userInfo.isAudioMuted));
	node->Set(context, ToV8String(isolate, "isMySelf"), v8::Boolean::New(isolate, zn_userInfo.isMySelf));
	node->Set(context, ToV8String(isolate, "userRole"), v8::Integer::New(isolate, zn_userInfo.userRole));
	node->Set(context, ToV8String(isolate, "isPurePhoneUser"), v8::Boolean::New(isolate, zn_userInfo.isPurePhoneUser));
	node->Set(context, ToV8String(isolate, "isTalking"), v8::Boolean::New(isolate, zn_userInfo.isTalking));
	node->Set(context, ToV8String(isolate, "isH323User"), v8::Boolean::New(isolate, zn_userInfo.isH323User));
	node->Set(context, ToV8String(isolate, "localRecordingStatus"), v8::Integer::New(isolate, zn_userInfo.localRecordingStatus));
	node->Set(context, ToV8String(isolate, "userInfoType"), v8::Integer::New(isolate, zn_userInfo.userInfoType));
	node->Set(context, ToV8String(isolate, "isRawLiveStreaming"), v8::Boolean::New(isolate, zn_userInfo.isRawLiveStreaming));
	node->Set(context, ToV8String(isolate, "hasRawLiveStreamPrivilege"), v8::Boolean::New(isolate, zn_userInfo.HasRawLiveStreamPrivilege));
	node->Set(context, ToV8String(isolate, "hasCamera"), v8::Boolean::New(isolate, zn_userInfo.hasCamera));
	node->Set(context, ToV8String(isolate, "audioJoinType"), v8::Integer::New(isolate, zn_userInfo.audioJoinType));
	node->Set(context, ToV8String(isolate, "isRaiseHand"), v8::Boolean::New(isolate, zn_userInfo.isRaiseHand));
	node->Set(context, ToV8String(isolate, "isBotUser"), v8::Boolean::New(isolate, zn_userInfo.isBotUser));
	node->Set(context, ToV8String(isolate, "botAppName"), ToV8String(isolate, zs2s(zn_userInfo.botAppName)));
	node->Set(context, ToV8String(isolate, "isInWebinarBackstage"), v8::Boolean::New(isolate, zn_userInfo.isInWebinarBackstage));
	node->Set(context, ToV8String(isolate, "isInWaitingRoom"), v8::Boolean::New(isolate, zn_userInfo.isInWaitingRoom));
	node->Set(context, ToV8String(isolate, "isClosedCaptionSender"), v8::Boolean::New(isolate, zn_userInfo.isClosedCaptionSender));
#if (!defined TARGET_OS_MAC)
	node->Set(context, ToV8String(isolate, "audioVoiceLevel"), v8::Integer::New(isolate, zn_userInfo.audioVoiceLevel));
#endif
	node->Set(context, ToV8String(isolate, "isInterpreter"), v8::Boolean::New(isolate, zn_userInfo.isInterpreter));
	node->Set(context, ToV8String(isolate, "isSignLanguageInterpreter"), v8::Boolean::New(isolate, zn_userInfo.isSignLanguageInterpreter));
	node->Set(context, ToV8String(isolate, "interpreterActiveLanguage"), ToV8String(isolate, zs2s(zn_userInfo.interpreterActiveLanguage)));
	node->Set(context, ToV8String(isolate, "emojiFeedbackType"), v8::Integer::New(isolate, zn_userInfo.emojiFeedbackType));

	v8::Local<v8::Object> webinarAttendeeStatusObj = v8::Object::New(isolate);
	webinarAttendeeStatusObj->Set(context, ToV8String(isolate, "allow_talk"), v8::Boolean::New(isolate, zn_userInfo.webinarAttendeeStatus.allow_talk));
	node->Set(context, ToV8String(isolate, "webinarAttendeeStatus"), webinarAttendeeStatusObj);

	return node;
}

v8::Local<v8::Object> NodeWrapUtil::CreateWallpaperItemObject(v8::Isolate* isolate, const ZNWallpaperItem& zn_item) {
	auto context = isolate->GetCurrentContext();
	v8::Local<v8::Object> node = v8::Object::New(isolate);

	if (zn_item.wallpaperId.empty())
	{
		return node;
	}
	node->Set(context, ToV8String(isolate, "wallpaperLayoutMode"), v8::Integer::New(isolate, zn_item.wallpaperLayoutMode));
	node->Set(context, ToV8String(isolate, "wallpaperId"), ToV8String(isolate, zs2s(zn_item.wallpaperId)));
	node->Set(context, ToV8String(isolate, "title"), ToV8String(isolate, zs2s(zn_item.title)));
	node->Set(context, ToV8String(isolate, "thumbnailPath"), ToV8String(isolate, zs2s(zn_item.thumbnailPath)));
	node->Set(context, ToV8String(isolate, "path"), ToV8String(isolate, zs2s(zn_item.path)));
	node->Set(context, ToV8String(isolate, "transparency"), v8::Integer::New(isolate, zn_item.transparency));

	return node;
}

v8::Local<v8::Object> NodeWrapUtil::CreateSharingSourceInfoObject(v8::Isolate* isolate, const ZNZoomSDKSharingSourceInfo& zn_shareInfo) {
	auto context = isolate->GetCurrentContext();
	v8::Local<v8::Object> node = v8::Object::New(isolate);

	node->Set(context, ToV8String(isolate, "userid"), v8::Integer::NewFromUnsigned(isolate, zn_shareInfo.userid));
	node->Set(context, ToV8String(isolate, "shareSourceID"), v8::Integer::NewFromUnsigned(isolate, zn_shareInfo.shareSourceID));
	node->Set(context, ToV8String(isolate, "status"), v8::Integer::New(isolate, zn_shareInfo.status));
	node->Set(context, ToV8String(isolate, "isShowingInFirstView"), v8::Boolean::New(isolate, zn_shareInfo.isShowingInFirstView));
	node->Set(context, ToV8String(isolate, "isShowingInSecondView"), v8::Boolean::New(isolate, zn_shareInfo.isShowingInSecondView));
	node->Set(context, ToV8String(isolate, "isCanBeRemoteControl"), v8::Boolean::New(isolate, zn_shareInfo.isCanBeRemoteControl));
	node->Set(context, ToV8String(isolate, "bEnableOptimizingVideoSharing"), v8::Boolean::New(isolate, zn_shareInfo.bEnableOptimizingVideoSharing));
	node->Set(context, ToV8String(isolate, "contentType"), v8::Integer::New(isolate, zn_shareInfo.contentType));
#if (defined __MACOS__)
	node->Set(context, ToV8String(isolate, "windowID"), v8::Integer::NewFromUnsigned(isolate, zn_shareInfo.windowID));
	node->Set(context, ToV8String(isolate, "monitorID"), v8::Integer::NewFromUnsigned(isolate, zn_shareInfo.monitorID));
#else
	node->Set(context, ToV8String(isolate, "windowID"), v8::BigInt::NewFromUnsigned(isolate, zn_shareInfo.windowID));
	node->Set(context, ToV8String(isolate, "monitorID"), ToV8String(isolate, zs2s(zn_shareInfo.monitorID)));
#endif

	return node;
}

v8::Local<v8::Object> NodeWrapUtil::CreateChatMsgInfoObject(v8::Isolate* isolate, const ZNChatMsgInfo& zn_chatMsg) {
	auto context = isolate->GetCurrentContext();
	v8::Local<v8::Object> node = v8::Object::New(isolate);

	node->Set(context, ToV8String(isolate, "messageID"), ToV8String(isolate, zs2s(zn_chatMsg.messageID)));
	node->Set(context, ToV8String(isolate, "senderUserId"), v8::Integer::NewFromUnsigned(isolate, zn_chatMsg.senderUserId));
	node->Set(context, ToV8String(isolate, "senderDisplayName"), ToV8String(isolate, zs2s(zn_chatMsg.senderDisplayName)));
	node->Set(context, ToV8String(isolate, "receiverUserId"), v8::Integer::NewFromUnsigned(isolate, zn_chatMsg.receiverUserId));
	node->Set(context, ToV8String(isolate, "receiverDisplayName"), ToV8String(isolate, zs2s(zn_chatMsg.receiverDisplayName)));
	node->Set(context, ToV8String(isolate, "content"), ToV8String(isolate, zs2s(zn_chatMsg.content)));
	node->Set(context, ToV8String(isolate, "timeStamp"), v8::Integer::New(isolate, zn_chatMsg.timeStamp));
	node->Set(context, ToV8String(isolate, "isChatToAll"), v8::Boolean::New(isolate, zn_chatMsg.isChatToAll));
	node->Set(context, ToV8String(isolate, "isChatToAllPanelist"), v8::Boolean::New(isolate, zn_chatMsg.isChatToAllPanelist));
	node->Set(context, ToV8String(isolate, "isChatToWaitingroom"), v8::Boolean::New(isolate, zn_chatMsg.isChatToWaitingroom));
	node->Set(context, ToV8String(isolate, "chatMessageType"), v8::Integer::New(isolate, zn_chatMsg.chatMessageType));
	node->Set(context, ToV8String(isolate, "isComment"), v8::Boolean::New(isolate, zn_chatMsg.isComment));
	node->Set(context, ToV8String(isolate, "isThread"), v8::Boolean::New(isolate, zn_chatMsg.isThread));
	node->Set(context, ToV8String(isolate, "threadID"), ToV8String(isolate, zs2s(zn_chatMsg.threadID)));

	v8::Local<v8::Array> styleItemNodes = v8::Array::New(isolate);
	for (unsigned int i = 0; i < zn_chatMsg.textStyleItemList.size(); ++i)
	{
		v8::HandleScope scope(isolate);
		v8::Local<v8::Object> style_node = v8::Object::New(isolate);
		style_node->Set(context, ToV8String(isolate, "textStyle"), v8::Integer::New(isolate, zn_chatMsg.textStyleItemList[i].textStyle));

		v8::Local<v8::Array> styleOffsetNodes = v8::Array::New(isolate);
		std::vector<ZNRichTextStyleOffset> textStyleOffsetList = zn_chatMsg.textStyleItemList[i].textStyleOffsetList;
		for (unsigned int j = 0; j < textStyleOffsetList.size(); ++j) 
		{
			v8::HandleScope scope(isolate);
			v8::Local<v8::Object> offset_node = v8::Object::New(isolate);
			offset_node->Set(context, ToV8String(isolate, "positionStart"), v8::Integer::New(isolate, textStyleOffsetList[j].positionStart));
			offset_node->Set(context, ToV8String(isolate, "positionEnd"), v8::Integer::New(isolate, textStyleOffsetList[j].positionEnd));
			offset_node->Set(context, ToV8String(isolate, "reserve"), ToV8String(isolate, zs2s(textStyleOffsetList[j].reserve)));
			styleOffsetNodes->Set(context, j, offset_node);
		}
		style_node->Set(context, ToV8String(isolate, "textStyleOffsetList"), styleOffsetNodes);
		styleItemNodes->Set(context, i, style_node);
	}
	node->Set(context, ToV8String(isolate, "textStyleItemList"), styleItemNodes);

	return node;
}

v8::Local<v8::Array> NodeWrapUtil::CreateVirtualBGImageInfoObjectList(v8::Isolate* isolate, ZNList<ZNVirtualBGImageInfo>& zn_list) {
	auto context = isolate->GetCurrentContext();
	v8::Local<v8::Array> nodes = v8::Array::New(isolate, zn_list.size());

	for (unsigned int i = 0; i < zn_list.size(); i++) 
	{
		v8::HandleScope scope(isolate);
		v8::Local<v8::Object> node = v8::Object::New(isolate);
		ZNVirtualBGImageInfo zn_imageinfo = zn_list[i];
		node->Set(context, ToV8String(isolate, "isSelected"), v8::Boolean::New(isolate, zn_imageinfo.isSelected));
		node->Set(context, ToV8String(isolate, "isAllowDelete"), v8::Boolean::New(isolate, zn_imageinfo.isAllowDelete));
		node->Set(context, ToV8String(isolate, "imageFilePath"), ToV8String(isolate, zs2s(zn_imageinfo.imageFilePath)));
		node->Set(context, ToV8String(isolate, "imageName"), ToV8String(isolate, zs2s(zn_imageinfo.imageName)));
		node->Set(context, ToV8String(isolate, "imageHandle"), v8::BigInt::New(isolate, zn_imageinfo.imageHandle));
		nodes->Set(context, i, node);
	}
	return nodes;
}

v8::Local<v8::Object> NodeWrapUtil::CreateZoomSDKDocSharingSourceInfoObject(v8::Isolate* isolate, const ZNZoomSDKDocSharingSourceInfo& zn_sourceinfo) {
	auto context = isolate->GetCurrentContext();
	v8::Local<v8::Object> node = v8::Object::New(isolate);

    node->Set(context, ToV8String(isolate, "status"), v8::Integer::New(isolate, (int32_t)zn_sourceinfo.status));
    node->Set(context, ToV8String(isolate, "docTitle"), ToV8String(isolate, zs2s(zn_sourceinfo.docTitle)));
    node->Set(context, ToV8String(isolate, "shareSourceID"), v8::Integer::New(isolate, zn_sourceinfo.shareSourceID));
    node->Set(context, ToV8String(isolate, "userID"), v8::Integer::New(isolate, zn_sourceinfo.userID));

    return node;
}