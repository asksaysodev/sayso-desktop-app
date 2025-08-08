#pragma once
#include "common_include.h"
#include <map>
#include <string>
BEGIN_ZOOM_SDK_NAMESPACE
class ICustomizedResourceHelperWrap : public ICustomizedResourceHelper
{
public:
	static ICustomizedResourceHelperWrap& GetInst() { static ICustomizedResourceHelperWrap inst; return inst; }; 
	virtual SDKError AddCustomizedPictureResource(const zchar_t* key_, const zchar_t* absolute_resource_path);
	virtual SDKError AddCustomizedStringResource(SDKCustomizedStringType customizedType, const zchar_t* customizedString);
	virtual SDKError AddCustomizedURLResource(SDKCustomizedURLType customizedType, const zchar_t* customizedUrl);

	void ApplyAll();
private:
	std::map<std::wstring, std::wstring> cache_customized_pic_map;
	std::map<SDKCustomizedStringType, std::wstring> cache_customized_str_map;
	std::map<SDKCustomizedURLType, std::wstring> cache_customized_url_map;
};
END_ZOOM_SDK_NAMESPACE