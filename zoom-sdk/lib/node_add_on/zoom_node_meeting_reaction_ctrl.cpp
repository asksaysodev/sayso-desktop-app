#include "zoom_node_meeting_reaction_ctrl.h"

ZoomNodeMeetingReactionCtrlWrap::ZoomNodeMeetingReactionCtrlWrap()
{
	_g_native_wrap.GetMeetingServiceWrap().GetMeetingReactionCtrl().SetSink(&ZoomNodeSinkWrapMgr::GetInst().m_meetingReactionWrapSink);
}

ZoomNodeMeetingReactionCtrlWrap::~ZoomNodeMeetingReactionCtrlWrap()
{
	_g_native_wrap.GetMeetingServiceWrap().GetMeetingReactionCtrl().SetSink(NULL);
}

void ZoomNodeMeetingReactionCtrlWrap::SetOnEmojiReactionReceivedCB(const v8::FunctionCallbackInfo<v8::Value>& args)
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
			ZoomNodeSinkHelper::GetInst().onEmojiReactionReceived.Clear();
			break;
		}
		if (!args[0]->IsFunction())
		{
			err = ZNSDKERR_INVALID_PARAMETER;
			break;
		}

		v8::Local<v8::Function> cbfunc = v8::Local<v8::Function>::Cast(args[0]);
		ZoomNodeSinkHelper::GetInst().onEmojiReactionReceived.Reset(isolate, cbfunc);

	} while (false);

	v8::Local<v8::Integer> bret = v8::Integer::New(isolate, (int32_t)err);
	args.GetReturnValue().Set(bret);
}

void ZoomNodeMeetingReactionCtrlWrap::SetOnEmojiReactionReceivedInWebinarCB(const v8::FunctionCallbackInfo<v8::Value>& args)
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
			ZoomNodeSinkHelper::GetInst().onEmojiReactionReceivedInWebinar.Clear();
			break;
		}
		if (!args[0]->IsFunction())
		{
			err = ZNSDKERR_INVALID_PARAMETER;
			break;
		}

		v8::Local<v8::Function> cbfunc = v8::Local<v8::Function>::Cast(args[0]);
		ZoomNodeSinkHelper::GetInst().onEmojiReactionReceivedInWebinar.Reset(isolate, cbfunc);

	} while (false);

	v8::Local<v8::Integer> bret = v8::Integer::New(isolate, (int32_t)err);
	args.GetReturnValue().Set(bret);
}