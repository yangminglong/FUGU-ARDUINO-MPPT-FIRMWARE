#ifndef FUGU_ARDUINO_MPPT_DEFINES
#define FUGU_ARDUINO_MPPT_DEFINES

#include <arduino.h>

#ifndef MPPT_DEFINE_GLOBAL_VARS
# define MPPT_GLOBAL extern
# define _INIT(x)
// # define _INIT_N(x)
#else
# define MPPT_GLOBAL
# define _INIT(x) = x
// //needed to ignore commas in array definitions
// #define UNPACK( ... ) __VA_ARGS__
// # define _INIT_N(x) UNPACK x
#endif

MPPT_GLOBAL String 
firmwareInfo      _INIT("V1.10   "),
firmwareDate      _INIT("30/08/21"),
firmwareContactR1 _INIT("www.youtube.com/"), 
firmwareContactR2 _INIT("TechBuilder     ");        
           
//====================== ARDUINO LIBRARIES (ESP32 Compatible Libraries) ============================//
// You will have to download and install the following libraries below in order to program the MPPT //
// unit. Visit TechBuilder's YouTube channel for the "MPPT" tutorial.                               //
//============================================================================================= ====//
// #include <EEPROM.h>                 //SYSTEM PARAMETER  - EEPROM Library (By: Arduino)
// #include <Wire.h>                   //SYSTEM PARAMETER  - WIRE Library (By: Arduino)
// #include <SPI.h>                    //SYSTEM PARAMETER  - SPI Library (By: Arduino)
// #include <WiFi.h>                   //SYSTEM PARAMETER  - WiFi Library (By: Arduino)
// #include <WiFiClient.h>             //SYSTEM PARAMETER  - WiFi Library (By: Arduino)
// #include <BlynkSimpleEsp32.h>       //SYSTEM PARAMETER  - Blynk WiFi Library For Phone App 
// #include <LiquidCrystal_I2C.h>      //SYSTEM PARAMETER  - ESP32 LCD Compatible Library (By: Robojax)
// #include <Adafruit_ADS1X15.h>       //SYSTEM PARAMETER  - ADS1115/ADS1015 ADC Library (By: Adafruit)

//====================================== USER PARAMETERS ===========================================//
// The parameters below are the default parameters used when the MPPT charger settings have not     //
// been set or saved through the LCD menu interface or mobile phone WiFi app. Some parameters here  //
// would allow you to override or unlock features for advanced users (settings not on the LCD menu) //
//==================================================================================================//
#define backflow_MOSFET  27          //SYSTEM PARAMETER - Backflow MOSFET
#define buck_IN          33          //SYSTEM PARAMETER - Buck MOSFET Driver PWM Pin
#define buck_EN          32          //SYSTEM PARAMETER - Buck MOSFET Driver Enable Pin
#define LED              2           //SYSTEM PARAMETER - LED Indicator GPIO Pin
#define FAN              16          //SYSTEM PARAMETER - Fan GPIO Pin
#define ADC_ALERT        34          //SYSTEM PARAMETER - Fan GPIO Pin
#define TempSensor       35          //SYSTEM PARAMETER - Temperature Sensor GPIO Pin
#define VoltageInputPin  00
#define VoltageOutputPin 00
#define CurrentInputPin  00
#define buttonLeft       18          //SYSTEM PARAMETER - 
#define buttonRight      17          //SYSTEM PARAMETER -
#define buttonBack       19          //SYSTEM PARAMETER - 
#define buttonSelect     23          //SYSTEM PARAMETER -

//========================================= WiFi SSID ==============================================//
// This MPPT firmware uses the Blynk phone app and arduino library for controls and data telemetry  //
// Fill in your WiFi SSID and password. You will also have to get your own authentication token     //
// from email after registering from the Blynk platform.                                            //
//==================================================================================================//
MPPT_GLOBAL char 
auth[] _INIT("InputBlynkAuthenticationToken"),  //   USER PARAMETER - Input Blynk Authentication Token (From email after registration)
ssid[] _INIT("InputWiFiSSID"),                  //   USER PARAMETER - Enter Your WiFi SSID
pass[] _INIT("InputWiFiPassword");               //   USER PARAMETER - Enter Your WiFi Password

