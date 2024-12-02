/* Comment this out to disable prints and save space */
#define BLYNK_PRINT Serial

/* Fill-in your Template ID (only if using Blynk.Cloud) */
#define BLYNK_TEMPLATE_ID "TMPL6HJu6kyKl"
#define BLYNK_TEMPLATE_NAME "FishFeeder"
#define BLYNK_AUTH_TOKEN "6UNudmwskIHrKxVzj_TQ9iHjbA5fcMt4"


#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include <ESP32Servo.h>
#include <TimeLib.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <ESP_Google_Sheet_Client.h>
//#include <GS_SDHelper.h>

// ####################### What you need to modify #####################
// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).

// Google Project ID
#define PROJECT_ID "fish-feeder-443418"

// Service Account's client email
#define CLIENT_EMAIL "fishfeeder@fish-feeder-443418.iam.gserviceaccount.com"

// Service Account's private key
const char PRIVATE_KEY[] PROGMEM = "-----BEGIN PRIVATE KEY-----\nMIIEvQIBADANBgkqhkiG9w0BAQEFAASCBKcwggSjAgEAAoIBAQCPv2Ar4WJ0PjB/\n7mkJKjkAqwQNoSyv8Z4g4Jq+pTdA62w+q5svjNZaqzP+CLjdndaVagjDFjc+lQUw\npSq2cJrR8cnPBQy/ic8p8oRfnQuk4aM0+ABTi7RHELOYP5XGq3FZRFG2geQqgIfR\nIhZXqHhGe7HBrM6Ez+7x/9wHm9CGIN6CR++lAg3TJwxNJ3YrpBIWQ/BG73n60vK3\naYkE7We+x3n5ueiLzSzPNY4l88K8Q9f0afpr8dpmgFkppwPcwBeP22Jmj23ZGGvW\n0LRdtsjxn8CDiVWqM9KhLwODc0eqAxW8QgO1/1Ux2ch6LcGvrhOMoFx46kOlhYaW\n2bGQScodAgMBAAECggEAGJXp0/d7iFguoWLcciXXlAar7czYQMt5eTQRI3RMi/z4\nCYv+Qda7vswzW0qQm87oKav2Da/aCQkh1x0YvhVqVWgeVRsPAB+I4CrLezL4D95L\ntL6KB8AIOHNh1DNj4ZPnEhZ+R1HV/XL4R8uluJ5m299CGtb8h3is7JUPQ/nhY18w\nLvi5L7VgPYMbzds/Qs8xlIJrOkZsUjhv4whWhoXuGO+h7mdXoA5SoOSRyoQCPoQc\nmRPoC+khnyNpCMcUTmdJ/KoRA5oq5PwurFbbiG20SWwA0pUOyPVoF+qoPRPP8HrH\nRaPOT7sQJcSG2wZYJNhUlcKQeS1mFq1/tsmWAMMuYQKBgQDCjpKsgXFqwy1P/Iie\nMjSba4gFYxNxh/ny+alocxiCrluR2uqtWsGBZltW3zMusTW27czhB2V8ylCfTAoh\nB7bi6rnQ9eK78HAWFR/r0NiEdPNSlPyT/0lx8qFMdrmVOARuzz2DEcjOxXmAKfri\nL8/z7F03kRw9lETIoJVjddONpQKBgQC9JP8NAEn/bM6niEzfX3ImTdNstnbGaC/9\nT0BU2c8gD07+73s05t3LLGTDY/HesPKyb6NDSIVjPsCQ0fCK6iXzZUuk/2myi3k5\n8yuLeQnrQMLCms1/ADbLiYPG0IVrLn1eCafYvcLTmXeh39mjVnkTLUP73r2ej2x+\nAb905gURGQKBgH/PQ4G5w3U8ZM8P+fvaxX54ZLINRR+nEUdSRb0NJ0PqINOvec2K\nOma0hDqMhEYfOkdC7v3BayVQjygmvty2AaoOQyWH8nw+VpPbKs4ys5pIbfSJVWgw\nI+KMXvQpZn8lfpwA3sr06vMS/V95tIvUlwsJ8q7nUCMP/SPDgoAFToXVAoGAHJhf\nRuPoTAlzJLjihlKzuz7A3Aqp4k0WFEHLBAj522r1Em26PsE4fuLzSObvRi+SQY/H\nKwDnyZ1PF5Xlu9qyah3CVeuXWzri1PQCfU+LRFnCGBG3R6AMqLDdj00vHokkwvbK\nvWXS8pKdF4f0K1GDdHa/dHC0Uih0yogRHec4lOECgYEAitmDCPRrnmhkaj00UKaq\n7GC3i0CleX4lQdoYX1lZe/MUc5uQVB52+tQa/DyrAH1y1oln6Hl7mbE8Ni6eBE2g\nh4tZ+SqBL9+vPzuSn66Q5YDGKjJjoPREsrIg2Ah4OE7BcP8EIxTN+inDDO/n5QWn\nmlk5cvFGo0rOoZIx/XAwqPs=\n-----END PRIVATE KEY-----\n";

