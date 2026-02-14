// algorithm_main.cpp
#include "algorithm.h"
#include "Trigonometry.h"
#include "Constants.h"

#include <iostream>
#include <cstdlib>
#include <string>
#include <vector>
#include <iomanip>
#include <sstream>



#ifdef _WIN32
#include <windows.h>
#endif

// --- [顯示層抽象化] ---

static void platform_log(const std::string& s) {
    // 這裡保留簡單的 cout，未來在 Qt 中可以改為 qDebug() 或發送訊號給 UI
    std::cout << s << std::flush;
}

// 格式化輸出結果


void displayResult(const std::string& expr, Calculator::CalcResult res, Calculator& engine) {
    std::stringstream ss;
    ss << "Expr   : " << expr << "\n";
    if (res.status == Calculator::CalcStatus::Success) {
        // 如果數字非常小或是非常大，自動切換到科學記號
        if (std::abs(res.value) < 1e-4 && res.value != 0)
            ss << "Result : " << std::scientific << std::setprecision(8) << res.value << "\n";
        else
            ss << "Result : " << std::fixed << std::setprecision(6) << res.value << "\n";
    }
    else {
        ss << "Status : " << engine.statusToText(res.status) << "\n";
    }
    ss << "--------------------------\n";
    platform_log(ss.str());
}

int main() {
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
#endif

    Calculator myCalc;

    // --- 定義 Lambda 輔助測試函數 ---
    auto quickTest = [&](std::string expr, std::string note = u8"") {
        if (!note.empty()) platform_log(u8"註解: " + note + "\n");
        Calculator::CalcResult res = myCalc.execute(expr);
        displayResult(expr, res, myCalc);
        };

    platform_log(u8"========== 引擎階段測試列表 ==========\n\n");

    // 1. 基礎四則運算與優先級 (Precedence)
    //platform_log(u8"[1. 基礎優先級測試]\n");
    std::cout << u8"\n" << u8"[1. 基礎優先級測試]\n";

    quickTest("10 + 5 * 2", u8"預期: 20 (先乘後加)");
    quickTest("100 / 4 - 5", u8"預期: 20 (先除後減)");
    quickTest("3 + 2 ^ 3", u8"預期: 11 (次方權限最高)");

    // 2. 括號處理 (Parentheses)
    //platform_log(u8"\n[2. 括號強制優先測試]\n");
    std::cout << u8"\n" << u8"[2. 括號強制優先測試]\n";

    quickTest("(10 + 5) * 2", u8"預期: 30");
    quickTest("3 + 2 ^ 3 + (5 * 6)", u8"預期: 41");
    quickTest("((1 + 2) * 3) ^ 2", u8"預期: 81 (多重括號)");

    // 3. 結合律測試 (Associativity)
    //platform_log(u8"\n[3. 結合律測試]\n");
    std::cout << u8"\n" << u8"[3. 結合律測試]\n";

    quickTest("10 - 5 - 2", u8"左結合預期: 3");
    quickTest("2 ^ 3 ^ 2", u8"右結合預期: 512 (2^9)");

    // 4. 常數辨識 (Constants)
    //platform_log(u8"\n[4. 物理/數學常數測試]\n");
    std::cout << u8"\n" << u8"[4. 物理/數學常數測試]\n";

    quickTest("PI * 2", u8"測試 PI 識別");
    quickTest("Gravity * 10", u8"測試 萬有引力常數");
    quickTest("E ^ 1", u8"測試 自然底數 E");
    quickTest("exp(1)", u8"測試 exp自然函數");

    // 5. 一元函數與科學計算 (Functions)
    //platform_log(u8"\n[5. 科學函數測試]\n");
    std::cout << u8"\n" << u8"[5. 科學函數測試]\n";

    quickTest("abs(-50)", u8"絕對值測試");
    quickTest("log(100)", u8"常用對數測試 (Base 10)");
    quickTest("sin(30)", u8"三角函數測試 (預設為 Degree)");
    quickTest("sin(PI / 6)", u8"複合測試 (若切換到弧度則結果為 0.5)");

    // 6. 一元負號處理 (Unary Minus)
    //platform_log(u8"\n[6. 一元負號測試]\n");
    std::cout << u8"\n"<< u8"[6. 一元負號測試]\n";
    quickTest("-5 + 10", u8"開頭負號測試");
    quickTest("5 * (-2)", u8"括號內負號測試");
    quickTest("--5", u8"連續負號測試 (負負得正)");

    // 7. 錯誤處理 (Error Handling)
    std::cout << u8"\n" << u8"[7. 異常輸入測試]\n";
    //platform_log(u8"\n[7. 異常輸入測試]\n");

    quickTest("10 / 0", u8"測試除以零");
    quickTest("log(-1)", u8"測試對數定義域錯誤");
    quickTest("2 s -16", u8"測試負數開偶數次方根");
    quickTest("10 + * 5", u8"測試語法錯誤");

    std::cout << u8"\n" << u8"[8. 進階運算測試]\n";
    quickTest("3 * 5 mod 6", u8"測試 mod 運算 (預期: 3)");
    quickTest("sqrt(16)", u8"測試平方根 (預期: 4)");
    quickTest("exp(2)", u8"測試自然指數函數 (預期: 7.38906)");
    quickTest("sinh(1)", u8"測試雙曲正弦 (預期: 1.175)");
    quickTest("cosh(1)", u8"測試雙曲餘弦 (預期: 1.543)");
    quickTest("tanh(1)", u8"測試雙曲正切 (預期: 0.761)");
    quickTest("tanh(1)", u8"測試雙曲正切 (預期: 0.761)");
    quickTest("fact(5)", u8"測試5階層(預期:120)");
    quickTest("fact(100)", u8"測試100階層(預期:??)");

    Calculator fibObj;
    std::cout << u8"fibonacciSeries(10): "; 
    
    auto seq = fibObj.fibonacciSeries(10); 
    for (auto v : seq){
        std::cout << v << " "; 
    }
    std::cout << "\n";

    std::cout << u8"fibonacciSeries(-10): ";
    seq = fibObj.fibonacciSeries(-10);
    for (auto v : seq) {
        std::cout << v << " ";
    }
	std::cout << "\n";

    std::cout << u8"[費氏數列]\n";
    auto p = fibObj.fibPair(10);

	std::cout << u8"fibPair(10): F(10)= "<< p.first<< ", F(9)=" << p.second <<u8" 費式函數\n";
    
    p = fibObj.fibPair(-10); 
    std::cout << u8"fibPair(-10): F(-10)=" << p.first << ", F(-9)=" << p.second << u8" 費式函數\n";
  
    platform_log(u8"\n========== 測試結束 ==========\n");

    // 這裡手動驗證一下外部類別的連通性
    std::cout << "\n[Direct Object Access Check]:\n";
    std::cout << "Constants PI: " << CalcConstants::PI << "\n";
    std::cout << "Trig Degree sin(30): " << TrigEngine().sin(30) << "\n";

    return 0;
}