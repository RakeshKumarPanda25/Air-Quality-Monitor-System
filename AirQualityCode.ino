#include <Wire.h> 
#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>
#include <DHT.h>
#include <math.h>

#define DHTPIN 2
#define DHTTYPE DHT11

#define MQ135_PIN A0
#define MQ7_PIN   A1

#define RL 10000.0    // Load resistor (ohms)
#define RO 41763.0    // Calibrated clean-air resistance

DHT dht(DHTPIN, DHTTYPE);
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// ---------- Gas Curve Constants (from MQ135 datasheet) ----------
float A_CO2 = 116.6020682, B_CO2 = -2.769034857;   // CO2
float A_NH3 = 102.2,         B_NH3 = -2.769;       // Ammonia
float A_BEN = 26.42,         B_BEN = -1.546;       // Benzene
float A_SO2 = 30,            B_SO2 = -1.77;        // Sulfur
float A_CO  = 605.18,        B_CO  = -3.937;       // Carbon Monoxide (rough via MQ135)

// ---------- Threshold for alerts ----------
const int THRESHOLD_CO2 = 800;  // ppm values indicative of poor air
const int THRESHOLD_NH3 = 50;
const int THRESHOLD_BEN = 20;
const int THRESHOLD_SO2 = 30;
const int THRESHOLD_CO  = 40;

const unsigned long ALERT_DURATION = 2000;
unsigned long alertStart = 0;
bool showAlert = false;
bool alertShown = false;
String alertMsg = "";

// ---------- Function to compute PPM ----------
float computePPM(int adc, float a, float b) {
  float Vrl = adc * (5.0 / 1023.0);
  float Rs = ((5.0 - Vrl) * RL) / Vrl;
  float ratio = Rs / RO;
  return a * pow(ratio, b);
}

void setup() {
  Serial.begin(9600);
  dht.begin();
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();
  delay(2000);
}

void loop() {
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();

  int mq135ADC = analogRead(MQ135_PIN);
  int mq7ADC = analogRead(MQ7_PIN);

  // Compute gas concentrations (ppm)
  float co2  = computePPM(mq135ADC, A_CO2, B_CO2);
  float nh3  = computePPM(mq135ADC, A_NH3, B_NH3);
  float ben  = computePPM(mq135ADC, A_BEN, B_BEN);
  float so2  = computePPM(mq135ADC, A_SO2, B_SO2);
  float co   = computePPM(mq7ADC, A_CO,  B_CO);

  // Determine which gases exceed safe limits
  alertMsg = "";
  if (!alertShown) {
    if (co2 > THRESHOLD_CO2) alertMsg += "CO2 ";
    if (nh3 > THRESHOLD_NH3) alertMsg += "NH3 ";
    if (ben > THRESHOLD_BEN) alertMsg += "C6H6 ";
    if (so2 > THRESHOLD_SO2) alertMsg += "S ";
    if (co  > THRESHOLD_CO)  alertMsg += "CO ";
  }

  if (alertMsg != "" && !alertShown) {
    showAlert = true;
    alertShown = true;
    alertStart = millis();
  }

  // ----------- OLED alert display -----------
  if (showAlert) {
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 20);
    display.print("ALERT! High:");
    display.setCursor(0, 35);
    display.print(alertMsg);
    display.display();
    if (millis() - alertStart >= ALERT_DURATION) {
      showAlert = false;
    }
  } else {
    // ------------- Normal Display -------------
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);

    display.setCursor(0, 0);
    display.print("T:"); display.print(temperature);
    display.print("C H:"); display.print(humidity); display.print("%");

    display.setCursor(0, 12);
    display.print("CO2:"); display.print(co2, 0); display.print("ppm");

    display.setCursor(0, 22);
    display.print("NH3:"); display.print(nh3, 0);
    display.print("ppm  S:"); display.print(so2, 0); display.print("ppm");

    display.setCursor(0, 32);
    display.print("C6H6:"); display.print(ben, 0); display.print("ppm");

    display.setCursor(0, 42);
    display.print("CO:"); display.print(co, 0); display.print("ppm");

    display.display();

    // Serial output for debugging
    Serial.print("CO2:"); Serial.print(co2); Serial.print("ppm  ");
    Serial.print("NH3:"); Serial.print(nh3); Serial.print("ppm  ");
    Serial.print("C6H6:"); Serial.print(ben); Serial.print("ppm  ");
    Serial.print("SO2:"); Serial.print(so2); Serial.print("ppm  ");
    Serial.print("CO:"); Serial.print(co); Serial.println("ppm");
  }

  delay(1000);
}
