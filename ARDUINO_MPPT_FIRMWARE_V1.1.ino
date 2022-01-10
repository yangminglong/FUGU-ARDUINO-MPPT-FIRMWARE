/*  PROJECT FUGU FIRMWARE V1.10  (DIY 1kW Open Source MPPT Solar Charge Controller)
 *  By: TechBuilder (Angelo Casimiro)
 *  FIRMWARE STATUS: Verified Stable Build Version
 *  (Contact me for the experimental beta versions)
 *  -----------------------------------------------------------------------------------------------------------
 *  DATE CREATED:  02/07/2021 
 *  DATE MODIFIED: 30/08/2021
 *  -----------------------------------------------------------------------------------------------------------
 *  CONTACTS:
 *  GitHub - www.github.com/AngeloCasi (New firmware releases will only be available on GitHub Link)
 *  Email - casithebuilder@gmail.com
 *  YouTube - www.youtube.com/TechBuilder
 *  Facebook - www.facebook.com/AngeloCasii
 *  -----------------------------------------------------------------------------------------------------------
 *  PROGRAM FEATURES:
 *  - MPPT Perturbed Algorithm With CC-CV
 *  - WiFi & Bluetooth BLE Blynk Phone App Telemetry
 *  - Selectable Charger/PSU Mode (can operate as a programmable buck converter)
 *  - Dual Core ESP32 Unlocked (using xTaskCreatePinnedToCore(); )
 *  - Precision ADC Tracking Auto ADS1115/ADS1015 Detection (16bit/12bit I2C ADC)
 *  - Automatic ACS712-30A Current Sensor Calibration
 *  - Equipped With Battery Disconnect & Input Disconnect Recovery Protection Protocol
 *  - LCD Menu Interface (with settings & 4 display layouts)
 *  - Flash Memory (non-volatile settings save function)
 *  - Settable PWM Resolution (8bit-16bit)
 *  - Settable PWM Switching Frequency (1.2kHz - 312kHz)
 *  -----------------------------------------------------------------------------------------------------------
 *  PROGRAM INSTRUCTIONS:
 *  1.) Watch YouTube video tutorial first before using
 *  2.) Install the required Arduino libraries for the ICs
 *  3.) Select Tools > ESP32 Dev Board Board 
 *  4.) Do not modify code unless you know what you are doing
 *  5.) The MPPT's synchronous buck converter topology is code dependent, messing with the algorithm
 *      and safety protection protocols can be extremely dangerous especially when dealing with HVDC.
 *  6.) Install Blynk Legacy to access the phone app telemetry feature
 *  7.) Input the Blynk authentication in this program token sent by Blynk to your email after registration
 *  8.) Input WiFi SSID and password in this program
 *  9.) When using WiFi only mode, change "disableFlashAutoLoad = 0" to = 1 (LCD and buttons not installed)
 *      this prevents the MPPT unit to load the Flash Memory saved settings and will load the Arduino variable
 *      declarations set below instead
 *  -----------------------------------------------------------------------------------------------------------
 *  GOOGLE DRIVE PROJECT LINK: coming soon
 *  INSTRUCTABLE TUTORIAL LINK: coming soon
 *  YOUTUBE TUTORIAL LINK: www.youtube.com/watch?v=ShXNJM6uHLM
 *  GITHUB UPDATED FUGU FIRMWARE LINK: github.com/AngeloCasi/FUGU-ARDUINO-MPPT-FIRMWARE
 *  -----------------------------------------------------------------------------------------------------------
 *  ACTIVE CHIPS USED IN FIRMWARE:
 *  - ESP32 WROOM32
 *  - ADS1115/ADS1015 I2C ADC
 *  - ACS712-30A Current Sensor IC
 *  - IR2104 MOSFET Driver
 *  - CH340C USB TO UART IC
 *  - 16X2 I2C Character LCD

 *  OTHER CHIPS USED IN PROJECT:
 *  - XL7005A 80V 0.4A Buck Regulator (2x)
 *  - AMS1115-3.3 LDO Linear Regulator 
 *  - AMS1115-5.0 LDO Linear Regulator  
 *  - CSD19505 N-ch MOSFETS (3x)
 *  - B1212 DC-DC Isolated Converter
 *  - SS310 Diodes
 */
//================================ MPPT FIRMWARE LCD MENU INFO =====================================//
// The lines below are for the Firmware Version info displayed on the MPPT's LCD Menu Interface     //
//==================================================================================================//

