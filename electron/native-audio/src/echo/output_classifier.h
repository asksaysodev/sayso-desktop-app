#pragma once

#include <algorithm>
#include <cctype>
#include <string>

#include "echo_policy.h"

namespace sayso::echo {

enum class Transport { kBuiltIn, kBluetooth, kUsb, kDisplay, kAirPlay, kVirtual, kOther };

inline OutputKind ClassifyOutput(Transport transport, const std::string& dataSource, const std::string& name) {
  switch (transport) {
    case Transport::kBuiltIn:
      if (dataSource == "headphones") return OutputKind::kHeadphones;
      if (dataSource == "speaker") return OutputKind::kSpeakers;
      return OutputKind::kUnknown;
    case Transport::kDisplay:
    case Transport::kAirPlay:
      return OutputKind::kSpeakers;
    case Transport::kVirtual:
      return OutputKind::kUnknown;
    case Transport::kBluetooth:
    case Transport::kUsb:
    case Transport::kOther:
      break;
  }
  std::string n = name;
  std::transform(n.begin(), n.end(), n.begin(), [](unsigned char ch) { return static_cast<char>(std::tolower(ch)); });
  for (const char* word : {"headphone", "headset", "airpods", "earbud", "earphone", "earpods", "buds"}) {
    if (n.find(word) != std::string::npos) return OutputKind::kHeadphones;
  }
  return OutputKind::kUnknown;
}

}
