#define BLINKER_WIFI
#include <Arduino.h>
#include <IRremoteESP8266.h>
#include <IRrecv.h>
#include <IRsend.h>
#include <IRutils.h>
#include <EEPROM.h>

#include <Blinker.h>

const uint16_t kIrLed = 3;  //esp01的gpio03接口（RX接口）
uint16_t RECV_PIN = 2;      //esp01的gpio2接口 
IRsend irsend(kIrLed);  // 定义红外发射端接口 接红外发射器的正极
IRrecv irrecv(RECV_PIN);  //定义红外接收端口

decode_results results;

char auth[] = "********";     //key
char ssid[] = "********";     //ssid
char pswd[] = "********";     //password

BlinkerButton Button0("btn-xuexi"); //学习开关数据键名
BlinkerButton Button1("btn-f");     //发射键数据键名


bool xuexi = false;


void dataRead(const String & data)
{
    BLINKER_LOG("Blinker readString: ", data);

    Blinker.vibrate();
    
    uint32_t BlinkerTime = millis();
    
    Blinker.print("millis", BlinkerTime);
}

decode_type_t protocol;
uint16_t size;
const uint16_t kFrequency = 38000;
bool success = true;

String modedate;
String bitdate;
String date;


void button0_callback(const String & state)
{
    if(state == "on"){   
      Serial.println("开");   
      xuexi = true;
      Button0.print("on");   
      Blinker.delay(100);
      Blinker.println("开始接收");
    }else if (state == "off"){   
      Serial.println("关");
      xuexi = false;
      Button0.print("off");
      Blinker.delay(100);
      Blinker.println("停止接收");
    }
    

}



void button1_callback(const String & state)
{
     Serial.println(state);
     Serial.println(state.substring(0,state.indexOf(',')));
     Serial.println(state.substring(state.indexOf(',')+1,state.lastIndexOf(',')));
     Serial.println(state.substring(state.lastIndexOf(',')+1));
     String modes = state.substring(0,state.indexOf(','));
     char zhi[5] ;
     strcpy(zhi,state.substring(state.indexOf(',')+1,state.lastIndexOf(',')).c_str());
     int data;
     sscanf(zhi, "%x", &data);
     int b = state.substring(state.lastIndexOf(',')+1).toInt();
     Serial.println(data);
     Serial.println(b);
     //irsend.sendRC5(data,b);
       if (modes == "NEC") {
        irsend.sendNEC(data,b);
      } else if (modes == "SONY") {
        irsend.sendSony(data,b);
      } else if (modes == "RC5") {
        irsend.sendRC5(data,b);
      } else if (modes == "RC6") {
        irsend.sendRC6(data,b);
      } else if (modes == "RCMM") {
        irsend.sendRCMM(data,b);
      } else if (modes == "PANASONIC") {
        irsend.sendPanasonic(data,b);
      } else if (modes == "LG") {
        irsend.sendLG(data,b);
      } else if (modes == "JVC") {
        irsend.sendJVC(data,b);
      } else if (modes == "AIWA_RC_T501") {
        irsend.sendAiwaRCT501(data,b);
      } else if (modes == "WHYNTER") {
        irsend.sendWhynter(data,b);
      } else if (modes == "NIKAI") {
        irsend.sendNikai(data,b);
      }
      
     
     

}


void dump(decode_results *results) {
  uint16_t count = results->rawlen;
  if (results->decode_type == UNKNOWN) {
    modedate = "Unknown encoding: ";
  } else if (results->decode_type == NEC) {
    modedate ="NEC";
  } else if (results->decode_type == SONY) {
    modedate ="SONY";
  } else if (results->decode_type == RC5) {
    modedate ="RC5";
  } else if (results->decode_type == RC5X) {
    modedate ="RC5X";
  } else if (results->decode_type == RC6) {
    modedate ="RC6";
  } else if (results->decode_type == RCMM) {
    modedate ="RCMM";
  } else if (results->decode_type == PANASONIC) {
    modedate ="PANASONIC";
    Serial.print(results->address, HEX);
    Serial.print(" Value");
  } else if (results->decode_type == LG) {
    modedate ="LG";
  } else if (results->decode_type == JVC) {
    modedate ="JVC";
  } else if (results->decode_type == AIWA_RC_T501) {
    modedate ="AIWA RC T501";
  } else if (results->decode_type == WHYNTER) {
    modedate ="Whynter";
  } else if (results->decode_type == NIKAI) {
    modedate ="Nikai";
  }
  date = uint64ToString(results->value, 16);
  bitdate = results->bits;
  modedate.concat(",");
  modedate.concat(date);
  modedate.concat(",");
  modedate.concat(bitdate);
  Blinker.println(modedate);
  
}


void setup()
{
    Serial.begin(115200);
    BLINKER_DEBUG.stream(Serial);
    Blinker.begin(auth, ssid, pswd);
    Blinker.attachData(dataRead);
    Blinker.delay(10);
    irsend.begin();
    irrecv.enableIRIn();  
}

void loop()
{
    Blinker.run();
    Button0.attach(button0_callback);
    Button1.attach(button1_callback);

    if(xuexi==true){
        if (irrecv.decode(&results)) { 
        dump(&results);
        irrecv.resume();
      }
    }
}