//====================================== MAIN PROGRAM =============================================//
// The codes below contain all the system processes for the MPPT firmware. Most of them are called //
// from the 8 .ino tabs. The codes are too long, Arduino tabs helped me a lot in organizing them.  //
// The firmware runs on two cores of the Arduino ESP32 as seen on the two separate pairs of void   //
// setups and loops. The xTaskCreatePinnedToCore() freeRTOS function allows you to access the      //
// unused ESP32 core through Arduino. Yes it does multicore processes simultaneously!              // 
//=================================================================================================//

#include "defines.h"
#include "2_Read_Sensors.h"
#include "3_Device_Protection.h"
#include "4_Charging_Algorithm.h"
#include "5_System_Processes.h"
#include "6_Onboard_Telemetry.h"
#include "8_LCD_Menu.h"


//================= CORE0: SETUP (DUAL CORE MODE) =====================//
// void coreTwo(void * pvParameters){
//  setupWiFi();                                              //TAB#7 - WiFi Initialization
// //================= CORE0: LOOP (DUAL CORE MODE) ======================//
//   while(1){
//     Wireless_Telemetry();                                   //TAB#7 - Wireless telemetry (WiFi & Bluetooth)
    
// }
// }

//================== CORE1: SETUP (DUAL CORE MODE) ====================//
void setup() { 
  
  //SERIAL INITIALIZATION          
  Serial.begin(baudRate);                                   //Set serial baud rate
  Serial.println("> Serial Initialized");                   //Startup message
  
  //GPIO PIN INITIALIZATION
  pinMode(backflow_MOSFET,OUTPUT);                          
  pinMode(buck_EN,OUTPUT);
  pinMode(LED,OUTPUT); 
  pinMode(FAN,OUTPUT);
  pinMode(TempSensor,INPUT); 
  pinMode(ADC_ALERT,INPUT);
  pinMode(buttonLeft,INPUT); 
  pinMode(buttonRight,INPUT); 
  pinMode(buttonBack,INPUT); 
  pinMode(buttonSelect,INPUT); 
  
  //PWM INITIALIZATION
  ledcSetup(pwmChannel,pwmFrequency,pwmResolution);          //Set PWM Parameters
  ledcAttachPin(buck_IN, pwmChannel);                        //Set pin as PWM
  ledcWrite(pwmChannel,PWM);                                 //Write PWM value at startup (duty = 0)
  pwmMax = pow(2,pwmResolution)-1;                           //Get PWM Max Bit Ceiling
  pwmMaxLimited = (PWM_MaxDC*pwmMax)/100.000;                //Get maximum PWM Duty Cycle (pwm limiting protection)
  
  //ADC INITIALIZATION
  // ADC_SetGain();                                             //Sets ADC Gain & Range
  // ads.begin();                                               //Initialize ADC

  //GPIO INITIALIZATION                          
  buck_Disable();

  // //ENABLE DUAL CORE MULTITASKING
  // xTaskCreatePinnedToCore(coreTwo,"coreTwo",10000,NULL,0,&Core2,0);
  
  //INITIALIZE AND LIOAD FLASH MEMORY DATA
  // EEPROM.begin(512);
  Serial.println("> FLASH MEMORY: STORAGE INITIALIZED");  //Startup message 
  initializeFlashAutoload();                              //Load stored settings from flash memory       
  Serial.println("> FLASH MEMORY: SAVED DATA LOADED");    //Startup message 

  //LCD INITIALIZATION
  if(enableLCD==1){
    lcd.begin();
    lcd.setBacklight(HIGH);
    lcd.setCursor(0,0);
    lcd.print("MPPT INITIALIZED");
    lcd.setCursor(0,1);
    lcd.print("FIRMWARE ");
    lcd.print(firmwareInfo);    
    delay(1500);
    lcd.clear();
  }

  //SETUP FINISHED
  Serial.println("> MPPT HAS INITIALIZED");                //Startup message

}
//================== CORE1: LOOP (DUAL CORE MODE) ======================//
void loop() {
  Read_Sensors();         //TAB#2 - Sensor data measurement and computation
  Device_Protection();    //TAB#3 - Fault detection algorithm  
  System_Processes();     //TAB#4 - Routine system processes 
  Charging_Algorithm();   //TAB#5 - Battery Charging Algorithm                    
  Onboard_Telemetry();    //TAB#6 - Onboard telemetry (USB & Serial Telemetry)
  LCD_Menu();             //TAB#8 - Low Power Algorithm
}
