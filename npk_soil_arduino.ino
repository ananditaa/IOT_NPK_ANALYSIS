#include <AltSoftSerial.h>

// RO to pin 8 & DI to pin 9 when using AltSoftSerial
#define RE 6
#define DE 7
const int sensor_pin = A1;
const byte temp[] = {0x01,0x03, 0x00, 0x13, 0x00, 0x02, 0x35, 0xCE};//
const byte mois[]  = {0x01,0x03,0x00,0x12,0x00,0x02,0x64,0x0E};
const byte econ[] = {0x01,0x03, 0x00, 0x15, 0x00, 0x01, 0x95, 0xCE};
const byte ph[] = {0x01,0x03, 0x00, 0x06, 0x00, 0x01, 0x64, 0x0B};//0x0B64

const byte nitro[] = { 0x01, 0x03, 0x00, 0x1E, 0x00, 0x01, 0xE4, 0x0C };
const byte phos[] = { 0x01, 0x03, 0x00, 0x1F, 0x00, 0x01, 0xB5, 0xCC };
const byte pota[] = { 0x01, 0x03, 0x00, 0x20, 0x00, 0x01, 0x85, 0xC0 };

byte values[11];
AltSoftSerial mod;

float envhumidity = 0.0, envtemperature = 0.0, soil_ph = 0.0, soil_mois = 0.0, soil_temp = 0.0;
byte val1 = 0, val2 = 0, val3 = 0, val4 = 0,val5 = 0, val6 = 0, val7 = 0;

void setup() {
  Serial.begin(9600);
  mod.begin(9600);
  pinMode(RE, OUTPUT);
  pinMode(DE, OUTPUT);

  // put RS-485 into receive mode
  digitalWrite(DE, LOW);
  digitalWrite(RE, LOW);

  delay(3000);
}

void loop() {
  val1 = moisture();
  //soil_mois = val1/10.0;
  delay(500);
  soil_temp = temperature();
  delay(500);
  val3 = econduc();
  delay(500);
  val4 = phydrogen();
  soil_ph = val4;
  delay(500);
  val5 = nitrogen();
  delay(500);
  val6 = phosphorous();
  delay(500);
  val7 = potassium();
  delay(500);

//  Serial.print("Moisture: ");Serial.print(soil_mois);Serial.println(" %");
//  delay(100);
//  Serial.print("Temperature: ");Serial.print(soil_temp);Serial.println(" C");
//  delay(100);
  float moisture_percentage;
  int sensor_analog;
  sensor_analog = analogRead(sensor_pin);
  moisture_percentage = ( 100 - ( (sensor_analog/1023.00) * 100 ) );
  Serial.print("Moisture Percentage = ");
  Serial.print(moisture_percentage);
  Serial.print("%\n\n");
  delay(500);
  Serial.print("EC: ");Serial.print(val3);Serial.println(" us/cm");
  delay(1000);
//  Serial.print("ph: ");Serial.print(soil_ph);Serial.println(" ph");
//  delay(100); 
  Serial.print("Nitrogen: "); Serial.print(val5);Serial.println(" mg/kg");
  delay(500);
  Serial.print("Phosphorous: ");Serial.print(val6);Serial.println(" mg/kg");
  delay(500);
  Serial.print("Potassium: ");Serial.print(val7);Serial.println(" mg/kg");
  Serial.println();
  delay(500);
}

byte moisture() {
  // clear the receive buffer
  mod.flushInput();

  // switch RS-485 to transmit mode
  digitalWrite(DE, HIGH);
  digitalWrite(RE, HIGH);
  delay(500);

  // write out the message
  for (uint8_t i = 0; i < sizeof(mois); i++) mod.write(mois[i]);

  // wait for the transmission to complete
  mod.flush();

  // switching RS485 to receive mode
  digitalWrite(DE, LOW);
  digitalWrite(RE, LOW);

  // delay to allow response bytes to be received!
  delay(500);

  // read in the received bytes
  for (byte i = 0; i < 7; i++) {
    values[i] = mod.read();
    Serial.print(values[i], HEX);
    Serial.print(' ');
  }
  uint16_t combinedBytes = (values[3] << 8) | values[4];
  float floatValue = static_cast<float>(combinedBytes);

  Serial.print("Moisture: ");
  Serial.println(floatValue/10);
}

