// algorithm_main.cpp
#include "algorithm.h"
#include <iostream>
#include <cstdlib>
#include <string>

#ifdef _WIN32
#include <windows.h>
#include <vector>
#endif

static void print_utf8_to_console(const std::string& s) {
#ifdef _WIN32
    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    if (h == INVALID_HANDLE_VALUE) return;
    // 將 UTF-8 轉成 UTF-16，然後用 WriteConsoleW 輸出
    int wlen = MultiByteToWideChar(CP_UTF8, 0, s.data(), (int)s.size(), nullptr, 0);
    if (wlen <= 0) return;
    std::vector<wchar_t> buf(wlen);
    MultiByteToWideChar(CP_UTF8, 0, s.data(), (int)s.size(), buf.data(), wlen);
    DWORD written = 0;
    WriteConsoleW(h, buf.data(), (DWORD)wlen, &written, nullptr);
#else
    std::cout << s;
#endif
}

void printTest(double a, char op, double b, Calculator& engine) {

    Calculator::CalcResult res = engine.calculate(a, op, b);

    std::string msg;
    // 假設此檔案以 UTF-8 編碼儲存，中文字串皆為 UTF-8
    msg += "input: ";
    msg += std::to_string(a);
    msg += " ";
    msg += op;
    msg += " ";
    msg += std::to_string(b);
    msg += "\n";

    if (res.status == Calculator::CalcStatus::Success) {
        msg += "result: ";
        msg += std::to_string(res.value);
        msg += "\n";
    } else {
        msg += "status: ";
        msg += Calculator::statusToText(res.status); // 回傳的字串也應為 UTF-8
        msg += "\n";
    }
    msg += "--------------------------\n";

    print_utf8_to_console(msg);
}

int main() {
    // 建議：設定控制台為 UTF-8（可選，但 WriteConsoleW 已足夠）
    #ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
    std::cout << u8"中文"<<"\n";
    std::cout << (const char*)u8"中文" << "\n";
    #endif

    Calculator myCalc;
    std::cout <<"10+5" << u8"：加法" << "\n";
    printTest(10, '+', 5, myCalc);

    std::cout << "10/4" << u8"：除法" << "\n";
    printTest(10, '/', 4, myCalc);

    std::cout << "10/0" << u8"：除0" << "\n";
    printTest(10, '/', 0, myCalc);

    std::cout << "10?5" << u8"；無效符號" << "\n";
    printTest(10, '?', 5, myCalc);

    std::cout << "10^2" << u8"；次方" << "\n";
    printTest(10, '^', 2, myCalc);

    std::cout << "100 Sqrt 2" << u8"；開根號"<<"\n";
    printTest(100, 's', 2, myCalc);
    return 0;
}