//====================================== USER PARAMETERS ==========================================//
// The parameters below are the default parameters used when the MPPT charger settings have not    //
// been set or saved through the LCD menu interface or mobile phone WiFi app. Some parameters here //
// would allow you to override or unlock features for advanced users (settings not on the LCD menu)//
//=================================================================================================//
MPPT_GLOBAL bool                                  
MPPT_Mode               _INIT(1),          //   USER PARAMETER - Enable MPPT algorithm),when disabled charger uses CC-CV algorithm 
output_Mode             _INIT(1),          //   USER PARAMETER - 0 = PSU MODE),1 = Charger Mode  
disableFlashAutoLoad    _INIT(0),          //   USER PARAMETER - Forces the MPPT to not use flash saved settings),enabling this "1" defaults to programmed firmware settings.
enablePPWM              _INIT(1),          //   USER PARAMETER - Enables Predictive PWM),this accelerates regulation speed (only applicable for battery charging application)
enableWiFi              _INIT(1),          //   USER PARAMETER - Enable WiFi Connection
enableFan               _INIT(1),          //   USER PARAMETER - Enable Cooling Fan
enableBluetooth         _INIT(1),          //   USER PARAMETER - Enable Bluetooth Connection
enableLCD               _INIT(1),          //   USER PARAMETER - Enable LCD display
enableLCDBacklight      _INIT(1),          //   USER PARAMETER - Enable LCD display's backlight
overrideFan             _INIT(0),          //   USER PARAMETER - Fan always on
enableDynamicCooling    _INIT(0);          //   USER PARAMETER - Enable for PWM cooling control 
MPPT_GLOBAL int
serialTelemMode         _INIT(1),          //  USER PARAMETER - Selects serial telemetry data feed (0 - Disable Serial),1 - Display All Data),2 - Display Essential),3 - Number only)
pwmResolution           _INIT(11),         //  USER PARAMETER - PWM Bit Resolution 
pwmFrequency            _INIT(39000),      //  USER PARAMETER - PWM Switching Frequency - Hz (For Buck)
temperatureFan          _INIT(60),         //  USER PARAMETER - Temperature threshold for fan to turn on
temperatureMax          _INIT(90),         //  USER PARAMETER - Overtemperature),System Shudown When Exceeded (deg C)
telemCounterReset       _INIT(0),          //  USER PARAMETER - Reset Telem Data Every (0 = Never),1 = Day),2 = Week),3 = Month),4 = Year) 
errorTimeLimit          _INIT(1000),       //  USER PARAMETER - Time interval for reseting error counter (milliseconds)  
errorCountLimit         _INIT(5),          //  USER PARAMETER - Maximum number of errors  
millisRoutineInterval   _INIT(250),        //  USER PARAMETER - Time Interval Refresh Rate For Routine Functions (ms)
millisSerialInterval    _INIT(1),          //  USER PARAMETER - Time Interval Refresh Rate For USB Serial Datafeed (ms)
millisLCDInterval       _INIT(1000),       //  USER PARAMETER - Time Interval Refresh Rate For LCD Display (ms)
millisWiFiInterval      _INIT(2000),       //  USER PARAMETER - Time Interval Refresh Rate For WiFi Telemetry (ms)
millisLCDBackLInterval  _INIT(2000),       //  USER PARAMETER - Time Interval Refresh Rate For WiFi Telemetry (ms)
backlightSleepMode      _INIT(0),          //  USER PARAMETER - 0 = Never),1 = 10secs),2 = 5mins),3 = 1hr),4 = 6 hrs),5 = 12hrs),6 = 1 day),7 = 3 days),8 = 1wk),9 = 1month
baudRate                _INIT(500000);      //  USER PARAMETER - USB Serial Baud Rate (bps)

MPPT_GLOBAL float 
voltageBatteryMax       _INIT(25.5000),    //   USER PARAMETER - Maximum Battery Charging Voltage (Output V)
voltageBatteryMin       _INIT(20.0000),    //   USER PARAMETER - Minimum Battery Charging Voltage (Output V)
currentChargingMax      _INIT(50.0000),    //   USER PARAMETER - Maximum Charging Current (A - Output)
electricalPrice         _INIT(9.5000);      //   USER PARAMETER - Input electrical price per kWh (Dollar/kWh,Euro/kWh,Peso/kWh)


