#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>

BLEServer* pServer;
BLECharacteristic* pCharacteristic;

#define SERVICE_UUID        "A1B2C3D4-1111-2222-3333-444455556666"
#define CHARACTERISTIC_UUID "FFEECCD1-0000-9999-8888-777766665555"

void setup() {
  Serial.begin(115200);
  BLEDevice::init("ESP32_MUSIC_MASTER");

  pServer = BLEDevice::createServer();
  BLEService *pService = pServer->createService(SERVICE_UUID);

  pCharacteristic = pService->createCharacteristic(
                      CHARACTERISTIC_UUID,
                      BLECharacteristic::PROPERTY_READ |
                      BLECharacteristic::PROPERTY_NOTIFY
                    );

  pCharacteristic->setValue("0");
  pService->start();

  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID);
  BLEDevice::startAdvertising();

  Serial.println(" ESP32 MASTER siap kirim status musik ke dua Slave (Kiri & Kanan)");
}

void loop() {
  if (Serial.available() > 0) {
    char musikStatus = Serial.read();
    if (musikStatus == '1' || musikStatus == '0') {
      pCharacteristic->setValue(&musikStatus, 1);
      pCharacteristic->notify();
      Serial.print(" Kirim via BLE ke Slave: ");
      Serial.println(musikStatus);
    }
  }
  delay(100);
}
