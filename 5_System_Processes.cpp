#include "5_System_Processes.h"

#include "defines.h"
#include <EEPROM.h>
#include "Preferences.h"
#include "nvs.h"
#include "nvs_flash.h"
#include "esp32-hal-log.h"

const char * my_nvs_errors[] = { "OTHER", "NOT_INITIALIZED", "NOT_FOUND", "TYPE_MISMATCH", "READ_ONLY", "NOT_ENOUGH_SPACE", "INVALID_NAME", "INVALID_HANDLE", "REMOVE_FAILED", "KEY_TOO_LONG", "PAGE_FULL", "INVALID_STATE", "INVALID_LENGTH"};
#define my_nvs_error(e) (((e)>ESP_ERR_NVS_BASE)? my_nvs_errors[(e)&~(ESP_ERR_NVS_BASE)]: my_nvs_errors[0])


class MyPreferences : public Preferences
{
public:
  MyPreferences() : Preferences() {}


  size_t myPutChar(const char* key, int8_t value){
      if(!_started || !key || _readOnly){
          return 0;
      }
      esp_err_t err = nvs_set_i8(_handle, key, value);
      if(err){
          log_e("nvs_set_i8 fail: %s %s", key, my_nvs_error(err));
          return 0;
      }
      return 1;
  }

  size_t myPutUChar(const char* key, uint8_t value){
      if(!_started || !key || _readOnly){
          return 0;
      }
      esp_err_t err = nvs_set_u8(_handle, key, value);
      if(err){
          log_e("nvs_set_u8 fail: %s %s", key, my_nvs_error(err));
          return 0;
      }
      return 1;
  }

  size_t myPutShort(const char* key, int16_t value){
      if(!_started || !key || _readOnly){
          return 0;
      }
      esp_err_t err = nvs_set_i16(_handle, key, value);
      if(err){
          log_e("nvs_set_i16 fail: %s %s", key, my_nvs_error(err));
          return 0;
      }
      return 2;
  }

  size_t myPutUShort(const char* key, uint16_t value){
      if(!_started || !key || _readOnly){
          return 0;
      }
      esp_err_t err = nvs_set_u16(_handle, key, value);
      if(err){
          log_e("nvs_set_u16 fail: %s %s", key, my_nvs_error(err));
          return 0;
      }
      return 2;
  }

  size_t myPutInt(const char* key, int32_t value){
      if(!_started || !key || _readOnly){
          return 0;
      }
      esp_err_t err = nvs_set_i32(_handle, key, value);
      if(err){
          log_e("nvs_set_i32 fail: %s %s", key, my_nvs_error(err));
          return 0;
      }
      return 4;
  }

  size_t myPutUInt(const char* key, uint32_t value){
      if(!_started || !key || _readOnly){
          return 0;
      }
      esp_err_t err = nvs_set_u32(_handle, key, value);
      if(err){
          log_e("nvs_set_u32 fail: %s %s", key, my_nvs_error(err));
          return 0;
      }
      return 4;
  }

  size_t myPutLong(const char* key, int32_t value){
      return myPutInt(key, value);
  }

  size_t myPutULong(const char* key, uint32_t value){
      return myPutUInt(key, value);
  }

  size_t myPutLong64(const char* key, int64_t value){
      if(!_started || !key || _readOnly){
          return 0;
      }
      esp_err_t err = nvs_set_i64(_handle, key, value);
      if(err){
          log_e("nvs_set_i64 fail: %s %s", key, my_nvs_error(err));
          return 0;
      }
      return 8;
  }

  size_t myPutULong64(const char* key, uint64_t value){
      if(!_started || !key || _readOnly){
          return 0;
      }
      esp_err_t err = nvs_set_u64(_handle, key, value);
      if(err){
          log_e("nvs_set_u64 fail: %s %s", key, my_nvs_error(err));
          return 0;
      }
      return 8;
  }

  size_t myPutFloat(const char* key, const float_t value){
      return myPutBytes(key, (void*)&value, sizeof(float_t));
  }

  size_t myPutDouble(const char* key, const double_t value){
      return myPutBytes(key, (void*)&value, sizeof(double_t));
  }

  size_t myPutBool(const char* key, const bool value){
      return myPutUChar(key, (uint8_t) (value ? 1 : 0));
  }

  size_t myPutString(const char* key, const char* value){
      if(!_started || !key || !value || _readOnly){
          return 0;
      }
      esp_err_t err = nvs_set_str(_handle, key, value);
      if(err){
          log_e("nvs_set_str fail: %s %s", key, my_nvs_error(err));
          return 0;
      }
      return strlen(value);
  }

  size_t myPutString(const char* key, const String value){
      return myPutString(key, value.c_str());
  }