//================================== CALIBRATION PARAMETERS =======================================//
// The parameters below can be tweaked for designing your own MPPT charge controllers. Only modify //
// the values below if you know what you are doing. The values below have been pre-calibrated for  //
// MPPT charge controllers designed by TechBuilder (Angelo S. Casimiro)                            //
//=================================================================================================//
MPPT_GLOBAL bool
ADS1015_Mode            _INIT(1);          //  CALIB PARAMETER - Use 1 for ADS1015 ADC model use 0 for ADS1115 ADC model
MPPT_GLOBAL int                     
ADC_GainSelect          _INIT(2),         //  CALIB PARAMETER - ADC Gain Selection (0→±6.144V 3mV/bit),1→±4.096V 2mV/bit),2→±2.048V 1mV/bit)
avgCountVS              _INIT(3),         //  CALIB PARAMETER - Voltage Sensor Average Sampling Count (Recommended: 3)
avgCountCS              _INIT(4),         //  CALIB PARAMETER - Current Sensor Average Sampling Count (Recommended: 4)
avgCountTS              _INIT(500);        //  CALIB PARAMETER - Temperature Sensor Average Sampling Count
MPPT_GLOBAL float                   
inVoltageMeasMax        _INIT(120.000),   //  CALIB PARAMETER - 输入电压上限（单位V），基于2.5V参考电压计算的最大输入电压，尽量靠近实际电压，获得更高测量精度
outVoltageMeasMax       _INIT(57.5000),   //  CALIB PARAMETER - 输出电压上限（单位V），
inCurrentMeasMax        _INIT(5000.00),   //  CALIB PARAMETER - 输入电流传感器电压上限（单位mV），
currentMidPoint         _INIT(2500.0),    //  CALIB PARAMETER - 电流传感器零点 (mV)
currentSensV            _INIT(20.000),    //  CALIB PARAMETER - 电流传感器灵敏度 (mV/A)
vOutSystemMax           _INIT(50.0000),   //  CALIB PARAMETER - 
cOutSystemMax           _INIT(50.0000),   //  CALIB PARAMETER - 
ntcResistance           _INIT(10000.0),   //  CALIB PARAMETER - NTC temp sensor's resistance. Change to 10000.00 if you are using a 10k NTC
voltageDropout          _INIT(1.0000),    //  CALIB PARAMETER - 降压调节器的压降电压（由于最大占空比限制，存在DOV）
voltageBatteryThresh    _INIT(1.5000),    //  CALIB PARAMETER - 达到此电压时，电源切断 (Output V)
currentInAbsolute       _INIT(31.0000),   //  CALIB PARAMETER - Maximum Input Current The System Can Handle (A - Input)
currentOutAbsolute      _INIT(50.0000),   //  CALIB PARAMETER - Maximum Output Current The System Can Handle (A - Input)
PPWM_margin             _INIT(99.5000),   //  CALIB PARAMETER - Minimum Operating Duty Cycle for Predictive PWM (%)
PWM_MaxDC               _INIT(97.0000),   //  CALIB PARAMETER - Maximum Operating Duty Cycle (%) 90%-97% is good
efficiencyRate          _INIT(1.0000),    //  CALIB PARAMETER - Theroretical Buck Efficiency (% decimal)
currentSens             _INIT(0.0000),    //  CALIB PARAMETER - Current Sensor Sensitivity (V/A)
vInSystemMin            _INIT(10.000);     //  CALIB PARAMETER - 

