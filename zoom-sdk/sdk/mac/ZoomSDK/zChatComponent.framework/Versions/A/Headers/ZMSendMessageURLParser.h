//
//  ZMSendMessageURLParser.h
//  zChatComponent
//
//  Created by Cyan.Yang on 2024/2/23.
//

#ifndef ZMSendMessageURLDetection_h
#define ZMSendMessageURLDetection_h

#include <algorithm>

#include <zChatApp/IZoomMessenger.h>
#include <cmmlib/CmmBase.h>

namespace zoom {

class SendMessageURLParser {
    using PositionType = CmmInt64;
    
    struct Range {
        PositionType start;
        PositionType end;
    };
    
    const bool IsRangeAvailable(const Range range) const {
        for (auto occupied_range : occupied_ranges_) {
            if (occupied_range.start > range.end) {
                // No more possible overlapping ranges since
                // |occupied_ranges_| is sorted.
                break;
            }
            
            if (occupied_range.end < range.start) {
                continue;
            }
            
            // An overlapping range is found.
            return false;
        }
        
        return true;
    }
    
 public:
    explicit SendMessageURLParser(NS_ZOOM_MESSAGER::IZoomMessenger &messenger,
                                  NS_ZOOM_MESSAGER::MapStyleOffset &style_map)
        : messenger_(messenger), style_map_(style_map)
    {
        // Preprocess the existed style map for faster overlapping range checks.
        auto iter = style_map.find(FontStyle_InsertLink);
        if (iter != style_map.end()) {
            const auto &offsets = iter->second;
            for (const auto &offset : offsets) {
                Range range = { offset._position_start, offset._position_end };
                occupied_ranges_.push_back(range);
            }
            std::sort(occupied_ranges_.begin(), occupied_ranges_.end(), [](auto a, auto b) {
                return a.start < b.start;
            });
        }
    }
    
    void ParseBody(const CString &body, const std::vector<NS_ZOOM_MESSAGER::MessageAtInfo> &atInfoList) {
        auto business_mgr = messenger_.GetZoomChatBusinessMgrPtr();
        if (!business_mgr) {
            return;
        }
        
        std::vector<NS_ZOOM_MESSAGER::MsgHighlightInfo> parsed_highlight_info;
        if (!business_mgr->ParseHighlightInfoInMsgWithAtInfo(body, atInfoList, parsed_highlight_info)) {
            return;
        }
        
        std::vector<NS_ZWEBSERVICE::StyleOffset> style_offsets;
        for (const auto &info : parsed_highlight_info) {
            Range range = { info.startpos, info.endpos };
            if (!IsRangeAvailable(range)) {
                continue;
            }
            
            style_offsets.emplace_back();
            auto &style_offset = style_offsets.back();
            style_offset._reserve1 = std::to_string((int) info.type);
            style_offset._position_start = (int) range.start;
            style_offset._position_end = (int) range.end;
        }
        if (!style_offsets.empty()) {
            style_map_.insert({ FontStyle_HighlightInfo, std::move(style_offsets) });
        }
    }
    
 private:
    NS_ZOOM_MESSAGER::IZoomMessenger &messenger_;
    NS_ZOOM_MESSAGER::MapStyleOffset &style_map_;
    
    std::vector<Range> occupied_ranges_;
};

} // namespace zoom

#endif /* ZMSendMessageURLDetection_h */