// The ID of the spreadsheet where you'll publish the data
const char spreadsheetId[] = "1Gk03YvygSDjrSxcy5iwXy-tW2M5DiAnx30aX7zM1hcA";

// Timer variables
unsigned long lastTime = 0;  
unsigned long timerDelay = 30000;

// Token Callback function
void tokenStatusCallback(TokenInfo info);
// Set password to "" for open networks.
char ssid[] = "Bella muerte";
char pass[] = "05302541";

// #####################################################################
Servo myservo;

#define LCD_LINE_1        V0
#define LCD_LINE_2        V1
#define TIMER_1           V2
#define TIMER_2           V3
#define TIMER_3           V4
#define EN_TIMER_1        V5
#define EN_TIMER_2        V6
#define EN_TIMER_3        V7
#define REPEAT_1          V8
#define REPEAT_2          V9
#define REPEAT_3          V10
#define MANUAL_FEED       V11
#define FEED_COUNT        V12
#define CLEAR_FEED        V13
#define Temperature       V15
#define Distance          V16
#define WaterLevel        V17


#define SERVO_PIN         13
#define IR_SENSOR_PIN     12
#define TempSensor        32
#define echoPin           26
#define trigPin           25
#define MANUAL_SW         14
OneWire ourWire(TempSensor);
DallasTemperature sensor(&ourWire);

#define EMPTY_FOOD        (digitalRead(IR_SENSOR_PIN))

#define TIMER_NUMBER      3
BlynkTimer timers;
unsigned long getTime() {
  time_t now;
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
    return 0;
  }
  time(&now);
  return now;
}

//HardwareSerial espSerial(2);
struct str_command
{
  unsigned char start_time_hour[TIMER_NUMBER];
  unsigned char start_time_min[TIMER_NUMBER];
  unsigned char day_timer[TIMER_NUMBER];
  unsigned char feed_repetition[TIMER_NUMBER];
  bool flag_timer_en[TIMER_NUMBER];
};

struct str_command timer;

float duration;
float distance;
float waterLevel=0;
const char* ntpServer = "pool.ntp.org";

unsigned char rtc_hour;
unsigned char rtc_min;
unsigned char rtc_weekday;

unsigned char server_hour;
unsigned char server_min;
unsigned char server_weekday;

unsigned char feed_rep_count;

unsigned int fish_feed_count;

bool flag_food_check;
bool flag_servo_on_food_check;
unsigned char food_check_count;
unsigned char food_detected_timeout;

bool rtc_synchronized;
bool flag_servo_on;
bool flag_servo_on_buf;

bool blynk_manual_feed_update;
void ultrasonic()
{
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);
    duration = pulseIn(echoPin, HIGH);
    distance = duration * 0.034 / 2; //formula to calculate the distance for ultrasonic sensor
    waterLevel=map(distance, 6, 0, 0,100); // 
    Serial.print("Distance: ");
    Serial.println(distance);
    Blynk.virtualWrite(V16, distance);
    Blynk.virtualWrite(V17, waterLevel);
}

