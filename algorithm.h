// algorithm.h

#pragma once
#include <cmath> // for std::isfinite in implementations if needed (header-only include is harmless but optional)
#include <vector>

using DoubleList = std::vector<double>;
using CharList = std::vector<char>;

class Calculator {
public:
    // 定義錯誤類型，方便 UI 顯示錯誤訊息
    enum class CalcStatus {
        Success = 0,
        DivideByZero,    // 除以零
        NegativeRoot,    // 負數開根號 (若未來擴充)
        InvalidOperator, // 無效的符號
        Overflow,        // 數字太大或非有限數
        UnknownError     // 未知錯誤
    }; // <--- 這裡加分號

    // 定義一個結構來同時攜帶「數值」與「狀態」
    struct CalcResult {
        double value;       // 計算結果
        CalcStatus status;  // 狀態碼
    }; // <--- 這裡加分號

    Calculator() noexcept; // 構造函數

    /**
      * 核心計算函式
      * @param a 第一個數字
      * @param op 運算符號 ('+', '-', '*', '/','^','s')
      * @param b 第二個數字
      * @return 包含結果與狀態的結構體
      */
    CalcResult calculate(double a, char op, double b) noexcept;

    /**
      * 靜態工具函式：將列舉轉為文字 (方便 UI 顯示)
      */
    static const char* statusToText(CalcStatus s) noexcept;

    CalcStatus getLastStatus() const noexcept;


    // 新增：處理連續運算
    // numbers: 數字列表 (例如 {10, 5, 2})
    // ops: 運算符列表 (例如 {'-', '-'})
    //CalcResult calculateSequence(std::vector<double> numbers, std::vector<char> ops) noexcept;


    CalcResult calculateSequence(DoubleList numbers, CharList ops) noexcept;

private:
    CalcStatus lastStatus = CalcStatus::Success;

    // 輔助函數：判斷運算符是否為右結合
    bool isRightAssociative(char op) const noexcept;

#ifdef ALGORITHM_IMPLEMENTATION

    CalcResult solveLeft(DoubleList nums, CharList ops) noexcept;
    CalcResult solveRight(DoubleList nums, CharList ops) noexcept;
#endif

};