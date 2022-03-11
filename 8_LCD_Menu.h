#ifndef _POWERON_LCD_MENU_H
#define _POWERON_LCD_MENU_H

#include <Arduino.h>
#include <U8g2lib.h>
#include <functional>

void (*current_operation_index)();

typedef struct
{
	uint8_t  current;
	uint8_t  up;    //向上翻索引号
	uint8_t  down;  //向下翻索引号
	uint8_t  enter; //确认索引号
	void (*current_operation)();
} key_table;

uint8_t func_index=0;
void showState();
void fun2();
void fun3();
void fun4();
void page_1_to_4();


key_table code_table[30]=
{
	{0, 1, 1, 1, (*goSleep)},   //第一层，休眠，按任意键进入下一页

	{1, 2, 2, 5, (*showState)},   //第二层，状态显示，上下切换详细状态与突出状态，按下进入配置菜单
	{2, 1, 1, 5, (*showState)},   //第二层，状态显示，上下切换详细状态与突出状态，按下进入配置菜单

  {3, 1,  1, 1, nullptr },   // 
  {4, 1,  1, 1, nullptr },   // 

  { 5, 12,  6, 21, (*showMenuItem0)},   // 配置菜单项：涓流电压 
  { 6,  5,  7, 27, (*showMenuItem1)},   // 配置菜单项：过冲电压	
  { 7,  6,  8, 33, (*showMenuItem2)},   // 配置菜单项：最大电流	
  { 8,  7,  9, 49, (*showMenuItem3)},   // 配置菜单项：网络开关
  { 9,  8, 10, 40, (*showMenuItem4)},   // 配置菜单项：充电算法
  {10,  9, 20, 41, (*showMenuItem5)},   // 配置菜单项：背光开关

  {11, 1,  1, 1, nullptr},   //  
  {12, 1,  1, 1, nullptr},   //  
  {13, 1,  1, 1, nullptr },   // 
  {14, 1,  1, 1, nullptr },   // 
  {15, 1,  1, 1, nullptr },   // 
  {16, 1,  1, 1, nullptr },   // 
  {17, 1,  1, 1, nullptr },   // 
  {18, 1,  1, 1, nullptr },   // 
  {19, 1,  1, 1, nullptr },   // 
  {20, 10, 5, 51, (*showMenuItemFactory)},   // 配置菜单项：出厂设置

  {21, 23, 24, 22, (*settingItem0_step_r1)     },   // 配置调整：涓流电压 步进0.1	
  {22, 25, 26,  5, (*settingItem0_step_10)     },   // 配置调整：涓流电压 步进1
  {23, 23, 24, 22, (*settingItem0_step_r1_next)},   // 配置调整：涓流电压 上一步 步进0.1
  {24, 23, 24, 22, (*settingItem0_step_r1_prev)},   // 配置调整：涓流电压 下一步 步进0.1
  {25, 25, 26,  5, (*settingItem0_step_10_next)},   // 配置调整：涓流电压 上一步 步进1
  {26, 25, 26,  5, (*settingItem0_step_10_prev)},   // 配置调整：涓流电压 下一步 步进1
  
  {27, 29, 30, 28, (*settingItem1_step_r1)     },   // 配置调整：过冲电压 步进0.1	
  {28, 31, 32,  6, (*settingItem1_step_10)     },   // 配置调整：过冲电压 步进1
  {29, 29, 30, 28, (*settingItem1_step_r1_next)},   // 配置调整：过冲电压 上一步 步进0.1
  {30, 29, 30, 28, (*settingItem1_step_r1_prev)},   // 配置调整：过冲电压 下一步 步进0.1
  {31, 31, 32,  6, (*settingItem1_step_10_next)},   // 配置调整：过冲电压 上一步 步进1
  {32, 31, 32,  6, (*settingItem1_step_10_prev)},   // 配置调整：过冲电压 下一步 步进1
  
  {33, 35, 36, 34, (*settingItem2_step_r1)     )},   // 配置调整：最大电流 步进0.1	
  {34, 37, 38,  7, (*settingItem2_step_10)     )},   // 配置调整：最大电流 步进1
  {35, 35, 36, 34, (*settingItem2_step_r1_next))},   // 配置调整：最大电流 上一步 步进0.1
  {36, 35, 36, 34, (*settingItem2_step_r1_prev))},   // 配置调整：最大电流 下一步 步进0.1
  {37, 37, 38,  7, (*settingItem2_step_10_next))},   // 配置调整：最大电流 上一步 步进1
  {38, 37, 38,  7, (*settingItem2_step_10_prev))},   // 配置调整：最大电流 下一步 步进1

  {39, 39, 39, 8, (*settingItem3_switch)},   // 配置调整：切换网络开关

  {40, 40, 40, 9, (*settingItem4_switch)},   // 配置调整：切换充电算法

  {41, 41, 41, 10, (*settingItem5_switch)},   // 配置调整：切换背光开关

  {42, 1, 1, 1, nullptr},   // 

  {51, 52, 52, 20, (*showMenuItemFactory_ignore)},   // 配置调整：出厂设置 取消
  {52, 51, 51, 20, (*showMenuItemFactory_accept)},   // 配置调整：出厂设置 确认 
                 
};