void sendSensor()
{
  // Request temperature to all devices on the data line
  sensor.requestTemperatures(); 
  bool send = false;
  Serial.print("Celsius temperature: ");
  //We can connect more than one IC on the same data wire. 0 refers to the first IC on the wire
  Serial.print(sensor.getTempCByIndex(0)); 
  
  Serial.println(sensor.getTempFByIndex(0));
  int tempC=sensor.getTempCByIndex(0);
  // if(tempC<22 || tempC>30)
  // {
  //   if(!send){
  //   espSerial.println(1);
  //   send = true;
  //   }
  // }
  // else
  // {
  //   send = false;
  // }
  
  delay(1000);
  
  // You can send any value at any time.
  // Please don't send more that 10 values per second.
  Blynk.virtualWrite(V15, tempC);
  delay(500);
}
// ######################################################################
void blynk_timer_en_set(unsigned char idx, unsigned char value)
{
  timer.flag_timer_en[idx] = value;
  Serial.println("Timer " + String(idx+1) + " Set: " + String(value));
}

// ######################################################################
void blynk_feed_repetition_set(unsigned char idx, unsigned char value)
{
  timer.feed_repetition[idx] = value;
  Serial.println("Feed Repetition " + String(idx+1) + " Set: " + String(value));
}

// ######################################################################
void blynk_timer_set(unsigned char idx, unsigned char start_hr, unsigned char start_min, unsigned char dow)
{
  timer.start_time_hour[idx] = start_hr;
  timer.start_time_min[idx] = start_min;
  timer.day_timer[idx] = dow;

  Serial.println();
  Serial.println("Timer " + String(idx + 1) + ":");
  Serial.println("Start Time: " + String(start_hr) + ":" + String(start_min));
  Serial.print("Day set: ");
  Serial.println(dow, HEX);
  Serial.println();
  
}

// #####################################################################
BLYNK_CONNECTED()
{
  Serial.println("BLYNK SERVER CONNECTED !!!");
  
  Blynk.syncVirtual(TIMER_1);
  Blynk.syncVirtual(TIMER_2);
  Blynk.syncVirtual(TIMER_3);
  Blynk.syncVirtual(EN_TIMER_1);
  Blynk.syncVirtual(EN_TIMER_2);
  Blynk.syncVirtual(EN_TIMER_3);
  Blynk.syncVirtual(REPEAT_1);
  Blynk.syncVirtual(REPEAT_2);
  Blynk.syncVirtual(REPEAT_3);
  Blynk.syncVirtual(FEED_COUNT);

  Blynk.sendInternal("rtc", "sync");
}  

// #####################################################################
BLYNK_WRITE ( MANUAL_FEED )
{
  int val = param.asInt();  // assigning incomming value from pin to a var

  if ( val )
   flag_servo_on = 1;
}

// #####################################################################
BLYNK_WRITE ( FEED_COUNT )
{
  fish_feed_count = param.asInt();
}

// #####################################################################
BLYNK_WRITE ( CLEAR_FEED )
{   

  int val = param.asInt();  // assigning incomming value from pin to a var
  
  if ( val )
  {
    fish_feed_count = 0;
    Blynk.virtualWrite(LCD_LINE_2, fish_feed_count);
    Blynk.virtualWrite(FEED_COUNT, fish_feed_count);    
  }
}

// #####################################################################
BLYNK_WRITE (REPEAT_1)
{
  int val = param.asInt();  // assigning incomming value from pin to a var
  blynk_feed_repetition_set(0, val);
}

// #####################################################################
BLYNK_WRITE (REPEAT_2)
{
  int val = param.asInt();  // assigning incomming value from pin to a var
  blynk_feed_repetition_set(1, val);
}

// #####################################################################
BLYNK_WRITE (REPEAT_3)
{
  int val = param.asInt();  // assigning incomming value from pin to a var
  blynk_feed_repetition_set(2, val);
}

// #####################################################################
BLYNK_WRITE (EN_TIMER_1)
{
  int val = param.asInt();  // assigning incomming value from pin to a var
  blynk_timer_en_set(0, val);  
}

// #####################################################################
BLYNK_WRITE (EN_TIMER_2)
{
  int val = param.asInt();  // assigning incomming value from pin to a var
  blynk_timer_en_set(1, val);
}

// #####################################################################
BLYNK_WRITE (EN_TIMER_3)
{
  int val = param.asInt();  // assigning incomming value from pin to a var
  blynk_timer_en_set(2, val);
}

