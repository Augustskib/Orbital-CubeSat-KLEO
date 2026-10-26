#pragma once
#include <cstdint>

using F64 = double;
using F32 = float;
using U32 = std::uint32_t;
using FwIndexType = unsigned;
using FwOpcodeType = unsigned;

namespace Fw {
struct Time {
    std::uint64_t microseconds;
    U32 getSeconds() const {
        return static_cast<U32>(microseconds / 1000000);
    }
    U32 getUSeconds() const {
        return static_cast<U32>(microseconds % 1000000);
    }
};
enum class CmdResponse { OK };
}
