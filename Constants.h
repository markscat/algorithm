#pragma once
//Constants.h
namespace CalcConstants {
    // 數學常數 (手動定義高精度數值)
    static constexpr double PI = 3.14159265358979323846;
    static constexpr double E = 2.71828182845904523536;
    static constexpr double SQRT2 = 1.41421356237309504880;

    // 物理常數 (依據國際標準單位)
    namespace Physics {
        static constexpr double Planck = 6.62607015e-34;   // 普朗克常數 (J·s)
        static constexpr double LightSpeed = 299792458.0; // 光速 (m/s)
        static constexpr double Gravity = 6.67430e-11;    // 萬有引力常數 (N·m²/kg²)
        static constexpr double ElementaryCharge = 1.602176634e-19; // 基本電荷 (C)

    }


// 化學常數 (依據 SI 標準)
    namespace Chemistry {
        static constexpr double Avogadro = 6.02214076e23;     // 阿伏伽德羅常數 (mol⁻¹)
        static constexpr double GasConstant = 8.314462618;    // 理想氣體常數 (J/(mol·K))
        static constexpr double Boltzmann = 1.380649e-23;     // 玻爾茲曼常數 (J/K)
        static constexpr double Faraday = 96485.33212;       // 法拉第常數 (C/mol)
        static constexpr double AtomicMassUnit = 1.66053906660e-27; // 原子質量單位 (kg)
    }


// 統計學與機率論常用常數
    namespace Statistics {
        // 歐拉-馬斯刻若尼常數 (Euler-Mascheroni constant)，常用於分佈計算
        static constexpr double EulerGamma = 0.57721566490153286060;

        // 標準常態分佈常用值 (1 / sqrt(2 * PI))
        static constexpr double InvSqrt2PI = 0.39894228040143267794;

        // 自然對數常用值 (對數轉換、半衰期或機率分佈轉換常用)
        static constexpr double LN2 = 0.693147180559945309417;
        static constexpr double LN10 = 2.30258509299404568402;
    }
}