// #####################################################################
BLYNK_WRITE( TIMER_1 )
{
  unsigned char week_day;
  unsigned char start_hr, start_min;
  
  TimeInputParam  t(param);
  
  if (t.hasStartTime() )
  {
     start_hr = t.getStartHour();
     start_min = t.getStartMinute();
    

     week_day = 0;
     
     for (int i = 1; i <= 7; i++)
     {
       if (t.isWeekdaySelected(i))  // will be "TRUE" if nothing selected as well
         week_day |= (0x01 << (i-1));
     }

     blynk_timer_set(0, start_hr, start_min, week_day);
  }
  else
  {
    Serial.println("Disabled Timer 1");
  }
}

// #####################################################################
BLYNK_WRITE( TIMER_2 )
{
  unsigned char week_day;
  unsigned char start_hr, start_min;
  
  TimeInputParam  t(param);
  
  if (t.hasStartTime() )
  {
     start_hr = t.getStartHour();
     start_min = t.getStartMinute();
    

     week_day = 0;
     
     for (int i = 1; i <= 7; i++)
     {
       if (t.isWeekdaySelected(i))  // will be "TRUE" if nothing selected as well
         week_day |= (0x01 << (i-1));
     }

     blynk_timer_set(1, start_hr, start_min, week_day);
  }
  else
  {
    Serial.println("Disabled Timer 2");
  }
}

// #####################################################################
BLYNK_WRITE( TIMER_3 )
{
  unsigned char week_day;
  unsigned char start_hr, start_min;
  
  TimeInputParam  t(param);
  
  if (t.hasStartTime() )
  {
     start_hr = t.getStartHour();
     start_min = t.getStartMinute();
    

     week_day = 0;
     
     for (int i = 1; i <= 7; i++)
     {
       if (t.isWeekdaySelected(i))  // will be "TRUE" if nothing selected as well
         week_day |= (0x01 << (i-1));
     }

     blynk_timer_set(2, start_hr, start_min, week_day);
  }
  else
  {
    Serial.println("Disabled Timer 3");
  }
}
  
// #####################################################################
BLYNK_WRITE( InternalPinRTC ) 
{
  const unsigned long DEFAULT_TIME = 1357041600; // Jan 1 2013
  unsigned long blynkTime = param.asLong(); 
  
  if ( blynkTime >= DEFAULT_TIME ) 
  {
    setTime(blynkTime);

    server_hour = hour();
    server_min = minute();    
    server_weekday = weekday();
  
    if ( server_weekday == 1 )
      server_weekday = 7;
    else
      server_weekday -= 1; 

    if ( rtc_synchronized == 0 )
    {
      rtc_hour = server_hour;
      rtc_min = server_min;
      rtc_weekday = server_weekday;
    }
    
    rtc_synchronized = 1;
  }
}

