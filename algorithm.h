// algorithm.h

#pragma once
#include <cmath> // for std::isfinite in implementations if needed (header-only include is harmless but optional)

class Calculator {
public:
    // 定義錯誤類型，方便 UI 顯示錯誤訊息
    enum class CalcStatus {
        Success = 0,
        DivideByZero,    // 除以零
        NegativeRoot,    // 負數開根號 (若未來擴充)
        InvalidOperator, // 無效的符號
        Overflow,        // 數字太大或非有限數
        //NegativeRoot,    // 負數開根號
        //Overflow,        // 數值溢位（數字太大）
        UnknownError     // 未知錯誤
    };

    // 定義一個結構來同時攜帶「數值」與「狀態」
    struct CalcResult {
        double value;       // 計算結果
        CalcStatus status;  // 狀態碼
    };

    Calculator() noexcept; // 構造函數

    /**
      * 核心計算函式
      * @param a 第一個數字
      * @param op 運算符號 ('+', '-', '*', '/')
      * @param b 第二個數字
      * @return 包含結果與狀態的結構體
      */
    CalcResult calculate(double a, char op, double b) noexcept;

    /**
      * 靜態工具函式：將列舉轉為文字 (方便 UI 顯示)
      */
    static const char* statusToText(CalcStatus s) noexcept;

    CalcStatus getLastStatus() const noexcept;

private:
    CalcStatus lastStatus = CalcStatus::Success;
};