U8G2_SSD1327_EA_W128128_F_4W_HW_SPI u8g2(U8G2_R0, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);



void setup(void)
{
  u8g2.begin();
  u8g2.enableUTF8Print();
  // ESP.wdtEnable(5000);//看门狗初始化
}

#include <iostream>
#include <stdio.h>
int screenWidth  = 128;
int screenHeight = 64;
int lineHeight = 12;
int screenMarginY = 1;

int avLineCounts = 0;
int lineGap = 0;

int calcScreenLine(int screenMargin = 1, int minLineGap = 0) {
  int lines = (screenHeight-screenMargin*2) / lineHeight;
  int lineGapT = 0;
  if (lines>1) {
    lineGapT = ((screenHeight-screenMargin*2) - lines*lineHeight) / (lines-1);
  }
  if (lineGapT < minLineGap)
    lines--;
  if (lines) {
    avLineCounts = lines;
    lineGap = ((screenHeight-screenMargin*2) - lines*lineHeight) / (lines-1);
  }
}

// 获取配置页面指定行的文字Y轴坐标
int getLinePos(int line) {
  return screenMarginY+12*line;
}


void goSleep()
{
  backlightSleepMode = true;
}

String ftoString(float data, int n, int j = 1) {
  char dataStr[16];
  dtostrf(data, n, j, dataStr);
  return dataStr;
}

void initU8g2() {
  // 设置字体：中文，12像素
  u8g2.setFont(u8g2_font_wqy12_t_gb2312);
  // 设置字体坐标系0点在左上角
  u8g2.setFontPosTop();
  // 文字高度
  lineHeight = u8g2.getHeight();
}

void showState()
{
  backlightSleepMode = false;

  String str;

  // 第一行 版本、  温度、 Solar状态、BAT状态、网络状态
  u8g2.setCursor(1, getStateLineTextPos(0));
  str =  " " + temperature + "℃" ;
  u8g2.print(str.c_str());

  // powerInput      = voltageInput*currentInput;
  // powerOutput     = voltageInput*currentInput*efficiencyRate;
  // 第二行 输入 电压 电流 
  str = "输入:" + ftoString(voltageInput, 5) + "V " + ftoString(currentInput, 4)  + "A" ;
  u8g2.setCursor(1, getStateLineTextPos(1));
  u8g2.print(str.c_str());

  // 第三行 输出 电压 电流 
  str = "输出:" + ftoString(voltageOutput, 5) + "V " + ftoString(currentOutput, 4)  + "A" ;
  u8g2.setCursor(1, getStateLineTextPos(2));
  u8g2.print(str.c_str());

  // 第四行 功率
  str = "功率:" + ftoString(powerInput, 6) + "W " + ftoString(voltageOutput, 4) + "%";
  u8g2.setCursor(1, getStateLineTextPos(3));
  u8g2.print(str.c_str());

  // 第五行 运行时间 积累kwh
  u8g2.setCursor(1, getStateLineTextPos(4)); 
  str = "收集:" + energySavings + "kwh "  + electricalPrice + "$" + daysRunning + "D";
  u8g2.print(str.c_str());  

}