// #####################################################################
unsigned long ms_min_buf;
unsigned long ms_sec_buf;
unsigned char time_10_sec;
unsigned long epochTime;
void checkTime() 
{
  unsigned long time_ms = millis();
  unsigned long ms_min_diff = time_ms - ms_min_buf;
  unsigned long ms_sec_diff = time_ms - ms_sec_buf;
  unsigned long server_time_sec;
  unsigned long real_time_sec;

  // Second management
  if ( ms_sec_diff >= 1000 )  // 1 seconds
  {
    ms_sec_buf = time_ms;

    time_10_sec++;
    if(time_10_sec >= 10)
    {
      time_10_sec = 0;
      Blynk.sendInternal("rtc", "sync"); 
    }    

    // update to blynk
    if ( flag_servo_on_buf != flag_servo_on )
    {
      if ( !((flag_servo_on==0)&&(feed_rep_count)) )
      {
        Blynk.virtualWrite(MANUAL_FEED, flag_servo_on);
      }
                   
      flag_servo_on_buf = flag_servo_on;
    }

    if ( food_detected_timeout )
      food_detected_timeout--;
    
    // servo management
    if ( flag_servo_on )
    {
      flag_servo_on = 0;
      
      if ( feed_rep_count )
        feed_rep_count--;
        myservo.write(180);
        sendToGoogleSheets();


      if ( fish_feed_count < 65535 )
      {
        fish_feed_count++;
        Blynk.virtualWrite(LCD_LINE_2, fish_feed_count);
        Blynk.virtualWrite(FEED_COUNT, fish_feed_count);
      } 

      if ( feed_rep_count == 0 )
      {
        Blynk.logEvent("feeding");
      }

      food_detected_timeout = 3;
      flag_food_check = 0;
    }
    else
    {
      if ( flag_food_check )
      {
        if ( !EMPTY_FOOD ) // found food during checking
        {
          flag_food_check = 0;
          food_detected_timeout = 0;
          Blynk.virtualWrite(LCD_LINE_1, "NORMAL");
        }
        else
        {
          if ( flag_servo_on_food_check )
          {
            flag_servo_on_food_check = 0;
            myservo.write(45);
            
            if ( food_check_count )
              food_check_count--;
          }
          else
          {
            myservo.write(0);
            
            if ( food_check_count )
            { 
              flag_servo_on_food_check = 1;
            }
            else    // not found food
            {
              flag_food_check = 0;
              Blynk.logEvent("empty_food");
              Blynk.virtualWrite(LCD_LINE_1, "EMPTY!!!");
            }
          }
        }
      }
      else
      {
       myservo.write(0);
  
        if ( feed_rep_count )
        {
          flag_servo_on = 1;    
        }
        else
        {
          if ( food_detected_timeout )
          {
            if ( EMPTY_FOOD )
            {
              flag_food_check = 1;
              food_check_count = 3;              
            }
            else
            {
              Blynk.virtualWrite(LCD_LINE_1, "NORMAL");
            }
          }
        }
      }
    }  
  }

  // Minute management
  if ( ms_min_diff >= 60000 )  // 1 min
  {
    ms_min_buf = time_ms;

    // แปลงเวลาให้อยู่ในรูปแบบของวินาที
    server_time_sec = (server_hour * 60 * 60) + (server_min * 60); 
    real_time_sec = (rtc_hour * 60 * 60) + (rtc_min * 60);
  
    // ซิงค์เวลากับ Server เมื่อเวลาปัจจุบันของบอร์ด มีค่าต่างจาก Server มากกว่า 5 นาที
    if   ( rtc_synchronized && 
         (!(server_hour == 23 && server_min == 59 && rtc_hour == 0)) &&
         (((server_time_sec > real_time_sec) && ((server_time_sec - real_time_sec) > 300)) || 
         ((server_time_sec < real_time_sec) && ((real_time_sec - server_time_sec) > 300))) )
         {
            rtc_hour = server_hour;
            rtc_min = server_min;
            rtc_weekday = server_weekday;
         }
  
    // เวลาปัจจุบันของบอร์ด
    rtc_min++;
    if (rtc_min >= 60)
    {
      rtc_min = 0;
      rtc_hour++;
      if (rtc_hour >= 24)
      {
        rtc_hour = 0;
        rtc_weekday++;
        if ( rtc_weekday > 7 )
         rtc_weekday = 1;
      }
    }

    // Check for start
    for (int j=0; j<TIMER_NUMBER; j++)
    {      
      if ( timer.flag_timer_en[j] &&  
           (timer.day_timer[j] == 0x00 || (timer.day_timer[j] & (0x01 << (rtc_weekday - 1) ))) &&
           (( rtc_hour == timer.start_time_hour[j] ) && ( rtc_min == timer.start_time_min[j] )) )
          {              
            feed_rep_count = timer.feed_repetition[j];
            flag_servo_on = 1;
          }                       
    }      
  
    // ส่งค่าเวลาต่างๆ ออกทาง Serial monitor เพื่อตรวจสอบ
    Serial.println("Server time: " + String(server_hour) + ":" + String(server_min));
    Serial.println(String("Server Weekday: ") + String(server_weekday));
    Serial.println("Real Time: " + String(rtc_hour) + ":" + String(rtc_min));
    Serial.println(String("RTC Weekday: ") + String(rtc_weekday));
    Serial.print("Blynk connection status: ");
    Serial.println (Blynk.connected());
    Serial.println();
    
  }  

   // To re-synchronize time again when server is connected
  if ( !Blynk.connected() )
    rtc_synchronized = 0;
}

