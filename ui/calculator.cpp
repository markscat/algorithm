#include "calculator.h"
#include "./ui_calculator.h"


/*
   第二面板設計

   餘弦公式請自行推算,我懶得做了
  |sin 和sin⁻¹同一個按鍵          | shift 功能切換 |
  |cos 和cos⁻¹同一個按鍵          | DEG 經度弦度切換|
  |tan 和tan⁻¹同一個按鍵          | pi 常數        |
  |exp(x)/ln(x) 自然對數         | √2常數         |
  |x^y /logx(y) 指數切換         |               |
  |log10(x)/10^x 10為底指數切換   |               |
 *
 */

Calculator::Calculator(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Calculator)
{
    ui->setupUi(this);

    // 這裡就是你要的「預設隱藏」
    ui->scientific_panel->hide();

    // 這一行非常重要！
    // 隱藏後，叫視窗「縮一下」，不然左邊會留下一塊難看的空白
    //this->adjustSize();
}

Calculator::~Calculator()
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


/*
 * void Calculator::on_Menu_Button_clicked() {
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
