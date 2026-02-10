//#include "pch.h"
#include "algorithm.h"
#include <iostream>

Calculator::Calculator() noexcept : lastStatus(CalcStatus::Success) {}

Calculator::CalcResult Calculator::calculate(double a, char op, double b) noexcept {
    CalcResult result = { 0.0, CalcStatus::Success };

    switch (op) {
    case '+':
        result.value = a + b;
        break;
    case '-':
        result.value = a - b;
        break;
    case '*':
        result.value = a * b;
        break;
    case '/':
        if (b == 0.0) {
            result.status = CalcStatus::DivideByZero;
            result.value = 0.0;
        } else {
            result.value = a / b;
        }
        break;
        // --- 新增：指數運算 (a 的 b 次方) ---
    case '^':
        result.value = std::pow(a, b);
        break;

    // --- 新增：方根運算 (b 的 a 次方根) ---
    // 例如 a=2, b=16，結果就是 4
    case 's':
        if (a < 0 && (int)b % 2 == 0) {
            // 如果是偶數次方根且數字是負的（如根號 -4），在實數領域無解
            result.status = CalcStatus::NegativeRoot;
        }
        else {
            // 開 a 次方根等於 1/a 次方
            result.value = std::pow(a, 1.0 / b);
        }
        break;
    default:
        result.status = CalcStatus::InvalidOperator;
        result.value = 0.0;
        break;
    }

    // 檢查運算結果是否溢位 (太大或太小到電腦無法表示)
    if (std::isinf(result.value)) {
        result.status = CalcStatus::Overflow;
    }


    lastStatus = result.status; // 更新最後一次狀態
    return result;
}

const char* Calculator::statusToText(Calculator::CalcStatus s) noexcept {
    switch (s) {
    case Calculator::CalcStatus::Success:         return u8"計算成功";
    case Calculator::CalcStatus::DivideByZero:    return u8"錯誤：不能除以零";
    case Calculator::CalcStatus::NegativeRoot:    return u8"錯誤：負數無法開根號";
    case Calculator::CalcStatus::InvalidOperator: return u8"錯誤：無效的運算符";
    case Calculator::CalcStatus::Overflow:        return u8"錯誤：結果不是有限數 (溢位或 NaN)";
    default:                                      return u8"發生未知錯誤";
    }
}


Calculator::CalcStatus Calculator::getLastStatus() const noexcept {
    return lastStatus;
}