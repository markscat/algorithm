#ifndef CALCULATOR_H
#define CALCULATOR_H

#include <QMainWindow>
#include "../include/algorithm.h" // 引用你的核心引擎
//calculator.h:: Included header algorithm.h is not used directly (fix available)


QT_BEGIN_NAMESPACE
namespace Ui {
class Calculator;
}
QT_END_NAMESPACE

class CalculatorUI : public QMainWindow
//calculator.h:15:7: Redefinition of 'Calculator'
//                       algorithm.h:15:7: previous definition is here
{
    Q_OBJECT

public:
    CalculatorUI(QWidget *parent = nullptr);
    ~CalculatorUI();


private slots:
    // Menu 按鈕的點擊事件 (Qt 會自動根據名稱連接)
    void on_Menu_Button_clicked();

private:
    Ui::Calculator *ui;

    // 實例化你的計算機大腦
    Calculator engine;
};
#endif // CALCULATOR_H
