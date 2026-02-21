// CalcLogger.cpp
//#include "CalcLogger.h"
#include "../include/CalcLogger.h"
#include <fstream>
#include <chrono>
#include <iomanip>
#include <sstream>

std::string CalcLogger::generateTimestampFilename() {
    auto now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    struct tm timeinfo;
#ifdef _WIN32
    localtime_s(&timeinfo, &now);
#else
    localtime_r(&now, &timeinfo);
#endif

    std::stringstream ss;
    // 格式：Calc_年-月-日_時-分-秒.log
    // 注意：Windows 檔名不能包含冒號 : 所以用減號或底線代替
    ss << "Calc_"
        << std::put_time(&timeinfo, "%Y%m%d_%H%M%S")
        << ".log";

    return ss.str();
}

void CalcLogger::writeLog(const std::string& expr,
    const std::vector<Calculator::Token>& postfix,
    const Calculator::CalcResult& res)
{
    // 這裡直接呼叫產生檔名的函數
    std::string filename = generateTimestampFilename();

    //std::ofstream fout(filename, std::ios::out); // 每筆計算存成一個新檔案，所以用 ios::out

    // 將 std::ios::out 改為 std::ios::app (Append 附加模式)
    // 這樣如果同一秒內有兩筆計算，它們會存在同一個檔案裡，而不會互相覆蓋
    std::ofstream fout(filename, std::ios::app);

    if (!fout.is_open()) return;

    // --- 取得完整時間顯示在內容中 ---
    auto now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    struct tm timeinfo;
#ifdef _WIN32
    localtime_s(&timeinfo, &now);
#else
    localtime_r(&now, &timeinfo);
#endif

    // --- 格式化 RPN ---
    std::stringstream ssPostfix;
    for (const auto& t : postfix) {
        if (t.type == Calculator::TokenType::Number) ssPostfix << t.value << " ";
        else if (!t.name.empty()) ssPostfix << t.name << " ";
        else ssPostfix << t.symbol << " ";
    }

    // --- 寫入檔案 ---
    fout << "Calculation Report\n";
    fout << "------------------------------------------------\n";
    fout << "Time    : " << std::put_time(&timeinfo, "%Y-%m-%d %H:%M:%S") << "\n";
    fout << "Formula : " << expr << "\n";
    fout << "RPN     : " << ssPostfix.str() << "\n";

    if (res.status == Calculator::CalcStatus::Success) {
        fout << "Result  : " << std::setprecision(15) << res.value << "\n";
    }
    else {
        fout << "Status  : ERROR (" << Calculator::statusToText(res.status) << ")\n";
    }
    fout << "------------------------------------------------\n";

    fout.close();
}