  size_t myPutBytes(const char* key, const void* value, size_t len){
      if(!_started || !key || !value || !len || _readOnly){
          return 0;
      }
      esp_err_t err = nvs_set_blob(_handle, key, value, len);
      if(err){
          log_e("nvs_set_blob fail: %s %s", key, my_nvs_error(err));
          return 0;
      }
      return len;
  }

  void myCommit() {
    esp_err_t err = nvs_commit(_handle);
    if(err){
        log_e("nvs_commit fail: %s", my_nvs_error(err));
        return ;
    }

  }

};

void resetVariables(){
  secondsElapsed = 0;
  energySavings  = 0; 
  daysRunning    = 0; 
  timeOn         = 0; 
}

void System_Processes(){
  ///////////////// FAN COOLING /////////////////
  if(enableFan==true){
    if(enableDynamicCooling==false){                                   //STATIC PWM COOLING MODE (2-PIN FAN - no need for hysteresis, temp data only refreshes after 'avgCountTS' or every 500 loop cycles)                       
      if(overrideFan==true){fanStatus=true;}                           //Force on fan
      else if(temperature>=temperatureFan){fanStatus=1;}               //Turn on fan when set fan temp reached
      else if(temperature<temperatureFan){fanStatus=0;}                //Turn off fan when set fan temp reached
      digitalWrite(FAN,fanStatus);                                     //Send a digital signal to the fan MOSFET
    }
    else{}                                                             //DYNAMIC PWM COOLING MODE (3-PIN FAN - coming soon)
  }
  else{digitalWrite(FAN,LOW);}                                         //Fan Disabled
  
  //////////// LOOP TIME STOPWATCH ////////////
  loopTimeStart = micros();                                            //Record Start Time
  loopTime = (loopTimeStart-loopTimeEnd)/1000.000;                     //Compute Loop Cycle Speed (mS)
  loopTimeEnd = micros();                                              //Record End Time

  ///////////// AUTO DATA RESET /////////////
  if(telemCounterReset==0){}                                           //Never Reset
  else if(telemCounterReset==1 && daysRunning>1)  { resetVariables(); }  //Daily Reset
  else if(telemCounterReset==2 && daysRunning>7)  { resetVariables(); }  //Weekly Reset
  else if(telemCounterReset==3 && daysRunning>30) { resetVariables(); }  //Monthly Reset
  else if(telemCounterReset==4 && daysRunning>365){ resetVariables(); }  //Yearly Reset 

  ///////////// LOW POWER MODE /////////////
  if(lowPowerMode==1){}   
  else{}      
}

MyPreferences prefe;

void setupPreferences() {
  prefe.begin("MPPT");
}


void loadSettings() { 
  flashMemLoad = prefe.getBool("flashMemLoad", 1);
  MPPT_Mode = prefe.getBool("MPPT_Mode", 1);
  output_Mode = prefe.getBool("output_Mode", 1);
  enableFan = prefe.getBool("enableFan", 1);
  enableWiFi = prefe.getBool("enableWiFi", 1);
  temperatureFan = prefe.getInt("temperatureFan", 60);
  temperatureMax = prefe.getInt("temperatureMax", 90);
  backlightSleepMode = prefe.getInt("backlightSleepMode", 0);
  voltageBatteryMax = prefe.getFloat("voltageBatteryMax", 25.5);
  voltageBatteryMin = prefe.getFloat("voltageBatteryMin", 20);
  currentChargingMax = prefe.getFloat("currentChargingMax", 50);

  // MPPT_Mode          = EEPROM.read(0);                       // Load saved charging mode setting
  // output_Mode        = EEPROM.read(12);                      // Load saved charging mode setting
  // voltageBatteryMax  = EEPROM.read(1)+(EEPROM.read(2)*.01);  // Load saved maximum battery voltage setting
  // voltageBatteryMin  = EEPROM.read(3)+(EEPROM.read(4)*.01);  // Load saved minimum battery voltage setting
  // currentChargingMax    = EEPROM.read(5)+(EEPROM.read(6)*.01);  // Load saved charging current setting
  // enableFan          = EEPROM.read(7);                       // Load saved fan enable settings
  // temperatureFan     = EEPROM.read(8);                       // Load saved fan temperature settings
  // temperatureMax     = EEPROM.read(9);                       // Load saved shutdown temperature settings
  // enableWiFi         = EEPROM.read(10);                      // Load saved WiFi enable settings  
  // flashMemLoad       = EEPROM.read(11);                      // Load saved flash memory autoload feature
  // backlightSleepMode = EEPROM.read(13);                      // Load saved lcd backlight sleep timer
}

