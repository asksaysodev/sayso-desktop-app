{
'variables':{
'module_name': "zoomsdk",
'module_mac': "./../sdk/mac",
"module_win32":"./../sdk/win32",
"module_win64":"./../sdk/win64",
'module_32pdbfile_name':"./../sdk/win32/zoomsdk.pdb",
'module_64pdbfile_name':"./../sdk/win64/zoomsdk.pdb",

'render_module_name': "zoomsdk_render",
'render_module_32pdbfile_name':"./../sdk/win32/zoomsdk_render.pdb",
'render_module_64pdbfile_name':"./../sdk/win64/zoomsdk_render.pdb",

'napi_util_module_name': "zoomsdk_napi_util",
},
      "targets":[
	  	 {
 		'target_name':"<(napi_util_module_name)",
		'conditions':[
		  [
		  'OS=="mac"',
		  {
		      'link_settings':{
			   'include_dirs':[
			   "./lib/node_add_on",
				],
			   },
		    "defines":['__MACOS__'],
   		    "xcode_settings":{
                    "DEBUG_INFORMATION_FORMAT": "dwarf-with-dsym",
                    "BUILD_DIR":"<(module_mac)",
                    "OTHER_CPLUSPLUSFLAGS" : [ '-ObjC++', "-std=c++20", "-stdlib=libc++",  '-fvisibility=hidden','-frtti'],
                    "DEPLOYMENT_POSTPROCESSING": "YES",
                    "VALID_ARCHS": "x86_64 arm64",

	        }
		  },
		  ],
		  [
		  'OS=="win" and target_arch=="x64"',
		  {
		   'product_dir':"<(module_win64)",
		  }
		  ],
		  [
		  'OS=="win" and target_arch=="ia32"',
		  {
			'product_dir':"<(module_win32)",
		  }
		  ],
		],
		"sources":[ "./lib/node_add_on/zoom_sdk_napi_util.cpp" ]
		},
      {
      'target_name':"<(module_name)",
      
      'conditions':[
      [
		  'OS=="mac"',
		  {
		      # 'product_dir':"<(module_mac)",
		      "dependencies": [ "<(napi_util_module_name)" ],
			  'sources':[
			     "./lib/node_add_on/mac/zoom_native_sdk_wrap_core.mm",
			     "./lib/node_add_on/mac/auth_service_wrap_core.mm",
			     "./lib/node_add_on/zoom_node_addon.cpp",
			     "./lib/node_add_on/mac/sdk_native_error.mm",
			     "./lib/node_add_on/mac/preMeetingServiceDelegate.mm",
			     "./lib/node_add_on/mac/meeting_service_wrap_core.mm",
			     "./lib/node_add_on/mac/meetingServiceDelegate.mm",
			     "./lib/node_add_on/mac/meeting_ui_service.mm",
			     "./lib/node_add_on/mac/meeting_as_service.mm",
			     "./lib/node_add_on/mac/meeting_audio_service.mm",
			     "./lib/node_add_on/mac/meeting_video_service.mm",
			     "./lib/node_add_on/mac/meeting_participants_service.mm",
			     "./lib/node_add_on/mac/zoom_setting_service.mm",
			     "./lib/node_add_on/mac/setting_video_service.mm",
			     "./lib/node_add_on/mac/setting_audio_service.mm",
			     "./lib/node_add_on/mac/setting_general_service.mm",
			     "./lib/node_add_on/mac/setting_recording_service.mm",
			     "./lib/node_add_on/mac/setting_wallpaper_service.mm",
			     "./lib/node_add_on/mac/setting_virtual_bg_service.mm",
			     "./lib/node_add_on/mac/customized_resource_wrap_core.mm",
			     "./lib/node_add_on/mac/meeting_share_service.mm",
			     "./lib/node_add_on/mac/meeting_H323_service.mm",
			     "./lib/node_add_on/mac/meeting_config_service.mm",
			     "./lib/node_add_on/mac/meeting_premeeting_service.mm",
			     "./lib/node_add_on/mac/task_to_main.mm",
			     "./lib/node_add_on/mac/zoom_sms_wrap.mm",
			     "./lib/node_add_on/mac/settingServiceDelegate.mm",
			     "./lib/node_add_on/mac/setting_statistic_service.mm",
			     "./lib/node_add_on/mac/setting_accessibility_service.mm",
			     "./lib/node_add_on/mac/meeting_recording_service.mm",
			      "./lib/node_add_on/mac/rawdata_api_wrap_core.mm",
			      "./lib/node_add_on/mac/rawdataDelegate.mm",
			      "./lib/node_add_on/mac/setting_shareScreen_service.mm",
			      "./lib/node_add_on/mac/meeting_appSignal_service.mm",
			      "./lib/node_add_on/mac/meeting_raw_archiving_wrap_core.mm",
			      "./lib/node_add_on/mac/liveStreamHelperDelegate.mm",
			      "./lib/node_add_on/mac/meeting_live_stream_wrap_core.mm",
				  "./lib/node_add_on/mac/meeting_reminder_wrap_core.mm",
				  "./lib/node_add_on/mac/meeting_chat_wrap_core.mm",
				  "./lib/node_add_on/mac/meeting_waiting_room_wrap_core.mm",
				  "./lib/node_add_on/mac/meeting_ai_companion_wrap_core.mm",
				  "./lib/node_add_on/mac/meeting_indicator_wrap_core.mm",
				  "./lib/node_add_on/mac/meeting_webinar_wrap_core.mm",
				  "./lib/node_add_on/mac/meeting_qa_wrap_core.mm",
				  "./lib/node_add_on/mac/meeting_reaction_wrap_core.mm",
				  "./lib/node_add_on/mac/meeting_close_caption_wrap_core.mm",
				  "./lib/node_add_on/mac/meeting_polling_wrap_core.mm",
				  "./lib/node_add_on/mac/meeting_whiteboard_wrap_core.mm",
				  "./lib/node_add_on/mac/meeting_docs_wrap_core.mm",
				  
				  "./lib/node_add_on/zoom_node_wrap_util.cpp",
				  "./lib/node_add_on/zoom_node_setting_share_ctrl.cpp",
				  "./lib/node_add_on/zoom_node_rawdata_api_ctrl.cpp",
				  "./lib/node_add_on/zoom_node_rawdata_helper_mgr_class.cpp",
				  "./lib/node_add_on/electron_sdk.pb.cc",
				  "./lib/node_add_on/zoom_node_meeting_recording_ctrl.cpp",
				  "./lib/node_add_on/raw_data_format.cpp",
				  "./lib/node_add_on/uv_ipc_common.cpp",
				  "./lib/node_add_on/raw_data_uv_ipc_server.cpp",
				  "./lib/node_add_on/zoom_node_setting_statistic_ctrl.cpp",
				  "./lib/node_add_on/zoom_node_setting_accessibility_ctrl.cpp",
				  "./lib/node_add_on/zoom_node_meeting_annotation.cpp",
				  "./lib/node_add_on/zoom_node_meeting_audio_ctrl.cpp",
				  "./lib/node_add_on/zoom_node_meeting_video_ctrl.cpp",
				  "./lib/node_add_on/zoom_node_meeting_participants_ctrl.cpp",
				  "./lib/node_add_on/zoom_node_meeting_share_ctrl.cpp",
				  "./lib/node_add_on/zoom_node_meeting_h323_ctrl.cpp",
				  "./lib/node_add_on/zoom_node_meeting_config_ctrl.cpp",
				  "./lib/node_add_on/zoom_node_setting_video_ctrl.cpp",
				  "./lib/node_add_on/zoom_node_setting_audio_ctrl.cpp",
				  "./lib/node_add_on/zoom_node_setting_general_ctrl.cpp",
				  "./lib/node_add_on/zoom_node_setting_recording_ctrl.cpp",
				  "./lib/node_add_on/zoom_node_setting_wallpaper_ctrl.cpp",
				  "./lib/node_add_on/zoom_node_setting_virtual_bg_ctrl.cpp",
				  "./lib/node_add_on/zoom_node_customized_resource.cpp",
                  "./lib/node_add_on/zoom_node_direct_share_helper.cpp",
				  "./lib/node_add_on/zoom_v8_to_c.cpp",
				  "./lib/node_add_on/run_task_to_main_thread.cpp",
				  "./lib/node_add_on/zoom_node_setting_ui_strategy_ctrl.cpp",
				  "./lib/node_add_on/zoom_node_sdk_sms_helper.cpp",
				  "./lib/node_add_on/zoom_node_meeting_AAN_ctrl.cpp",
				  "./lib/node_add_on/zoom_sdk_napi_util_exporter.cpp",
				  "./lib/node_add_on/zoom_node_meeting_raw_archiving_ctrl.cpp",
				  "./lib/node_add_on/zoom_node_meeting_live_stream_ctrl.cpp",
				  "./lib/node_add_on/zoom_node_meeting_reminder_ctrl.cpp",
				  "./lib/node_add_on/zoom_node_meeting_chat_ctrl.cpp",
				  "./lib/node_add_on/zoom_node_meeting_qa_ctrl.cpp",
				  "./lib/node_add_on/zoom_node_meeting_reaction_ctrl.cpp",
				  "./lib/node_add_on/zoom_node_meeting_close_caption_ctrl.cpp",
				  "./lib/node_add_on/zoom_node_meeting_polling_ctrl.cpp",
				  "./lib/node_add_on/zoom_node_meeting_waiting_room_ctrl.cpp",
				  "./lib/node_add_on/zoom_node_meeting_ai_companion_ctrl.cpp",
				  "./lib/node_add_on/zoom_node_meeting_indicator_ctrl.cpp",
				  "./lib/node_add_on/zoom_node_meeting_webinar_ctrl.cpp",
				  "./lib/node_add_on/zoom_node_meeting_whiteboard_ctrl.cpp",
				  "./lib/node_add_on/zoom_node_meeting_docs_ctrl.cpp"
			  ],
			  'mac_framework_dirs':[
			  './../sdk/mac/ZoomSDK',
			  ],
			  'mac_bundle_resources': [
     			"../sdk/mac/zoomsdk_napi_util.node",
     		  ],
			  'link_settings':{
			  'libraries':[
			   "ZoomSDK.framework",
			   "../sdk/mac/ZoomSDK/libprotobuf.dylib",
			   ],
			   'include_dirs':[
			   "./lib/node_add_on",
			   "./lib/node_add_on/protobuf_src",
				],
			   },
		      "defines":['__MACOS__'],
   		      "xcode_settings":{
                    "DEBUG_INFORMATION_FORMAT": "dwarf-with-dsym",
                    "BUILD_DIR":"<(module_mac)",
                    "OTHER_CPLUSPLUSFLAGS" : [ '-ObjC++', "-std=c++20", "-stdlib=libc++",  '-fvisibility=hidden','-frtti'],
                    "OTHER_LDFLAGS": [ "-stdlib=libc++"],
                    "DEPLOYMENT_POSTPROCESSING": "YES",
                    "VALID_ARCHS": "x86_64 arm64 arm64e",

	        }
			   
		  },
		  ],
		  [
		  'OS=="win" and target_arch=="x64"',
		  {
		   'product_dir':"<(module_win64)",
		   'configurations': {
				'Release': {
				  'msvs_settings': {
					'VCLinkerTool': {
					  'GenerateDebugInformation': 'true',
					  'GenerateMapFile': 'true',
					  'ProgramDatabaseFile': "<(module_64pdbfile_name)",
					},
					'VCCLCompilerTool': {
                     'RuntimeLibrary': '2',
                     "PreprocessorDefinitions": ["NOMINMAX"]
                    },
				  }, 
				},
		  }, # configurations
		  'link_settings':{
			  'libraries':[
			   "./../sdk/win64/libprotobuf-lite.lib",
			   "./../sdk/win64/zoomsdk_napi_util.lib",
			   ],
			   },
		  }
		  ],
		  [
		  'OS=="win" and target_arch=="ia32"',
		  {
		   'product_dir':"<(module_win32)",
		     'configurations': {
				'Release': {
				  'msvs_settings': {
					'VCLinkerTool': {
					  'GenerateDebugInformation': 'true',
					  'GenerateMapFile': 'true',
					  'ProgramDatabaseFile': "<(module_32pdbfile_name)",
					},
					'VCCLCompilerTool': {
                     'RuntimeLibrary': '2',
                     "PreprocessorDefinitions": ["NOMINMAX"]
                    },
				  }, 
				},
		  }, # configurations
		  'link_settings':{
			  'libraries':[
			   "./../sdk/win32/libprotobuf-lite.lib",
			   "./../sdk/win32/zoomsdk_napi_util.lib",
			   ],
			   },
		  }
		  ],
		  [
		  'OS=="win"',
		  {
		  "dependencies": [ "<(napi_util_module_name)" ],
		  "sources":[
			  "./lib/node_add_on/win/zoom_native_sdk_wrap_core.cpp",
			  "./lib/node_add_on/win/auth_service_wrap_core.cpp", 
			  "./lib/node_add_on/win/meeting_service_wrap_core.cpp",
			  "./lib/node_add_on/win/setting_service_wrap_core.cpp",
			  "./lib/node_add_on/win/meeting_annotation_wrap_core.cpp",
			  "./lib/node_add_on/win/meeting_audio_wrap_core.cpp",
			  "./lib/node_add_on/win/meeting_video_wrap_core.cpp",
			  "./lib/node_add_on/win/meeting_share_wrap_core.cpp",
			  "./lib/node_add_on/win/meeting_config_wrap_core.cpp",
			  "./lib/node_add_on/win/meeting_participants_wrap_core.cpp",
			  "./lib/node_add_on/win/meeting_h323_wrap_core.cpp",
			  "./lib/node_add_on/win/setting_video_wrap_core.cpp",
			  "./lib/node_add_on/win/setting_audio_wrap_core.cpp",
			  "./lib/node_add_on/win/setting_general_wrap_core.cpp",
			  "./lib/node_add_on/win/setting_ui_strategy_wrap_core.cpp",
			  "./lib/node_add_on/win/setting_share_wrap_core.cpp",
			  "./lib/node_add_on/win/meeting_recording_wrap_core.cpp",
			  "./lib/node_add_on/win/meeting_AAN_wrap_core.cpp",
			  "./lib/node_add_on/win/meeting_raw_archiving_wrap_core.cpp",
			  "./lib/node_add_on/win/meeting_qa_wrap_core.cpp",
			  "./lib/node_add_on/win/meeting_reaction_wrap_core.cpp",
			  "./lib/node_add_on/win/meeting_polling_wrap_core.cpp",
			  "./lib/node_add_on/win/meeting_close_caption_wrap_core.cpp",
			  "./lib/node_add_on/win/meeting_live_stream_wrap_core.cpp",
			  "./lib/node_add_on/win/meeting_reminder_wrap_core.cpp",
			  "./lib/node_add_on/win/meeting_chat_wrap_core.cpp",
			  "./lib/node_add_on/win/meeting_waiting_room_wrap_core.cpp",
			  "./lib/node_add_on/win/meeting_ai_companion_wrap_core.cpp",
			  "./lib/node_add_on/win/meeting_indicator_wrap_core.cpp",
			  "./lib/node_add_on/win/meeting_webinar_wrap_core.cpp",
			  "./lib/node_add_on/win/meeting_whiteboard_wrap_core.cpp",
			  "./lib/node_add_on/win/meeting_docs_wrap_core.cpp",
			  
			  "./lib/node_add_on/win/setting_statistic_wrap_core.cpp",
			  "./lib/node_add_on/win/setting_accessibility_wrap_core.cpp",
			  "./lib/node_add_on/win/setting_recording_wrap_core.cpp",
			  "./lib/node_add_on/win/setting_wallpaper_wrap_core.cpp",
			  "./lib/node_add_on/win/setting_virtual_bg_wrap_core.cpp",
			  "./lib/node_add_on/win/directshare_helper_wrap_core.cpp",
			  "./lib/node_add_on/win/customized_resource_wrap_core.cpp",
			  "./lib/node_add_on/win/zoom_sdk_sms_helper_wrap_core.cpp",
			  "./lib/node_add_on/zoom_node_addon.cpp",
			  "./lib/node_add_on/electron_sdk.pb.cc",
			  "./lib/node_add_on/node_res.rc",
			  "./lib/node_add_on/win/zoom_native_to_wrap.cpp",

			  "./lib/node_add_on/zoom_node_wrap_util.cpp",
			  "./lib/node_add_on/zoom_node_rawdata_api_ctrl.cpp",
			  "./lib/node_add_on/zoom_node_meeting_annotation.cpp",
			  "./lib/node_add_on/zoom_node_meeting_audio_ctrl.cpp",
			  "./lib/node_add_on/zoom_node_meeting_video_ctrl.cpp",
			  "./lib/node_add_on/zoom_node_meeting_participants_ctrl.cpp",
			  "./lib/node_add_on/zoom_node_meeting_share_ctrl.cpp",
			  "./lib/node_add_on/zoom_node_meeting_h323_ctrl.cpp",
			  "./lib/node_add_on/zoom_node_meeting_config_ctrl.cpp",
			  "./lib/node_add_on/zoom_node_meeting_recording_ctrl.cpp",
			  "./lib/node_add_on/zoom_node_setting_video_ctrl.cpp",
			  "./lib/node_add_on/zoom_node_setting_audio_ctrl.cpp",
			  "./lib/node_add_on/zoom_node_setting_general_ctrl.cpp",
			  "./lib/node_add_on/zoom_node_setting_ui_strategy_ctrl.cpp",
			  "./lib/node_add_on/zoom_node_setting_share_ctrl.cpp",
			  "./lib/node_add_on/zoom_node_setting_statistic_ctrl.cpp",
			  "./lib/node_add_on/zoom_node_setting_accessibility_ctrl.cpp",
			  "./lib/node_add_on/zoom_node_meeting_AAN_ctrl.cpp",
			  "./lib/node_add_on/zoom_node_meeting_raw_archiving_ctrl.cpp",
			  "./lib/node_add_on/zoom_node_meeting_live_stream_ctrl.cpp",
			  "./lib/node_add_on/zoom_node_meeting_reminder_ctrl.cpp",
			  "./lib/node_add_on/zoom_node_meeting_chat_ctrl.cpp",
			  "./lib/node_add_on/zoom_node_meeting_waiting_room_ctrl.cpp",
			  "./lib/node_add_on/zoom_node_meeting_ai_companion_ctrl.cpp",
			  "./lib/node_add_on/zoom_node_meeting_indicator_ctrl.cpp",
			  "./lib/node_add_on/zoom_node_meeting_webinar_ctrl.cpp",
			  "./lib/node_add_on/zoom_node_meeting_polling_ctrl.cpp",
			  "./lib/node_add_on/zoom_node_meeting_close_caption_ctrl.cpp",
			  "./lib/node_add_on/zoom_node_meeting_reaction_ctrl.cpp",
			  "./lib/node_add_on/zoom_node_meeting_qa_ctrl.cpp",
			  "./lib/node_add_on/zoom_node_meeting_whiteboard_ctrl.cpp",
			  "./lib/node_add_on/zoom_node_meeting_docs_ctrl.cpp",
			  
			  "./lib/node_add_on/zoom_node_setting_recording_ctrl.cpp",
			  "./lib/node_add_on/zoom_node_setting_wallpaper_ctrl.cpp",
			  "./lib/node_add_on/zoom_node_setting_virtual_bg_ctrl.cpp",
			  "./lib/node_add_on/zoom_node_customized_resource.cpp",
			  "./lib/node_add_on/zoom_node_direct_share_helper.cpp",
			  "./lib/node_add_on/zoom_v8_to_c.cpp",
			  "./lib/node_add_on/raw_data_format.cpp",
			  "./lib/node_add_on/uv_ipc_common.cpp",
			  "./lib/node_add_on/raw_data_uv_ipc_server.cpp",
			  "./lib/node_add_on/run_task_to_main_thread.cpp",
			  
			  "./lib/node_add_on/zoom_node_rawdata_helper_mgr_class.cpp",
			  "./lib/node_add_on/zoom_sdk_napi_util_exporter.cpp",
			  
			  "./lib/node_add_on/win/zoom_native_raw_api_wrap_core.cpp",
			  "./lib/node_add_on/win/zoom_native_raw_render_wrap_core.cpp",
			  "./lib/node_add_on/win/zoom_native_audio_rawdata_wrap_core.cpp",
			  "./lib/node_add_on/zoom_node_sdk_sms_helper.cpp"
		  ],
			  
			  'defines':['BUILD_WIN'],
			  
			   'include_dirs':[
			   "./lib/node_add_on/protobuf_src",
			   "./lib/node_add_on/win/wrap",
			   "./lib/node_add_on/win/h",
			   "./lib/node_add_on",
				],
		  
		  }
		  
		  ]

		 ]
      },
	 
	 {
	 'target_name':"<(render_module_name)",
	 
	 'conditions':[
	  [
		  'OS=="mac"',
		  {
		      # 'product_dir':"<(module_mac)",
		      "dependencies": [ "<(napi_util_module_name)" ],
			  'sources':[
			     "./lib/node_add_on/zoom_node_render_addon.cpp",
			     "./lib/node_add_on/zoom_v8_to_c.cpp",
			     "./lib/node_add_on/uv_ipc_common.cpp",
			     "./lib/node_add_on/zoom_v8_to_c.cpp",
			     "./lib/node_add_on/zoom_sdk_napi_util_exporter.cpp",
			     "./lib/node_add_on/electron_sdk.pb.cc",
			  ],
			  'mac_framework_dirs':[
			  './../sdk/mac/ZoomSDK',
			  ],
			  'mac_bundle_resources': [
     			"../sdk/mac/zoomsdk_napi_util.node",
     		],
			  'link_settings':{
			  'libraries':[
			   "ZoomSDK.framework",
			   "../sdk/mac/ZoomSDK/libprotobuf.dylib",

			   ],
			   'include_dirs':[
			   "./lib/node_add_on",
			   "./lib/node_add_on/protobuf_src",
				],
			   },
			   "defines":['__MACOS__'],
			   "xcode_settings":{
                    "DEBUG_INFORMATION_FORMAT": "dwarf-with-dsym",
                    "BUILD_DIR":"<(module_mac)",
                    "OTHER_CPLUSPLUSFLAGS" : [ '-ObjC++', "-std=c++20", "-stdlib=libc++",  '-fvisibility=hidden','-frtti'],
                    "OTHER_LDFLAGS": [ "-stdlib=libc++"],
                    "DEPLOYMENT_POSTPROCESSING": "YES",
                    "VALID_ARCHS": "x86_64 arm64 arm64e",
	        }
			   
		  },
		  ],
		  [
		  'OS=="win" and target_arch=="x64"',
		  {
		   'product_dir':"<(module_win64)",
		   'configurations': {
				'Release': {
				  'msvs_settings': {
					'VCLinkerTool': {
					  'GenerateDebugInformation': 'true',
					  'GenerateMapFile': 'true',
					  'ProgramDatabaseFile': "<(render_module_64pdbfile_name)",
					},
					'VCCLCompilerTool': {
                     'RuntimeLibrary': '2',
                     "PreprocessorDefinitions": ["NOMINMAX"]
                    },
				  }, 
				},
		  }, # configurations
		  'link_settings':{
			  'libraries':[
			   "./../sdk/win64/libprotobuf-lite.lib",
			   "./../sdk/win64/zoomsdk_napi_util.lib",

			   ],
			   },
		  }
		  ],
		  [
		  'OS=="win" and target_arch=="ia32"',
		  {
		   'product_dir':"<(module_win32)",
		     'configurations': {
				'Release': {
				  'msvs_settings': {
					'VCLinkerTool': {
					  'GenerateDebugInformation': 'true',
					  'GenerateMapFile': 'true',
					  'ProgramDatabaseFile': "<(render_module_32pdbfile_name)",
					},
					'VCCLCompilerTool': {
                     'RuntimeLibrary': '2',
                     "PreprocessorDefinitions": ["NOMINMAX"]
                    },
				  }, 
				},
		  }, # configurations
		  'link_settings':{
			  'libraries':[
			   "./../sdk/win32/libprotobuf-lite.lib",
			   "./../sdk/win32/zoomsdk_napi_util.lib",
			   ],
			   },
		  }
		  ],
		  [
		  'OS=="win"',
		  {
		  "dependencies": [ "<(napi_util_module_name)" ],
		  "sources":[
			  "./lib/node_add_on/electron_sdk.pb.cc",
			  "./lib/node_add_on/zoom_node_render_addon.cpp",
			  "./lib/node_add_on/node_res.rc",
			  "./lib/node_add_on/win/zoom_native_to_wrap.cpp",
			  
			  "./lib/node_add_on/zoom_v8_to_c.cpp",
			 
			  "./lib/node_add_on/uv_ipc_common.cpp",
			  "./lib/node_add_on/zoom_sdk_napi_util_exporter.cpp",
			  
		  ],
			  
			'defines':['BUILD_WIN'],  
			
			   'include_dirs':[
			   "./lib/node_add_on/protobuf_src",
			   "./lib/node_add_on/win/wrap",
			   "./lib/node_add_on/win/h",
			   "./lib/node_add_on",
				],
		  }
		  
		  ]
	 ]
	 },
	 
	  

      ]
}
