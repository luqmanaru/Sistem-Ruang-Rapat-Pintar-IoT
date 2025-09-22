// Library untuk LCD
#include <LiquidCrystal_I2C.h>

// Library untuk sensor suhu
#include <DHT.h>

// Pin untuk sensor gerak (PIR)
#define PIR_PIN 2

// Pin untuk sensor suhu
#define DHT_PIN 3
#define DHT_TYPE DHT11

// Inisialisasi LCD (alamat 0x27, 16 kolom, 2 baris)
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Inisialisasi sensor suhu
DHT dht(DHT_PIN, DHT_TYPE);

// Variabel untuk status ruangan
bool roomOccupied = false;
float temperature = 0.0;
float humidity = 0.0;

// Variabel untuk debouncing sensor PIR
unsigned long lastPIRTriggerTime = 0;
unsigned long pirCooldown = 5000; // 5 detik cooldown

void setup() {
  // Inisialisasi Serial Monitor
  Serial.begin(9600);
  Serial.println("IoT-Based Meeting Room Manager");
  
  // Inisialisasi LCD
  lcd.init();
  lcd.backlight();
  lcd.print("Meeting Room");
  lcd.setCursor(0, 1);
  lcd.print("Manager System");
  delay(2000);
  lcd.clear();
  
  // Inisialisasi sensor PIR
  pinMode(PIR_PIN, INPUT);
  
  // Inisialisasi sensor DHT
  dht.begin();
  
  // Tampilkan pesan awal
  lcd.print("Initializing...");
  delay(1000);
  lcd.clear();
}

void loop() {
  // Baca sensor suhu
  readTemperature();
  
  // Baca sensor gerak
  readMotion();
  
  // Update tampilan LCD
  updateDisplay();
  
  // Kirim data ke Serial Monitor
  sendDataToSerial();
  
  delay(2000); // Update setiap 2 detik
}

void readTemperature() {
  // Baca suhu dan kelembaban
  humidity = dht.readHumidity();
  temperature = dht.readTemperature();
  
  // Cek jika pembacaan gagal
  if (isnan(humidity) || isnan(temperature)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
}

void readMotion() {
  // Baca status sensor PIR
  int pirState = digitalRead(PIR_PIN);
  
  // Jika terdeteksi gerakan
  if (pirState == HIGH) {
    // Cek cooldown untuk menghindari trigger berulang
    if (millis() - lastPIRTriggerTime > pirCooldown) {
      roomOccupied = true;
      lastPIRTriggerTime = millis();
      Serial.println("Motion detected! Room occupied.");
    }
  } else {
    // Jika tidak ada gerakan, cek apakah sudah melewati waktu cooldown
    if (millis() - lastPIRTriggerTime > pirCooldown) {
      roomOccupied = false;
    }
  }
}

void updateDisplay() {
  // Bersihkan LCD
  lcd.clear();
  
  // Tampilkan status ruangan
  lcd.setCursor(0, 0);
  if (roomOccupied) {
    lcd.print("Status: Occupied");
  } else {
    lcd.print("Status: Vacant");
  }
  
  // Tampilkan suhu dan kelembaban
  lcd.setCursor(0, 1);
  lcd.print("Temp:");
  lcd.print(temperature, 1);
  lcd.print("C ");
  lcd.print("Hum:");
  lcd.print(humidity, 0);
  lcd.print("%");
}

void sendDataToSerial() {
  // Kirim data ke Serial Monitor
  Serial.print("Status Ruangan: ");
  if (roomOccupied) {
    Serial.println("Terisi");
  } else {
    Serial.println("Kosong");
  }
  
  Serial.print("Suhu: ");
  Serial.print(temperature, 1);
  Serial.println(" C");
  
  Serial.print("Kelembaban: ");
  Serial.print(humidity, 0);
  Serial.println(" %");
  
  Serial.println("-------------------");
}
