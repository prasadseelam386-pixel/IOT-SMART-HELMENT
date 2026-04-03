#define BLYNK_TEMPLATE_ID "TMPL3KcNncHUt"
#define BLYNK_TEMPLATE_NAME "SMART HELMET"
#define BLYNK_AUTH_TOKEN "rn2KW2BzfwRma3yNC5ef8jCAmsWw18p5"

#ifdef ESP32
  #include <WiFi.h>
  #include <BlynkSimpleEsp32.h>
#else
  #include <ESP8266WiFi.h>
  #include <BlynkSimpleEsp8266.h>
#endif

#define BLYNK_PRINT Serial
 
char auth[] = "rn2KW2BzfwRma3yNC5ef8jCAmsWw18p5";    // authuntifaction id
char ssid[] = "project02";   //wifi name
char pass[] = "project02";   //wifi password

#include <TinyGPS++.h>
TinyGPSPlus gps;
#include<SoftwareSerial.h>
SoftwareSerial gpsSerial(D5,D6);
float latitude,longitude;

#include<LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27,16,2);

#include <Wire.h>

// MPU6050 Slave Device Address
const uint8_t MPU6050SlaveAddress = 0x68;

// Select SDA and SCL pins for I2C communication 
const uint8_t scl = D1;
const uint8_t sda = D2;

// sensitivity scale factor respective to full scale setting provided in datasheet 
const uint16_t AccelScaleFactor = 16384;
const uint16_t GyroScaleFactor = 131;

// MPU6050 few configuration register addresses
const uint8_t MPU6050_REGISTER_SMPLRT_DIV   =  0x19;
const uint8_t MPU6050_REGISTER_USER_CTRL    =  0x6A;
const uint8_t MPU6050_REGISTER_PWR_MGMT_1   =  0x6B;
const uint8_t MPU6050_REGISTER_PWR_MGMT_2   =  0x6C;
const uint8_t MPU6050_REGISTER_CONFIG       =  0x1A;
const uint8_t MPU6050_REGISTER_GYRO_CONFIG  =  0x1B;
const uint8_t MPU6050_REGISTER_ACCEL_CONFIG =  0x1C;
const uint8_t MPU6050_REGISTER_FIFO_EN      =  0x23;
const uint8_t MPU6050_REGISTER_INT_ENABLE   =  0x38;
const uint8_t MPU6050_REGISTER_ACCEL_XOUT_H =  0x3B;
const uint8_t MPU6050_REGISTER_SIGNAL_PATH_RESET  = 0x68;

int16_t AccelX, AccelY, AccelZ, Temperature, GyroX, GyroY, GyroZ;
double Ax, Ay, Az, T, Gx, Gy, Gz;

int a=0;
int b=0;
int c=0;
int d=0;

#define buzzer D3
#define motor D8

#define sw D7
int sw_sense;
String helmet_status="NORMAL";

#define gas A0
int gas_sense;
String gas_status="CHECKING";

String status="CHECKING";

void beep()
{
  digitalWrite(buzzer,HIGH);delay(300);digitalWrite(buzzer,LOW);delay(200);
}
void setup()
{
  Serial.begin(9600);
  gpsSerial.begin(9600);

  lcd.init();
  lcd.backlight();

  pinMode(gas,INPUT);

  pinMode(sw,INPUT_PULLUP);

  pinMode(buzzer,OUTPUT);
  pinMode(motor,OUTPUT);
  digitalWrite(buzzer,LOW);
  digitalWrite(motor,LOW);

  pinMode(D0,OUTPUT);
  digitalWrite(D0,LOW);
  
  Wire.begin(sda, scl);
  MPU6050_Init();

  lcd.clear();
  lcd.print("   ** READY **  ");
  delay(1500);

  lcd.clear();
  lcd.print("CONNECTING TO...");
  lcd.setCursor(0,1);
  lcd.print(ssid);
  delay(500);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, pass);
  
  while(WiFi.status() != WL_CONNECTED)
  {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }
  Serial.println(WiFi.localIP());

  Blynk.config(auth);

  lcd.clear();
  lcd.print(" WIFI CONNECTED ");
  lcd.setCursor(0,1);
  lcd.print(WiFi.localIP());
  delay(1000);
}