//===================================== SYSTEM PARAMETERS =========================================//
// Do not change parameter values in this section. The values below are variables used by system   //
// processes. Changing the values can damage the MPPT hardware. Kindly leave it as is! However),   //
// you can access these variables to acquire data needed for your mods.                            //
//=================================================================================================//
MPPT_GLOBAL bool
buckEnable            _INIT(0),          // SYSTEM PARAMETER - Buck Enable Status
fanStatus             _INIT(0),          // SYSTEM PARAMETER - Fan activity status (1 = On),0 = Off)
bypassEnable          _INIT(0),          // SYSTEM PARAMETER - 
chargingPause         _INIT(0),          // SYSTEM PARAMETER - 
lowPowerMode          _INIT(0),          // SYSTEM PARAMETER - 
buttonRightStatus     _INIT(0),          // SYSTEM PARAMETER -
buttonLeftStatus      _INIT(0),          // SYSTEM PARAMETER - 
buttonBackStatus      _INIT(0),          // SYSTEM PARAMETER - 
buttonSelectStatus    _INIT(0),          // SYSTEM PARAMETER -
buttonRightCommand    _INIT(0),          // SYSTEM PARAMETER - 
buttonLeftCommand     _INIT(0),          // SYSTEM PARAMETER - 
buttonBackCommand     _INIT(0),          // SYSTEM PARAMETER - 
buttonSelectCommand   _INIT(0),          // SYSTEM PARAMETER -
settingMode           _INIT(0),          // SYSTEM PARAMETER -
setMenuPage           _INIT(0),          // SYSTEM PARAMETER -
boolTemp              _INIT(0),          // SYSTEM PARAMETER -
flashMemLoad          _INIT(0),          // SYSTEM PARAMETER -  
confirmationMenu      _INIT(0),          // SYSTEM PARAMETER -      
WIFI                  _INIT(0),          // SYSTEM PARAMETER - 
BNC                   _INIT(0),          // SYSTEM PARAMETER -  
REC                   _INIT(0),          // SYSTEM PARAMETER - 
FLV                   _INIT(0),          // SYSTEM PARAMETER - 
IUV                   _INIT(0),          // SYSTEM PARAMETER - 
IOV                   _INIT(0),          // SYSTEM PARAMETER - 
IOC                   _INIT(0),          // SYSTEM PARAMETER - 
OUV                   _INIT(0),          // SYSTEM PARAMETER - 
OOV                   _INIT(0),          // SYSTEM PARAMETER - 
OOC                   _INIT(0),          // SYSTEM PARAMETER - 
OTE                   _INIT(0);           // SYSTEM PARAMETER - 
MPPT_GLOBAL int
inputSource           _INIT(0),          // SYSTEM PARAMETER - 0 = MPPT has no power source),1 = MPPT is using solar as source),2 = MPPTis using battery as power source
avgStoreTS            _INIT(0),          // SYSTEM PARAMETER - Temperature Sensor uses non invasive averaging),this is used an accumulator for mean averaging
temperature           _INIT(0),          // SYSTEM PARAMETER -
sampleStoreTS         _INIT(0),          // SYSTEM PARAMETER - TS AVG nth Sample
pwmMax                _INIT(0),          // SYSTEM PARAMETER - 在设置的PWM分辨率下的PWM最大数值，如4095
pwmMaxLimited         _INIT(0),          // SYSTEM PARAMETER -
PWM                   _INIT(0),          // SYSTEM PARAMETER -
PPWM                  _INIT(0),          // SYSTEM PARAMETER -
pwmChannel            _INIT(0),          // SYSTEM PARAMETER -
batteryPercent        _INIT(0),          // SYSTEM PARAMETER -
errorCount            _INIT(0),          // SYSTEM PARAMETER -
menuPage              _INIT(0),          // SYSTEM PARAMETER -
subMenuPage           _INIT(0),          // SYSTEM PARAMETER -
ERR                   _INIT(0),          // SYSTEM PARAMETER - 
conv1                 _INIT(0),          // SYSTEM PARAMETER -
conv2                 _INIT(0),          // SYSTEM PARAMETER -
intTemp               _INIT(0);           // SYSTEM PARAMETER -
MPPT_GLOBAL float
VSI                   _INIT(0.0000),     // SYSTEM PARAMETER - Raw input voltage sensor ADC voltage
VSO                   _INIT(0.0000),     // SYSTEM PARAMETER - Raw output voltage sensor ADC voltage
CSI                   _INIT(0.0000),     // SYSTEM PARAMETER - Raw current sensor ADC voltage
CSI_converted         _INIT(0.0000),     // SYSTEM PARAMETER - Actual current sensor ADC voltage 
TS                    _INIT(0.0000),     // SYSTEM PARAMETER - Raw temperature sensor ADC value
powerInput            _INIT(0.0000),     // SYSTEM PARAMETER - Input power (solar power) in Watts
powerInputPrev        _INIT(0.0000),     // SYSTEM PARAMETER - Previously stored input power variable for MPPT algorithm (Watts)
powerOutput           _INIT(0.0000),     // SYSTEM PARAMETER - Output power (battery or charing power in Watts)
energySavings         _INIT(0.0000),     // SYSTEM PARAMETER - Energy savings in fiat currency (Peso),USD),Euros or etc...)
voltageInput          _INIT(0.0000),     // SYSTEM PARAMETER - Input voltage (solar voltage)
voltageInputPrev      _INIT(0.0000),     // SYSTEM PARAMETER - Previously stored input voltage variable for MPPT algorithm
voltageOutput         _INIT(0.0000),     // SYSTEM PARAMETER - Input voltage (battery voltage)
currentInput          _INIT(0.0000),     // SYSTEM PARAMETER - 输入电流 (单位A)
currentOutput         _INIT(0.0000),     // SYSTEM PARAMETER - Output current (battery or charing current in Amperes)
TSlog                 _INIT(0.0000),     // SYSTEM PARAMETER - Part of NTC thermistor thermal sensing code
ADC_BitReso           _INIT(0.0000),     // SYSTEM PARAMETER - System detects the approriate bit resolution factor for ADS1015/ADS1115 ADC
daysRunning           _INIT(0.0000),     // SYSTEM PARAMETER - Stores the total number of days the MPPT device has been running since last powered
Wh                    _INIT(0.0000),     // SYSTEM PARAMETER - Stores the accumulated energy harvested (Watt-Hours)
kWh                   _INIT(0.0000),     // SYSTEM PARAMETER - Stores the accumulated energy harvested (Kiliowatt-Hours)
MWh                   _INIT(0.0000),     // SYSTEM PARAMETER - Stores the accumulated energy harvested (Megawatt-Hours)
loopTime              _INIT(0.0000),     // SYSTEM PARAMETER -
outputDeviation       _INIT(0.0000),     // SYSTEM PARAMETER - Output Voltage Deviation (%)
buckEfficiency        _INIT(0.0000),     // SYSTEM PARAMETER - Measure buck converter power conversion efficiency (only applicable to my dual current sensor version)
floatTemp             _INIT(0.0000),
vOutSystemMin         _INIT(0.0000);     //  CALIB PARAMETER - 
MPPT_GLOBAL unsigned long 
currentErrorMillis    _INIT(0),          //SYSTEM PARAMETER -
currentButtonMillis   _INIT(0),          //SYSTEM PARAMETER -
currentSerialMillis   _INIT(0),          //SYSTEM PARAMETER -
currentRoutineMillis  _INIT(0),          //SYSTEM PARAMETER -
currentLCDMillis      _INIT(0),          //SYSTEM PARAMETER - 
currentLCDBackLMillis _INIT(0),          //SYSTEM PARAMETER - 
currentWiFiMillis     _INIT(0),          //SYSTEM PARAMETER - 
currentMenuSetMillis  _INIT(0),          //SYSTEM PARAMETER - 
prevButtonMillis      _INIT(0),          //SYSTEM PARAMETER -
prevSerialMillis      _INIT(0),          //SYSTEM PARAMETER -
prevRoutineMillis     _INIT(0),          //SYSTEM PARAMETER -
prevErrorMillis       _INIT(0),          //SYSTEM PARAMETER -
prevWiFiMillis        _INIT(0),          //SYSTEM PARAMETER -
prevLCDMillis         _INIT(0),          //SYSTEM PARAMETER -
prevLCDBackLMillis    _INIT(0),          //SYSTEM PARAMETER -
timeOn                _INIT(0),          //SYSTEM PARAMETER -
loopTimeStart         _INIT(0),          //SYSTEM PARAMETER - Used for the loop cycle stop watch),records the loop start time
loopTimeEnd           _INIT(0),          //SYSTEM PARAMETER - Used for the loop cycle stop watch),records the loop end time
secondsElapsed        _INIT(0);           //SYSTEM PARAMETER - 

enum POWERFROM {
  POWER_FROM_USB,
  POWER_FROM_SOLAR,
  POWER_FROM_BAT
};

#endif