#include "zoom_node_meeting_close_caption_ctrl.h"

ZoomNodeCloseCaptionCtrlWrap::ZoomNodeCloseCaptionCtrlWrap()
{
	_g_native_wrap.GetMeetingServiceWrap().GetMeetingCloseCaptionCtrl().SetSink(&ZoomNodeSinkWrapMgr::GetInst().m_closeCaptionWrapSink);
}

ZoomNodeCloseCaptionCtrlWrap::~ZoomNodeCloseCaptionCtrlWrap()
{
	_g_native_wrap.GetMeetingServiceWrap().GetMeetingCloseCaptionCtrl().SetSink(NULL);
}

void ZoomNodeCloseCaptionCtrlWrap::SetOnLiveTranscriptionMsgInfoReceivedCB(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	v8::Isolate* isolate = args.GetIsolate();
	ZNSDKError err = ZNSDKERR_SUCCESS;
	do
	{
		if (args.Length() < 1) {
			err = ZNSDKERR_INVALID_PARAMETER;
			break;
		}
		if (args[0]->IsNull())
		{
			ZoomNodeSinkHelper::GetInst().onLiveTranscriptionMsgInfoReceived.Clear();
			break;
		}
		if (!args[0]->IsFunction())
		{
			err = ZNSDKERR_INVALID_PARAMETER;
			break;
		}

		v8::Local<v8::Function> cbfunc = v8::Local<v8::Function>::Cast(args[0]);
		ZoomNodeSinkHelper::GetInst().onLiveTranscriptionMsgInfoReceived.Reset(isolate, cbfunc);

	} while (false);

	v8::Local<v8::Integer> bret = v8::Integer::New(isolate, (int32_t)err);
	args.GetReturnValue().Set(bret);
}

void ZoomNodeCloseCaptionCtrlWrap::SetOnOriginalLanguageMsgReceivedCB(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	v8::Isolate* isolate = args.GetIsolate();
	ZNSDKError err = ZNSDKERR_SUCCESS;
	do
	{
		if (args.Length() < 1) {
			err = ZNSDKERR_INVALID_PARAMETER;
			break;
		}
		if (args[0]->IsNull())
		{
			ZoomNodeSinkHelper::GetInst().onOriginalLanguageMsgReceived.Clear();
			break;
		}
		if (!args[0]->IsFunction())
		{
			err = ZNSDKERR_INVALID_PARAMETER;
			break;
		}

		v8::Local<v8::Function> cbfunc = v8::Local<v8::Function>::Cast(args[0]);
		ZoomNodeSinkHelper::GetInst().onOriginalLanguageMsgReceived.Reset(isolate, cbfunc);

	} while (false);

	v8::Local<v8::Integer> bret = v8::Integer::New(isolate, (int32_t)err);
	args.GetReturnValue().Set(bret);
}