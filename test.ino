#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEScan.h>
#include <BLEAdvertisedDevice.h>
#include <BLEUUID.h>
#include <list>

constexpr int scanTime = 5; //In seconds
constexpr const char* SERVICE_UUID = "90d8fdc0-0cae-41df-ac77-b74d280fb9c4";  // CRAP SERVICE ID

std::list<BLEAdvertisedDevice> deviceList;
 
class MyAdvertisedDeviceCallbacks: public BLEAdvertisedDeviceCallbacks {
    void onResult(BLEAdvertisedDevice advertisedDevice) {
      if (advertisedDevice.haveServiceUUID()) {
            if (advertisedDevice.isAdvertisingService(BLEUUID(SERVICE_UUID))) {
              deviceList.push_back(advertisedDevice);
            }
      }
    }       
};

void setup() {
  Serial.begin(115200);
  Serial.println("Scanning...");
  BLEDevice::init("");
  BLEScan* pBLEScan = BLEDevice::getScan();
  pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
  pBLEScan->setActiveScan(true); //active scan uses more power, but get results faster
}

void loop() {
  deviceList.clear();
  BLEScan* pBLEScan = BLEDevice::getScan();
  BLEScanResults foundDevices = pBLEScan->start(scanTime);
  Serial.print("Devices found: ");
  Serial.println(foundDevices.getCount());
  Serial.println("Scan done!");
  pBLEScan->stop();
  Serial.println("Found Devices:");
  for(auto entry: deviceList) {
    Serial.print("Device name:");
    Serial.println(entry.getName().c_str());
  }
  Serial.println();
  Serial.println("================================");
  Serial.println();
  delay(5000);
}