// #####################################################################
void check_switch()
{
  if ( digitalRead(MANUAL_SW) == 0 )
    flag_servo_on = 1;
}

// #####################################################################
void setup()
{
  // Debug console
  Serial.begin(115200);
  //espSerial.begin(115200, SERIAL_8N1, RXD2, TXD2);  
  sensor.begin();
  
  pinMode(MANUAL_SW, INPUT_PULLUP);
  
  pinMode(trigPin, OUTPUT); 
  pinMode(echoPin, INPUT);
  pinMode(IR_SENSOR_PIN, INPUT_PULLUP);
  myservo.attach(SERVO_PIN, 544, 2400);
  myservo.write(0);   
  
  Blynk.begin(BLYNK_AUTH_TOKEN , ssid, pass);
  timers.setInterval(100L, sendSensor);
  timers.setInterval(100L, ultrasonic);
  
  
  configTime(0, 0, ntpServer);
  GSheet.printf("ESP Google Sheet Client v%s\n\n", ESP_GOOGLE_SHEET_CLIENT_VERSION);

  GSheet.setTokenCallback(tokenStatusCallback);

    // Set the seconds to refresh the auth token before expire (60 to 3540, default is 300 seconds)
  GSheet.setPrerefreshSeconds(10 * 60);

    // Begin the access token generation for Google API authentication
  GSheet.begin(CLIENT_EMAIL, PROJECT_ID, PRIVATE_KEY);

  
  // You can also specify server:
  //Blynk.begin(auth, ssid, pass, "blynk-cloud.com", 80);
  //Blynk.begin(auth, ssid, pass, IPAddress(192,168,1,100), 8080);
 
}

// #####################################################################
void loop()
{
  Blynk.run();
  checkTime();
  timers.run();
  check_switch();
  bool ready = GSheet.ready();
}
String convertTimestampToDateTime(unsigned long timestamp) {
  // Convert the Unix timestamp to struct tm (UTC time)
  struct tm * timeinfo;
  time_t rawtime = timestamp + (3600*7);
  timeinfo = gmtime(&rawtime);  // Use gmtime to get UTC time

  // Format the date and time into DD//MM//YY HH//MM//SS format
  char dateTimeString[20];  // To store formatted date and time string
  strftime(dateTimeString, sizeof(dateTimeString), "%d/%m/%y %H:%M:%S", timeinfo);

  return String(dateTimeString);
}




void sendToGoogleSheets() {
  FirebaseJson response;
  FirebaseJson valueRange;
  bool ready = GSheet.ready();

  // Get the current timestamp
  epochTime = getTime();
  String formattedDateTime = convertTimestampToDateTime(epochTime);
  

  // Add data to FirebaseJson object
  valueRange.add("majorDimension", "COLUMNS");
  valueRange.set("values/[0]/[0]", formattedDateTime);
  valueRange.set("values/[1]/[0]", "Feed success"); // Example message for testing


  Serial.println("\nAppending spreadsheet values...");
  Serial.println("----------------------------");

  // Append values to the spreadsheet
  bool success = GSheet.values.append(
      &response,           // Returned response
      spreadsheetId,       // Spreadsheet ID
      "FishFeeder!A1",         // Range to append
      &valueRange          // Data range to append
  );

  if (success) {
    response.toString(Serial, true); // Print response
    valueRange.clear();             // Clear data after sending
  } else {
    Serial.println(GSheet.errorReason());
  }

  Serial.println();
  Serial.println(ESP.getFreeHeap()); // Print available heap memory
}

void tokenStatusCallback(TokenInfo info) {
  if (info.status == token_status_error) {
    GSheet.printf("Token info: type = %s, status = %s\n", GSheet.getTokenType(info).c_str(), GSheet.getTokenStatus(info).c_str());
    GSheet.printf("Token error: %s\n", GSheet.getTokenError(info).c_str());
  } else {
    GSheet.printf("Token info: type = %s, status = %s\n", GSheet.getTokenType(info).c_str(), GSheet.getTokenStatus(info).c_str());
  }
}
