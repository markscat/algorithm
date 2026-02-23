#include "calculator.h"
#include "./ui_calculator.h"


/*

### 第一階段：基礎設施與視窗控制
**目標：** 讓視窗啟動時長得正確，且 `Menu` 鈕能正常開關面板。

1.  **實例化大腦**：在 `calculator.h` 私有區塊加入 `Calculator engine;`。
2.  **建構子初始化**：
    *   呼叫 `ui->scientific_panel->hide();`。
    *   呼叫 `this->adjustSize();`。
    *   設定 `ui->lineEdit->setAlignment(Qt::AlignRight);`。
3.  **實作 Menu 槽 (Slot)**：
    *   寫好 `on_Menu_Button_clicked()`，內部執行 `hide/show` + `adjustSize()`。
    *   **測試點**：程式開啟是否只有右邊？按 Menu 是否會彈出左邊？

   第二面板設計

   餘弦公式請自行推算,我懶得做了
  |sin 和sin⁻¹同一個按鍵          | shift 功能切換 |
  |cos 和cos⁻¹同一個按鍵          | DEG 經度弦度切換|
  |tan 和tan⁻¹同一個按鍵          | pi 常數        |
  |exp(x)/ln(x) 自然對數         | √2常數         |
  |x^y /logx(y) 指數切換         |  exp 常數      |
  |log10(x)/10^x 10為底指數切換   |  fib函數       |
    *

---

### 第二階段：數字輸入與螢幕顯示
**目標：** 讓 0-9、小數點和退格鍵可以運作。

1.  **統一數字處理**：
    *   不要寫 10 個 function。建立一個 `void onDigitClicked()`。
    *   在建構子用迴圈把 `Button_0` 到 `Button_9` 和 `Dot_Button` 全部 `connect` 到這個函式。
2.  **實作邏輯**：
    *   使用 `qobject_cast<QPushButton*>(sender())->text()` 抓取數字。
    *   更新 `lineEdit` 的內容。
3.  **實作退格與清空**：
    *   `Back_Button` (⌫)、`Clear_Button`、`Clear_Entry_Buttom` (CE)。
    *   **測試點**：能不能在螢幕上打出 `123.456` 並用退格鍵刪除？

---

### 第三階段：狀態管理 (RadioButtons & Shift)
**目標：** 處理單位的切換以及 Shift 的變身效果。

1.  **單位切換**：
    *   連接 `DEG`, `RAD`, `GRA` 的 `toggled` 訊號。
    *   呼叫 `engine.trig.setUnit(...)`。
2.  **REC 錄製開關**：
    *   連接 `REC_Radiobuttom`。當選中時呼叫 `engine.startLogging()`，取消時呼叫 `engine.stopLogging()`。
3.  **Shift 邏輯**：
    *   實作 `on_Shift_Button_clicked()`。
    *   改變 `isShifted` 布林值，並遍歷科學面板的按鈕，更改它們的 `setText`（例如 Sin 變 Sin⁻¹）。
    *   **測試點**：按 Shift 時，左邊的按鈕字有沒有變？切換 DEG/RAD 有沒有閃退？
    *
    *

---

### 第四階段：算式構建 (運算符)
**目標：** 把 `+ - X ÷` 以及科學函數轉成字串。

1.  **運算符轉換**：
    *   建立一個函式處理 `+ - X ÷ %`。
    *   **注意**：在丟給 `engine` 之前，要把 `X` 換成 `*`，`÷` 換成 `/`。
2.  **科學函數**：
    *   按下 `Sin(x)` 時，在 `lineEdit` 加上 `"sin("`。
    *   按下 `PI_Button` 時，加上 `"PI"`。
    *   **測試點**：螢幕上是否能出現 `sin(PI/2) + 10` 這樣的字串？

---

### 第五階段：核心執行與錯誤處理
**目標：** 按下 `=`，得到結果。

1.  **等於鍵邏輯**：
    *   實作 `on_equal_Button_clicked()`。
    *   將 `lineEdit->text()` 轉成 `std::string`。
    *   呼叫 `auto res = engine.execute(str);`。
2.  **顯示結果**：
    *   如果 `res.status == Success`，顯示 `res.value`。
    *   否則，使用 `engine.statusToText()` 把錯誤訊息秀在 `label` 或彈出警告。
3.  **歷史紀錄**：
    *   計算成功後，把剛才的算式丟給上方的 `label` 顯示。
    *   **測試點**：輸入 `2+3*4` 按等於，是否出現 `14`？

---



 *
 */

