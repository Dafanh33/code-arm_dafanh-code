#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEScan.h>
#include <BLEClient.h>
#include <ESP32Servo.h>

#define SERVICE_UUID        "A1B2C3D4-1111-2222-3333-444455556666"
#define CHARACTERISTIC_UUID "FFEECCD1-0000-9999-8888-777766665555"

Servo s1, s2, s3, s4, s5;
int musikStatus = 0;
BLERemoteCharacteristic* pRemoteCharacteristic;
BLEClient* pClient;

void notifyCallback(BLERemoteCharacteristic* pBLERemoteCharacteristic, uint8_t* pData, size_t length, bool isNotify) {
  musikStatus = ((char)pData[0] == '1') ? 1 : 0;
}

bool connectToServer(BLEAddress pAddress) {
  pClient = BLEDevice::createClient();
  pClient->connect(pAddress);
  BLERemoteService* pRemoteService = pClient->getService(SERVICE_UUID);
  if (pRemoteService == nullptr) return false;
  pRemoteCharacteristic = pRemoteService->getCharacteristic(CHARACTERISTIC_UUID);
  if (pRemoteCharacteristic == nullptr) return false;
  pRemoteCharacteristic->registerForNotify(notifyCallback);
  return true;
}

void setup() {
  Serial.begin(115200);
  BLEDevice::init("ARM_KANAN");

  BLEScan* pScan = BLEDevice::getScan();
  BLEScanResults* foundDevices = pScan->start(5, false);
  for (int i = 0; i < foundDevices->getCount(); i++) {
    BLEAdvertisedDevice dev = foundDevices->getDevice(i);
    if (dev.haveServiceUUID() && dev.isAdvertisingService(BLEUUID(SERVICE_UUID))) {
      connectToServer(dev.getAddress());
      break;
    }
  }

  s1.attach(15); s2.attach(2); s3.attach(4); s4.attach(16); s5.attach(17);
}

void loop() {
  if (musikStatus == 1) {
    for (int pos = 70; pos <= 110; pos += 5) {
      s1.write(pos); s2.write(180 - pos); s3.write(pos); s4.write(180 - pos); s5.write(pos);
      delay(40);
    }
  } else {
    s1.write(90); s2.write(90); s3.write(90); s4.write(90); s5.write(90);
  }
}
