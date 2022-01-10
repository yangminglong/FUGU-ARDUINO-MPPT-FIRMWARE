#include "2_Read_Sensors.h"

#include <arduino.h>
#include "defines.h"


adc_attenuation_t attenuation = ADC_11db;
void ADC_SetGain() {

  analogSetAttenuation(attenuation);
}



uint16_t analogAvgRead(uint8_t pin) {

  return 0;
}

float analogRawToMilliVolts(uint16_t adc_reading)
{
    // uint16_t max_reading = 8191;
    uint16_t max_mv = 750;
    switch(attenuation){
        case 3: max_mv = 2500; break;
        case 2: max_mv = 1300; break;
        case 1: max_mv = 1050; break;
        default: break;
    }
    return (adc_reading * max_mv) / 8191.0;
}

float readMilliVolts(uint8_t pin) 
{
  return analogRawToMilliVolts(analogAvgRead(pin));
}


void readTempSensor()
{
  /////////// TEMPERATURE SENSOR /////////////
  if(sampleStoreTS<=avgCountTS){                               //TEMPERATURE SENSOR - Lite Averaging
    TS = TS + analogRead(TempSensor);
    sampleStoreTS++;   
  }
  else{
    TS = TS/sampleStoreTS;
    TSlog = log(ntcResistance*(4095.00/TS-1.00));
    temperature = (1.0/(1.009249522e-03+2.378405444e-04*TSlog+2.019202697e-07*TSlog*TSlog*TSlog))-273.15;
    sampleStoreTS = 0;
    TS = 0;
  }
}

float mapf(float x, float in_min, float in_max, float out_min, float out_max) {
    const float dividend = out_max - out_min;
    const float divisor = in_max - in_min;
    const float delta = x - in_min;

    return (delta * dividend + (divisor / 2)) / divisor + out_min;
}

void readVoltageInput() {
  VSI = readMilliVolts(VoltageInputPin);
  voltageInput = mapf(VSI, 0, 2500, 0, inVoltageMeasMax);
}

void readVoltageOutput() {
  VSO = readMilliVolts(VoltageOutputPin);
  voltageOutput = mapf(VSO, 0, 2500, 0, outVoltageMeasMax);
}

void readCurrentInput() {
  float sensorCurrIn = readMilliVolts(CurrentInputPin);
  CSI = mapf(sensorCurrIn, 0, 2500, 0, inCurrentMeasMax);       
   //currentMidPoint)*-1)/currentSensV
  currentInput = (CSI - currentMidPoint) / currentSensV;
  currentOutput = (voltageInput*currentInput)/voltageOutput;
}

void calcBatteryPercent() {
  batteryPercent  = ((voltageOutput-voltageBatteryMin)/(voltageBatteryMax-voltageBatteryMin))*101;
  batteryPercent  = constrain(batteryPercent,0,100);
}

void Read_Sensors(){
  //
  readTempSensor();

  // /////////// VOLTAGE & CURRENT SENSORS /////////////
  readVoltageInput();
  readVoltageOutput();
  readCurrentInput();


  // VSI = 0.0000;      //Clear Previous Input Voltage 
  // VSO = 0.0000;      //Clear Previous Output Voltage  
  // CSI = 0.0000;      //Clear Previous Current  

  // //VOLTAGE SENSOR - Instantenous Averaging   
  // for(int i = 0; i<avgCountVS; i++){
  //   VSI = VSI + ads.computeVolts(ads.readADC_SingleEnded(3));
  //   VSO = VSO + ads.computeVolts(ads.readADC_SingleEnded(1));
  // }
  // voltageInput  = (VSI/avgCountVS)*inVoltageDivRatio; 
  // voltageOutput = (VSO/avgCountVS)*outVoltageDivRatio; 

  
  // //CURRENT SENSOR - Instantenous Averaging   
  // for(int i = 0; i<avgCountCS; i++){
  //   CSI = CSI + ads.computeVolts(ads.readADC_SingleEnded(2));
  // }
  // CSI_converted = (CSI/avgCountCS)*1.3300;
  // currentInput  = ((CSI_converted-currentMidPoint)*-1)/currentSensV;  
  // if(currentInput<0){currentInput=0.0000;}
  // if(voltageOutput<=0){currentOutput = 0.0000;}
  // else{ currentOutput = (voltageInput*currentInput)/voltageOutput;}

  // 电源检测
  if(voltageInput<=3 && voltageOutput<=3){inputSource=POWER_FROM_USB;}  // 输入输出都断开，USB供电
  else if(voltageInput>voltageOutput)    {inputSource=POWER_FROM_SOLAR;}  // 输入供电
  else if(voltageInput<voltageOutput)    {inputSource=POWER_FROM_BAT;}  // 输出供电
  
  //////// 自动电流传感器零点校准 ////////
  if(buckEnable==0 && FLV==0 && OOV == 0){                
    currentMidPoint = CSI;
  }
  
  // 功率计算 - Through computation
  powerInput      = voltageInput*currentInput;                   // 输入功率
  powerOutput     = voltageInput*currentInput*efficiencyRate;    // 理想输出功率
  outputDeviation = (voltageOutput/voltageBatteryMax)*100.000;   // 目标电压偏差

  //STATE OF CHARGE - Battery Percentage
  calcBatteryPercent();

  //TIME DEPENDENT SENSOR DATA COMPUTATION
  currentRoutineMillis = millis();
  if(currentRoutineMillis-prevRoutineMillis>=millisRoutineInterval){   // 每毫秒运行一次例程例程间隔（毫秒）
    prevRoutineMillis = currentRoutineMillis;                          // Store previous time
    Wh = Wh+(powerInput/(3600.000*(1000.000/millisRoutineInterval)));  // 积累并计算收集的能量 (3600s*(1000/interval))
    kWh = Wh/1000.000;
    MWh = Wh/1000000.000;
    daysRunning = timeOn/(86400.000*(1000.000/millisRoutineInterval)); // 运行时间（天） (86400s*(1000/interval))
    timeOn++;                                                          // Increment time counter
  } 

  //OTHER DATA
  secondsElapsed = millis()/1000;                                      // 获取自打开并处于活动状态以来的时间（秒）
  energySavings  = electricalPrice*(Wh/1000.0000);                     //Computes the solar energy saving in terms of money (electricity flag rate)   
}
