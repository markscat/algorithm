//#include "pch.h"

#include "algorithm.h"
#include <iostream>
#include <cmath>
#include <vector>
#include <stack>


// 為了讓代碼更乾淨，我們在 .cpp 頂部定義一些短名稱
using Result = Calculator::CalcResult;
using Status = Calculator::CalcStatus;

Calculator::Calculator() noexcept : lastStatus(CalcStatus::Success) {}


// 判斷是否為右結合律 (目前只有指數 '^' 是右結合)
bool Calculator::isRightAssociative(char op) const noexcept {
    return op == '^';
}
#define ALGORITHM_IMPLEMENTATION_2

#ifdef ALGORITHM_IMPLEMENTATION_2
/*
Calculator::CalcResult Calculator::calculateSequence(std::vector<double> numbers, std::vector<char> ops) noexcept {
if (numbers.empty()) return { 0.0, CalcStatus::Success };
if (ops.empty()) return { numbers[0], CalcStatus::Success };
if (numbers.size() != ops.size() + 1) {
    lastStatus = CalcStatus::InvalidOperator; // 數量不匹配
    return { 0.0, lastStatus 
    };
}*/


Calculator::CalcResult Calculator::calculateSequence(DoubleList nums, CharList ops) noexcept {
    // --- 所有的判斷都進來函數後才做 ---

    // 1. 基本安全檢查
    if (nums.empty()) return { 0.0, CalcStatus::Success };
    if (ops.empty())  return { nums[0], CalcStatus::Success };
    if (nums.size() != ops.size() + 1) {
        lastStatus = CalcStatus::InvalidOperator; // 數量不匹配
        return { 0.0, lastStatus };
    }


    // 2. 初始化結果
    CalcResult result = { 0.0, CalcStatus::Success };

    // 3. 判斷結合律並直接計算
    // 如果第一個符號是 '^'，就走「右結合」邏輯
    if (ops[0] == '^') {
        double currentVal = nums.back();
        for (int i = (int)ops.size() - 1; i >= 0; --i) {
            result = calculate(nums[i], ops[i], currentVal);
            currentVal = result.value;
            if (result.status != CalcStatus::Success) break;
        }
    }
    // 否則，一律走「左結合」邏輯 (加減乘除)
    else {
        result.value = nums[0];
        for (size_t i = 0; i < ops.size(); ++i) {
            result = calculate(result.value, ops[i], nums[i + 1]);
            if (result.status != CalcStatus::Success) break;
        }
    }

    return result;
}
#endif

#ifdef ALGORITHM_IMPLEMENTATION
// --- 主功能：指揮官 ---
Result Calculator::calculateSequence(DoubleList nums, CharList ops) noexcept {
    // 1. 基本檢查 (防呆)
    if (nums.empty()) return { 0.0, Status::Success };
    if (ops.empty())  return { nums[0], Status::Success };
    if (nums.size() != ops.size() + 1) return { 0.0, Status::InvalidOperator };

    // 2. 根據第一個符號決定「計算方向」
    if (isRightAssociative(ops[0])) {
        return solveRight(nums, ops); // 右結合
    }
    else {
        return solveLeft(nums, ops);  // 左結合
    }
}

// --- 輔助功能：處理左結合 (如 + - * /) ---
Result Calculator::solveLeft(DoubleList nums, CharList ops) noexcept {
    Result res = { nums[0], Status::Success };

    for (size_t i = 0; i < ops.size(); ++i) {
        res = calculate(res.value, ops[i], nums[i + 1]);
        if (res.status != Status::Success) break;
    }
    return res;
}

// --- 輔助功能：處理右結合 (如 ^ 指數) ---
Result Calculator::solveRight(DoubleList nums, CharList ops) noexcept {
    double currentVal = nums.back();
    Status currentStatus = Status::Success;

    for (int i = (int)ops.size() - 1; i >= 0; --i) {
        Result res = calculate(nums[i], ops[i], currentVal);
        currentVal = res.value;
        currentStatus = res.status;
        if (currentStatus != Status::Success) break;
    }
    return { currentVal, currentStatus };
}

#endif

Calculator::CalcResult Calculator::calculate(double a, char op, double b) noexcept {
    //成員函式 "Calculator::calculate" 不能在其類別之外重新宣告
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