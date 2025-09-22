# Sistem-Ruang-Rapat-Pintar-IoT

![Arduino](https://img.shields.io/badge/Arduino-00979D?style=flat&logo=arduino&logoColor=white)
![IoT](https://img.shields.io/badge/IoT-Project-blue)

Sistem ruang rapat pintar berbasis IoT yang mengintegrasikan LCD, sensor gerak, dan sensor suhu untuk monitoring otomatis.

## 🎯 Fitur Utama
- **Monitoring Suhu dan Kelembaban**: Menampilkan suhu dan kelembaban ruangan secara real-time
- **Deteksi Kehadiran**: Menggunakan sensor gerak (PIR) untuk mendeteksi apakah ruangan sedang digunakan
- **Tampilan LCD**: Menampilkan status ruangan, suhu, dan kelembaban pada LCD 16x2
- **Serial Monitor**: Menampilkan data lengkap untuk monitoring dan debugging
- **Cooldown Mechanism**: Mencegah trigger berulang dari sensor gerak dengan cooldown period

## 🛠️ Komponen yang Dibutuhkan
| Komponen | Jumlah | Keterangan |
|----------|--------|------------|
| Arduino Uno | 1 | Mikrokontroler utama |
| LCD I2C 16x2 | 1 | Untuk tampilan informasi |
| Sensor Gerak PIR | 1 | Untuk deteksi kehadiran |
| Sensor Suhu DHT11 | 1 | Untuk mengukur suhu dan kelembaban |
| Resistor 10KΩ | 1 | Pull-up untuk sensor DHT11 (jika diperlukan) |
| Kabel Jumper | 10 | Untuk koneksi |
| Breadboard | 1 | Tempat memasang komponen |

## 🔌 Skema Koneksi
| Komponen | Pin Arduino | Keterangan |
|----------|-------------|------------|
| LCD I2C | SDA (A4), SCL (A5) | Komunikasi I2C |
| Sensor Gerak PIR | Pin 2 | Digital Input |
| Sensor Suhu DHT11 | Pin 3 | Digital Input |
| VCC | 5V | Power Supply |
| GND | GND | Ground |

**Catatan**: 
- LCD I2C menggunakan pin SDA (A4) dan SCL (A5) untuk komunikasi I2C
- Sensor PIR terhubung ke pin digital 2 dengan output HIGH saat mendeteksi gerakan
- Sensor DHT11 terhubung ke pin digital 3 untuk pembacaan suhu dan kelembaban

## 📝 Kode Program
Kode program terdiri dari beberapa fungsi utama:

### Fungsi Setup
```cpp
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
```

### Fungsi Loop
```cpp
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
```

### Fungsi Pembacaan Suhu
```cpp
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
```

### Fungsi Pembacaan Gerak
```cpp
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
```

### Fungsi Update Tampilan LCD
```cpp
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
```
## Rangkaian Digital
<img width="827" height="939" alt="image" src="https://github.com/user-attachments/assets/5b93b9ae-82ea-47b6-8dee-d20fe083889e" />

## 📊 Output Sistem
| Komponen | Output | Deskripsi |
|----------|--------|-----------|
| LCD | Status Ruangan | Menampilkan "Occupied" atau "Vacant" |
| LCD | Suhu | Menampilkan suhu dalam Celsius |
| LCD | Kelembaban | Menampilkan kelembaban dalam persen |
| Serial Monitor | Data Lengkap | Menampilkan status, suhu, dan kelembaban |

## 📹 Video Demo
Lihat video demonstrasi proyek di: [https://youtu.be/ejuj1XKY-ek](https://youtu.be/ejuj1XKY-ek)

## 👨‍💻 Tim Pengembang
Proyek ini dikembangkan oleh **Kelompok 5** sebagai tugas kelompok mata kuliah Internet of Things:

| Nama | NIM | Peran |
|------|-----|------|
| Hanif Luqmanul Hakim | 2022310035 | Ketua, Programmer |
| Bagas Aditya | 2022310063 | Programmer |
| Dedy Wijaya | 2022310052 | Hardware Specialist |
| Muhammad Iqbal | 2022310089 | Documentation |
| Rayan Ikmal Amala | 2022310043 | Testing |

---
**luqmanaru**

Proyek ini dikembangkan sebagai tugas kelompok mata kuliah Internet of Things 1. Sistem dirancang untuk monitoring ruang rapat dengan menampilkan status, suhu, dan kelembaban pada LCD.