void saveSettings() {

  prefe.myPutBool("MPPT_Mode", MPPT_Mode);
  prefe.myPutBool("output_Mode", output_Mode);
  prefe.myPutBool("enableFan", enableFan);
  prefe.myPutBool("enableWiFi", enableWiFi);
  prefe.myPutInt("temperatureFan", temperatureFan);
  prefe.myPutInt("temperatureMax", temperatureMax);
  prefe.myPutInt("backlightSleepMode", backlightSleepMode);
  prefe.myPutFloat("voltageBatteryMax", voltageBatteryMax);
  prefe.myPutFloat("voltageBatteryMin", voltageBatteryMin);
  prefe.myPutFloat("currentChargingMax", currentChargingMax);
  prefe.myCommit();

//   // EEPROM.write(0,MPPT_Mode);           //STORE: Algorithm 
  // EEPROM.write(12,output_Mode);        //STORE: Charge/PSU Mode Selection 
//   conv1 = voltageBatteryMax*100;       //STORE: Maximum Battery Voltage (gets whole number)
//   conv2 = conv1%100;                   //STORE: Maximum Battery Voltage (gets decimal number and converts to a whole number)
//   EEPROM.write(1,voltageBatteryMax);
//   EEPROM.write(2,conv2);
//   conv1 = voltageBatteryMin*100;       //STORE: Minimum Battery Voltage (gets whole number)
//   conv2 = conv1%100;                   //STORE: Minimum Battery Voltage (gets decimal number and converts to a whole number)
//   EEPROM.write(3,voltageBatteryMin);
//   EEPROM.write(4,conv2);
//   conv1 = currentChargingMax*100;         //STORE: Charging Current
//   conv2 = conv1%100;
//   EEPROM.write(5,currentChargingMax);
//   EEPROM.write(6,conv2);
//   EEPROM.write(7,enableFan);           //STORE: Fan Enable
//   EEPROM.write(8,temperatureFan);      //STORE: Fan Temp
//   EEPROM.write(9,temperatureMax);      //STORE: Shutdown Temp
//   EEPROM.write(10,enableWiFi);         //STORE: Enable WiFi
// //EEPROM.write(11,flashMemLoad);       //STORE: Enable autoload (must be excluded from bulk save, uncomment under discretion)
//   EEPROM.write(13,backlightSleepMode); //STORE: LCD backlight sleep timer 
//   EEPROM.commit();                     //Saves setting changes to flash memory
}

void factoryReset(){
  prefe.myPutBool("flashMemLoad", 1);
  prefe.myPutBool("MPPT_Mode", 1);
  prefe.myPutBool("output_Mode", 1);
  prefe.myPutBool("enableFan", 1);
  prefe.myPutBool("enableWiFi", 1);
  prefe.myPutInt("temperatureFan", 60);
  prefe.myPutInt("temperatureMax", 90);
  prefe.myPutInt("backlightSleepMode", 0);
  prefe.myPutFloat("voltageBatteryMax", 25.5);
  prefe.myPutFloat("voltageBatteryMin", 20);
  prefe.myPutFloat("currentChargingMax", 50);
  prefe.myCommit();

  // EEPROM.write(0,1);  //STORE: Charging Algorithm (1 = MPPT Mode)
  // EEPROM.write(12,1); //STORE: Charger/PSU Mode Selection (1 = Charger Mode)
  // EEPROM.write(1,12); //STORE: Max Battery Voltage (whole)
  // EEPROM.write(2,0);  //STORE: Max Battery Voltage (decimal)
  // EEPROM.write(3,9);  //STORE: Min Battery Voltage (whole)
  // EEPROM.write(4,0);  //STORE: Min Battery Voltage (decimal) 
  // EEPROM.write(5,30); //STORE: Charging Current (whole)
  // EEPROM.write(6,0);  //STORE: Charging Current (decimal)
  // EEPROM.write(7,1);  //STORE: Fan Enable (Bool)
  // EEPROM.write(8,60); //STORE: Fan Temp (Integer)
  // EEPROM.write(9,90); //STORE: Shutdown Temp (Integer)
  // EEPROM.write(10,1); //STORE: Enable WiFi (Boolean)
  // EEPROM.write(11,1); //STORE: Enable autoload (on by default)
  // // EEPROM.write(13,0); //STORE: LCD backlight sleep timer (default: 0 = never)
  // EEPROM.commit();
  loadSettings();
}

void saveAutoloadSettings(){
  // EEPROM.write(11,flashMemLoad);       //STORE: Enable autoload
  // EEPROM.commit();                     //Saves setting changes to flash memory
  prefe.putBool("flashMemLoad", flashMemLoad);
}

void initializeFlashAutoload() {
  if(disableFlashAutoLoad==0){
    flashMemLoad = prefe.getBool("flashMemLoad", false);
    // flashMemLoad = EEPROM.read(11);       //Load saved autoload (must be excluded from bulk save, uncomment under discretion) 
    if(flashMemLoad==1){loadSettings();}  //Load stored settings from flash memory  
  } 
}