void loop()
{
  Blynk.run();

  d=d+1;
  
  Read_RawValue(MPU6050SlaveAddress, MPU6050_REGISTER_ACCEL_XOUT_H);
  
  //divide each with their sensitivity scale factor
  Ax = (double)AccelX/AccelScaleFactor;
  Ay = (double)AccelY/AccelScaleFactor;
  Az = (double)AccelZ/AccelScaleFactor;
  T = (double)Temperature/340+36.53; //temperature formula
  Gx = (double)GyroX/GyroScaleFactor;
  Gy = (double)GyroY/GyroScaleFactor;
  Gz = (double)GyroZ/GyroScaleFactor;

  Serial.print("Ax: "); Serial.print(Ax);
  Serial.print(" Ay: "); Serial.print(Ay);
  Serial.print(" Az: "); Serial.print(Az);
  Serial.print(" T: "); Serial.print(T);
  Serial.print(" Gx: "); Serial.print(Gx);
  Serial.print(" Gy: "); Serial.print(Gy);
  Serial.print(" Gz: "); Serial.println(Gz);

  sw_sense=digitalRead(sw);
  sw_sense=1-sw_sense;
  Serial.print("SW : "); Serial.println(sw_sense);

  if(sw_sense==0)
  {
    b=0;
    helmet_alert();
  }
  else
  {
    helmet_normal();
  }

  if(d==20)
  {
    d=0;
    Blynk.virtualWrite(V0,gas_sense);
  }

  if(c==2)
  {
    Blynk.logEvent("helmet_alert");
  }

  if(b==2)
  {
    Blynk.logEvent("alcohol_alert");
    Blynk.virtualWrite(V0,gas_sense);
  }
  Serial.print("B : ");Serial.println(b);

  if(a==1)
  {
    Blynk.logEvent("fall_alert");
    Blynk.virtualWrite(V0,gas_sense);
  }

  lcd.clear();
  lcd.print("HELMET : ");lcd.print(sw_sense);
  lcd.setCursor(0,1);
  lcd.print("STATUS : ");lcd.print(helmet_status);
  digitalWrite(D0,LOW);delay(300);digitalWrite(D0,LOW);delay(200);
  digitalWrite(D0,LOW);delay(300);digitalWrite(D0,LOW);delay(200);
  lcd.clear();
  lcd.print("ALCOHOL : ");lcd.print(gas_sense);
  lcd.setCursor(0,1);
  lcd.print("STATUS : ");lcd.print(gas_status);
  digitalWrite(D0,LOW);delay(300);digitalWrite(D0,LOW);delay(200);
  digitalWrite(D0,LOW);delay(300);digitalWrite(D0,LOW);delay(200);
  lcd.clear();
  lcd.print("TEMP : ");lcd.print(T-10);
  lcd.setCursor(0,1);
  lcd.print("STATUS : ");lcd.print(status);
  digitalWrite(D0,LOW);delay(300);digitalWrite(D0,LOW);delay(200);
  digitalWrite(D0,LOW);delay(300);digitalWrite(D0,LOW);delay(200);
}
void helmet_alert()
{
  helmet_status="ALERT";
  
  c=c+1;
  if(c>=10)
  {
    c=10;
  }
  
  digitalWrite(motor,LOW);
  
  lcd.clear();
  lcd.print(" *** ALERT *** ");
  lcd.setCursor(0,1);
  lcd.print("  WEAR HELMET  ");
  beep();
}
void helmet_normal()
{
  helmet_status="NORMAL";
  c=0;
  alcohol_loop();
}
void alcohol_loop()
{
  gas_sense=analogRead(gas);
  gas_sense=map(gas_sense,0,1023,0,100);
  Serial.print("GAS : "); Serial.println(gas_sense);

  if(gas_sense >= 35)
  {
    status="ALERT";
    gas_status="ALERT";
    digitalWrite(motor,LOW);

    lcd.clear();
    lcd.print(" *** ALERT *** ");
    lcd.setCursor(0,1);
    lcd.print("ALCOHOL DETECTED");
    beep();
    beep();
    
    b=b+1;

    if(b>=10)
    {
      b=10;
    }
  }
  else
  {
    gas_status="NORMAL";
    b=0;
    accident_loop();
  }
}
void location_data()
{
  if(gpsSerial.available() > 0)
  {
    if (gps.encode(gpsSerial.read()))
    {
      if (gps.location.isValid())
      {
        latitude = gps.location.lat();
        longitude = gps.location.lng();
      }
    }
  }
}
void I2C_Write(uint8_t deviceAddress, uint8_t regAddress, uint8_t data){
  Wire.beginTransmission(deviceAddress);
  Wire.write(regAddress);
  Wire.write(data);
  Wire.endTransmission();
}

