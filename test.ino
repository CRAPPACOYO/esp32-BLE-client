#include <BLEDevice.h>
#include <BLEAddress.h>
#include <BLEUtils.h>
#include <BLEScan.h>
#include <BLEAdvertisedDevice.h>
#include <BLEUUID.h>
#include <list>

constexpr int scanTime = 5; //In seconds
constexpr const char* SERVICE_UUID = "90d8fdc0-0cae-41df-ac77-b74d280fb9c4";  // CRAP SERVICE ID

std::list<BLEAdvertisedDevice> deviceList;
BLEClient* mClient = nullptr;

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
  mClient = BLEDevice::createClient();
  BLEScan* pBLEScan = BLEDevice::getScan();
  pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
  pBLEScan->setActiveScan(true); //active scan uses more power, but get results faster
}

void connectToDevice(BLEAddress& pAddress) {
  Serial.print("Forming a connection to ");
  Serial.println(pAddress.toString().c_str());
  // Connect to the remove BLE Server.
  mClient->connect(pAddress);
  Serial.println(" - Connected to server");

  // Obtain a reference to the service we are after in the remote BLE server.
  BLERemoteService* pRemoteService = mClient->getService(SERVICE_UUID);
  if (pRemoteService == nullptr) {
    Serial.print("Failed to find our service UUID: ");
    Serial.println(SERVICE_UUID);
    return;
  }
  Serial.println("Found our service");

  mClient->disconnect();
  Serial.println("Disconnected");
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
  for (auto entry : deviceList) {
    Serial.print("Device name:");
    Serial.println(entry.getName().c_str());
    BLEAddress address = entry.getAddress();
    connectToDevice(address);
  }
  Serial.println();
  Serial.println("================================");
  Serial.println();
  delay(5000);
}