void showSetting_page1() {
  u8g2.setFontPosTop();
  // u8g2.setFontDirection(0);

  u8g2.setCursor(2, getLinePos(0));
  u8g2.print(" 涓流电压:" + ftoString(voltageBatteryMin, 4));
  u8g2.setCursor(2, getLinePos(1));
  u8g2.print(" 过冲电压:" + ftoString(voltageBatteryMin, 4));
  u8g2.setCursor(2, getLinePos(2));
  u8g2.print(" 最大电流:" + ftoString(currentCharging, 4));
  u8g2.setCursor(2, getLinePos(3));
  u8g2.print(" 网络开关:" + enableWiFi ? "开" : "关");
  u8g2.setCursor(2, getLinePos(4));
  u8g2.print(" 算法切换:"+ MPPT_Mode ? "MPPT跟踪" : "PWM降压");

  u8g2.setCursor(128-12-2, getLinePos(4));
  u8g2.print("↓↓");
}

void showSetting_page2() {
  u8g2.setFontPosTop();

  u8g2.setCursor(2, getLinePos(0));
  u8g2.print(" 背光开关:" + "" + enableLCDBacklight ? "开" : "关");
  u8g2.setCursor(2, getLinePos(1));
  u8g2.print(" 恢复出厂设置");

  u8g2.setCursor(128-12-2, getLinePos(0));
  u8g2.print("↑↑");
}

void showSetting_page1_Item(int nItem, bool isEdit = false) {
  backlightSleepMode = false;

  u8g2.clearBuffer();
  //显示选中框, 灰色背景
  u8g2.setDrawColor(64);
  u8g2.drawBox(0, getLinePos(nItem), screenWidth, lineHeight);
  if (isEdit) {
    // 空白背景
    u8g2.setDrawColor(0); 
    u8g2.drawBox(60, getLinePos(nItem), screenWidth-60, lineHeight);
  } 
  
  showSetting_page1();

  u8g2.sendBuffer();
}

void showSetting_page2_Item(int nItem) {
  u8g2.clearBuffer();
  //显示选中框, 灰色背景
  u8g2.setDrawColor(64);
  u8g2.drawBox(0, getLinePos(nItem), screenWidth, lineHeight);

  showSetting_page2();

  u8g2.sendBuffer();
}

void showMenuItem0() {
  showSetting_page1_Item(0);
}
void showMenuItem1() {
  showSetting_page1_Item(1);
}
void showMenuItem2() {
  showSetting_page1_Item(2);
}
void showMenuItem3() {
  showSetting_page1_Item(3);
}
void showMenuItem4() {
  showSetting_page1_Item(4);
}

void showMenuItem5() {
  showSetting_page2_Item(0);
}
void showMenuItemFactory() {
  showSetting_page2_Item(1);
}
// void showMenuItem7() {
//   showSetting_page2_Item(2);
// }


bool nextTrigger;
bool prevTrigger;
bool btnTrigger;

float curStep_MinBatV = 0.1;
float curStep_MaxBatV = 0.1;
float curStep_MaxBatI = 0.1;

void showSettingItem0()
{
  // 显示背景
  showSetting_page1_Item(0, true);
}


void settingItem0_step_r1()
{
  showSettingItem0(); 
  curStep_MinBatV = 0.1;
}

void settingItem0_step_10()
{
  showSettingItem0(); 
  curStep_MinBatV = 1;
}


void settingItemStepNext(std::function<void()> doNext)
{
  if (nextTrigger) {
    nextTrigger = false;
    doNext();
  }
}

void settingItemStepPrev(std::function<void()> doPrev)
{
  if (prevTrigger) {
    prevTrigger = false;
    doPrev();
  }
}

void settingItemClick(std::function<void()> doClick)
{
  if (btnTrigger) {
    btnTrigger = false;
    doClick();
  }
}

void voltageBatteryMinNext() {
    if (voltageBatteryMin < voltageBatteryMax-1)
      voltageBatteryMin += curStep_MinBatV;
}