CalculatorUI::CalculatorUI(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Calculator)
{
    ui->setupUi(this);


    // --- 1. 視窗尺寸與佈局約束 (解決縮成小點的關鍵) ---
    // 強迫視窗的大小永遠「吸附」在佈局內的元件上
    // 這樣當面板隱藏時，視窗會自動縮小；面板顯示時，視窗會自動撐開
    if (this->centralWidget()->layout()) {
        this->centralWidget()->layout()->setSizeConstraint(QLayout::SetFixedSize);
    }



    // --- 2. 螢幕 (QLineEdit) 初始化 ---
    ui->lineEdit->setAlignment(Qt::AlignRight); // 文字靠右
    ui->lineEdit->setReadOnly(true);            // 唯讀，禁止鍵盤輸入
    ui->lineEdit->setText("0");                 // 預設顯示 0
    ui->label->setText("");                     // 上方紀錄標籤先清空

    // --- 3. 擴充面板初始狀態 ---
    ui->scientific_panel->hide();               // 啟動時隱藏左側面板

    // --- 4. 單元/模式初始狀態 ---
    ui->BIN->setChecked(true);                  // 預設選中 DEG (對應你 UI 上的第一個 Radio)
    // 這裡記得同步你的引擎單位
    // engine.trig.setUnit(TrigEngine::AngleUnit::Degree);

    // --- 5. 其他 UI 微調 ---
    // 如果你有兩個空按鍵，可以先禁用它們
    ui->pushButton_11->setEnabled(false);
    ui->pushButton_12->setEnabled(false);
}

CalculatorUI::~CalculatorUI()
{
    delete ui;
}


/*
 * void Calculator::on_btn_Unit_clicked() {
    // 1. 取得目前引擎的單位
    auto current = trig.getUnit();

    // 2. 依照順序切換並更新 UI
    if (current == AngleUnit::Degree) {
        trig.setUnit(AngleUnit::Radian);
        ui->btn_Unit->setText("RAD"); // 按鍵文字直接變，使用者立刻知道換了
    } else if (current == AngleUnit::Radian) {
        // 如果你不想處理 GRAD，這一段可以跳過，直接切回 Degree
        trig.setUnit(AngleUnit::Gradian);
        ui->btn_Unit->setText("GRA");
    } else {
        trig.setUnit(AngleUnit::Degree);
        ui->btn_Unit->setText("DEG");
    }
}
*/

/*
void Calculator::updateScientificButtons(bool shifted) {
    if (shifted) {
        ui->btn_sin->setText("sin⁻¹");
        ui->btn_pow->setText("ʸ√x");
    } else {
        ui->btn_sin->setText("sin");
        ui->btn_pow->setText("xʸ");
    }
}

*/


void CalculatorUI::on_Menu_Button_clicked()
{
    ui->scientific_panel->setVisible(!ui->scientific_panel->isVisible());
}
/*
void CalculatorUI::on_Menu_Button_clicked() {

    if (ui->scientific_panel->isHidden()) {
        ui->scientific_panel->show();
    } else {
        ui->scientific_panel->hide();
    }

    // 先確保 Layout 重新計算
    ui->centralwidget->layout()->activate();

    // 再執行調整，這時因為右邊有 minimumSize，視窗就不會縮成一團
    this->adjustSize();
}
*/


/*彈出動畫1
 *
  #include <QPropertyAnimation>

QPropertyAnimation *anim = new QPropertyAnimation(ui->scientific_panel, "geometry");
anim->setDuration(250);   // 動畫時間 0.25 秒

void Calculator::on_pushButton_4_clicked()
{
    QRect startRect;
    QRect endRect;

    int panelWidth = ui->scientific_panel->width();

    if (!ui->scientific_panel->isVisible())
    {
        ui->scientific_panel->show();

        startRect = QRect(-panelWidth, 180, panelWidth, 411);
        endRect   = QRect(30, 180, panelWidth, 411);
    }
    else
    {
        startRect = QRect(30, 180, panelWidth, 411);
        endRect   = QRect(-panelWidth, 180, panelWidth, 411);
    }

    QPropertyAnimation *anim = new QPropertyAnimation(ui->scientific_panel, "geometry");
    anim->setDuration(250);
    anim->setStartValue(startRect);
    anim->setEndValue(endRect);
    anim->start();

    connect(anim, &QPropertyAnimation::finished, this, [=]() {
        if (endRect.x() < 0)
            ui->scientific_panel->hide();
    });
}
*/


/*
 *
 * 彈出動畫2
// 這需要 #include <QPropertyAnimation>
void Calculator::on_Menu_Button_clicked() {
    int targetWidth = ui->scientific_panel->width() > 0 ? 0 : 150;

    // 建立一個動畫，改變左側面板的最小寬度
    QPropertyAnimation *animation = new QPropertyAnimation(ui->scientific_panel, "minimumWidth");
    animation->setDuration(200); // 0.2 秒
    animation->setStartValue(ui->scientific_panel->width());
    animation->setEndValue(targetWidth);
    animation->start(QAbstractAnimation::DeleteWhenStopped);

    // 注意：這種做法要把左邊面板的超出部分切掉 (setClipsCharacters)
}


*/
