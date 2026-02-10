#pragma once
#pragma once

namespace CalcConstants {
    // 數學常數 (手動定義高精度數值)
    inline constexpr double PI = 3.14159265358979323846;
    inline constexpr double E = 2.71828182845904523536;
    inline constexpr double SQRT2 = 1.41421356237309504880;

    // 物理常數 (依據國際標準單位)
    namespace Physics {
        inline constexpr double Planck = 6.62607015e-34;   // 普朗克常數 (J·s)
        inline constexpr double LightSpeed = 299792458.0; // 光速 (m/s)
        inline constexpr double Gravity = 6.67430e-11;    // 萬有引力常數 (N·m²/kg²)
    }
}