void voltageBatteryMinPrev() {
    if (voltageBatteryMin > 0)
      voltageBatteryMin -= curStep_MinBatV;
}

void settingItem0_step_r1_next()
{
  settingItemStepNext(&voltageBatteryMinNext);
  showSettingItem0(); 
}

void settingItem0_step_r1_prev()
{
  settingItemStepPrev(&voltageBatteryMinPrev);
  showSettingItem0(); 
}

void settingItem0_step_10_next()
{
  settingItemStepNext(&voltageBatteryMinNext);
  showSettingItem0(); 
}

void settingItem0_step_10_prev()
{
  settingItemStepPrev(&voltageBatteryMinPrev);
  showSettingItem0(); 
}


void showSettingItem1()
{
  // 显示
  showSetting_page1_Item(1, true);
}


void settingItem1_step_r1()
{
  showSettingItem1(); 
  curStep_MaxBatV = 0.1;
}

void settingItem1_step_10()
{
  showSettingItem1(); 
  curStep_MaxBatV = 1;
}

void voltageBatteryMaxNext() {
    if (voltageBatteryMax < voltageBatteryMax-1)
      voltageBatteryMax += curStep_MinBatV;
}

void voltageBatteryMaxPrev() {
    if (voltageBatteryMax > 0)
      voltageBatteryMax -= curStep_MinBatV;
}

void settingItem1_step_r1_next()
{
  settingItemStepNext(&voltageBatteryMaxNext);
  showSettingItem1(); 
}

void settingItem1_step_r1_prev()
{
  settingItemStepPrev(&voltageBatteryMaxPrev);
  showSettingItem1(); 
}

void settingItem1_step_10_next()
{
  settingItemStepNext(&voltageBatteryMaxNext);
  showSettingItem1(); 
}

void settingItem1_step_10_prev()
{
  settingItemStepPrev(&voltageBatteryMaxPrev);
  showSettingItem1(); 
}


void showSettingItem2()
{
  // 显示
  showSetting_page1_Item(2, true);
}


void settingItem2_step_r1()
{
  showSettingItem2(); 
  curStep_MaxBatI = 0.1;
}

void settingItem2_step_10()
{
  showSettingItem2(); 
  curStep_MaxBatI = 1;
}

void currentChargingMaxNext() {
    currentChargingMax += curStep_MaxBatI;
    if (currentChargingMax > currentChargingMaxAbs)
      currentChargingMax = currentChargingMaxAbs;

}

void currentChargingMaxPrev() {
    currentChargingMax -= curStep_MaxBatI;
    if (currentChargingMax < 0)
      curStep_MaxBatI = 0
}

void settingItem2_step_r1_next()
{
  settingItemStepNext(&currentChargingMaxNext);
  showSettingItem2(); 
}

void settingItem2_step_r1_prev()
{
  settingItemStepPrev(&currentChargingMaxPrev);
  showSettingItem2(); 
}

void settingItem2_step_10_next()
{
  settingItemStepNext(&currentChargingMaxNext);
  showSettingItem2(); 
}

void settingItem2_step_10_prev()
{
  settingItemStepPrev(&currentChargingMaxPrev);
  showSettingItem2(); 
}

void settingItem3_switch() 
{
  settingItemStepPrev([](){
    enableWiFi = !enableWiFi;
  });

  // 显示
  showSetting_page1_Item(3, true);

}


void settingItem4_switch() 
{
  settingItemStepPrev([](){
    MPPT_Mode = !MPPT_Mode;
  });

  // 显示
  showSetting_page1_Item(4, true);

}

void settingItem5_switch() 
{
  settingItemStepPrev([](){
    enableLCDBacklight = !enableLCDBacklight;
  });

  // 显示
  showSetting_page2_Item(0, true);
}


void showMenuItemFactory_ignore() 
{
  // 显示
  showMenuItemFactory();
}

void doFactory() {

}

void showMenuItemFactory_accept() 
{
  settingItemClick(&doFactory);

  // 显示
  showSetting_page2_Item(1, true);
}

#endif