byte temperature() {
  // clear the receive buffer
  mod.flushInput();

  // switch RS-485 to transmit mode
  digitalWrite(DE, HIGH);
  digitalWrite(RE, HIGH);
  delay(500);

  // write out the message
  for (uint8_t i = 0; i < sizeof(temp); i++) mod.write(temp[i]);

  // wait for the transmission to complete
  mod.flush();

  // switching RS485 to receive mode
  digitalWrite(DE, LOW);
  digitalWrite(RE, LOW);

  // delay to allow response bytes to be received!
  delay(2000);

  // read in the received bytes
  for (byte i = 0; i < 7; i++) {
    values[i] = mod.read();
    Serial.print(values[i], HEX);
    Serial.print(' ');
  }
  uint16_t combinedBytes = (values[5] << 8) | values[6];
  float floatValue = static_cast<float>(combinedBytes);

  Serial.print("Temperature: ");
  Serial.println(floatValue/10);
}

byte econduc() {
  // clear the receive buffer
  mod.flushInput();

  // switch RS-485 to transmit mode
  digitalWrite(DE, HIGH);
  digitalWrite(RE, HIGH);
  delay(500);

  // write out the message
  for (uint8_t i = 0; i < sizeof(econ); i++) mod.write(econ[i]);

  // wait for the transmission to complete
  mod.flush();

  // switching RS485 to receive mode
  digitalWrite(DE, LOW);
  digitalWrite(RE, LOW);

  // delay to allow response bytes to be received!
  delay(500);

  // read in the received bytes
  for (byte i = 0; i < 7; i++) {
    values[i] = mod.read();
    //Serial.print(values[i], HEX);
    //Serial.print(' ');
  }
  return values[4];
}

byte phydrogen() {
  // clear the receive buffer
  mod.flushInput();
  // switch RS-485 to transmit mode
  digitalWrite(DE, HIGH);
  digitalWrite(RE, HIGH);
  delay(500);

  // write out the message
  for (uint8_t i = 0; i < sizeof(ph); i++) mod.write(ph[i]);

  // wait for the transmission to complete
  mod.flush();

  // switching RS485 to receive mode
  digitalWrite(DE, LOW);
  digitalWrite(RE, LOW);

  // delay to allow response bytes to be received!
  delay(500);

  // read in the received bytes
  for (byte i = 0; i < 7; i++) {
    values[i] = mod.read();
    //Serial.print(values[i], HEX);
    //Serial.print(' ');
  }
  uint16_t combinedBytes = (values[3] << 8) | values[4];
  float floatValue = static_cast<float>(combinedBytes);

  Serial.print("pH: ");
  Serial.println(floatValue/100);
}

byte nitrogen() {
  // clear the receive buffer
  mod.flushInput();

  // switch RS-485 to transmit mode
  digitalWrite(DE, HIGH);
  digitalWrite(RE, HIGH);
  delay(500);

  // write out the message
  for (uint8_t i = 0; i < sizeof(nitro); i++) mod.write(nitro[i]);

  // wait for the transmission to complete
  mod.flush();

  // switching RS485 to receive mode
  digitalWrite(DE, LOW);
  digitalWrite(RE, LOW);

  // delay to allow response bytes to be received!
  delay(500);

  // read in the received bytes
  for (byte i = 0; i < 7; i++) {
    values[i] = mod.read();
    //Serial.print(values[i], HEX);
    //Serial.print(' ');
  }
  return values[4];
}

byte phosphorous() {
  mod.flushInput();
  digitalWrite(DE, HIGH);
  digitalWrite(RE, HIGH);
  delay(500);
  for (uint8_t i = 0; i < sizeof(phos); i++) mod.write(phos[i]);
  mod.flush();
  digitalWrite(DE, LOW);
  digitalWrite(RE, LOW);
  // delay to allow response bytes to be received!
  delay(500);
  for (byte i = 0; i < 7; i++) {
    values[i] = mod.read();
    // Serial.print(values[i], HEX);
    // Serial.print(' ');
  }
  return values[4];
}

byte potassium() {
  mod.flushInput();
  digitalWrite(DE, HIGH);
  digitalWrite(RE, HIGH);
  delay(500);
  for (uint8_t i = 0; i < sizeof(pota); i++) mod.write(pota[i]);
  mod.flush();
  digitalWrite(DE, LOW);
  digitalWrite(RE, LOW);
  // delay to allow response bytes to be received!
  delay(500);
  for (byte i = 0; i < 7; i++) {
    values[i] = mod.read();
    // Serial.print(values[i], HEX);
    // Serial.print(' ');
  }
  return values[4];
}