// read all 14 register
void Read_RawValue(uint8_t deviceAddress, uint8_t regAddress){
  Wire.beginTransmission(deviceAddress);
  Wire.write(regAddress);
  Wire.endTransmission();
  Wire.requestFrom(deviceAddress, (uint8_t)14);
  AccelX = (((int16_t)Wire.read()<<8) | Wire.read());
  AccelY = (((int16_t)Wire.read()<<8) | Wire.read());
  AccelZ = (((int16_t)Wire.read()<<8) | Wire.read());
  Temperature = (((int16_t)Wire.read()<<8) | Wire.read());
  GyroX = (((int16_t)Wire.read()<<8) | Wire.read());
  GyroY = (((int16_t)Wire.read()<<8) | Wire.read());
  GyroZ = (((int16_t)Wire.read()<<8) | Wire.read());
}

//configure MPU6050
void MPU6050_Init(){
  delay(150);
  I2C_Write(MPU6050SlaveAddress, MPU6050_REGISTER_SMPLRT_DIV, 0x07);
  I2C_Write(MPU6050SlaveAddress, MPU6050_REGISTER_PWR_MGMT_1, 0x01);
  I2C_Write(MPU6050SlaveAddress, MPU6050_REGISTER_PWR_MGMT_2, 0x00);
  I2C_Write(MPU6050SlaveAddress, MPU6050_REGISTER_CONFIG, 0x00);
  I2C_Write(MPU6050SlaveAddress, MPU6050_REGISTER_GYRO_CONFIG, 0x00);//set +/-250 degree/second full scale
  I2C_Write(MPU6050SlaveAddress, MPU6050_REGISTER_ACCEL_CONFIG, 0x00);// set +/- 2g full scale
  I2C_Write(MPU6050SlaveAddress, MPU6050_REGISTER_FIFO_EN, 0x00);
  I2C_Write(MPU6050SlaveAddress, MPU6050_REGISTER_INT_ENABLE, 0x01);
  I2C_Write(MPU6050SlaveAddress, MPU6050_REGISTER_SIGNAL_PATH_RESET, 0x00);
  I2C_Write(MPU6050SlaveAddress, MPU6050_REGISTER_USER_CTRL, 0x00);
}
void accident_loop()
{
  if(Ax >= 0.55  || Ax <= -0.55 || Ay >= 0.55 || Ay <= -0.55)
  {
    status="ALERT";
    a=a+1;

    if(a>=10)
    {
      a=10;
    }
    Serial.println("ALERT");
    digitalWrite(buzzer,HIGH);
    digitalWrite(motor,LOW);

    lcd.clear();
    lcd.print(" *** ALERT *** ");
    lcd.setCursor(0,1);
    lcd.print("ACCIDENT DETECTED");
    delay(1000);
  }
  else
  {
    status="NORMAL";
    a=0;
    Serial.println("NORMAL");
    digitalWrite(buzzer,LOW);
    digitalWrite(motor,HIGH);
  }
}
