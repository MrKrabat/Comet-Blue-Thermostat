#include "CometBlue.h"


/**************************************************************************/
/*!
    @brief class constructor
    @param mac Bluetooth MAC address of comet blue device
*/
/**************************************************************************/
CometBlue::CometBlue(std::string mac) : _deviceAddress(mac) {
    //
}

/**************************************************************************/
/*!
    @brief inits bluetooth and connect to comet blue
    @param deviceName Name of bluetooth device
    @param pin Device Pin
*/
/**************************************************************************/
bool CometBlue::connect(std::string deviceName, uint32_t pin) {
    // check if connected
    if (_connected) {
        return true;
    }

    // connect to device
    BLEDevice::init(deviceName);
    _client = BLEDevice::createClient();
    if (!_client->connect(_deviceAddress)) {
        return false;
    }

    // check service uuid
    _remoteService = _client->getService(BLEUUID(SERVICE_UUID));
    if (_remoteService == nullptr) {
        _client->disconnect();
        return false;
    }

    // authenticate
    BLERemoteCharacteristic* pRemotePin = _remoteService->getCharacteristic(BLEUUID(PIN_UUID));
    if (pRemotePin == nullptr) {
        _client->disconnect();
        return false;
    }
    uint8_t pinencoded[4] = {(uint8_t)(pin & 0xFF), (uint8_t)((pin >> 8) & 0xFF), (uint8_t)((pin >> 16) & 0xFF), (uint8_t)((pin >> 24) & 0xFF)};
    pRemotePin->writeValue(pinencoded, sizeof(pinencoded), true);

    // verify if successful
    std::string value = _remoteService->getValue(BLEUUID(BAT_UUID));
    if (value.length() == 0) {
        _client->disconnect();
        return false;
    }

    _connected = true;
    return true;
}

/**************************************************************************/
/*!
    @brief disconnects from bluetooth device
*/
/**************************************************************************/
void CometBlue::disconnect(void) {
    // check if connected
    if (!_connected) {
        return;
    }
    _client->disconnect();
    _connected = false;
    _client = NULL;
    _remoteService = NULL;
}

/**************************************************************************/
/*!
    @brief checks if connected to device and connection still open
    @return true if connected to device
*/
/**************************************************************************/
bool CometBlue::isConnected(void) {
    // check if connected
    if (_connected) {
        // check if connection still open
        if(_client->isConnected()) {
            return true;
        }
        disconnect();
        return false;
    }
    return false;
}

/**************************************************************************/
/*!
    @brief Get device name
    @return Device name
*/
/**************************************************************************/
String CometBlue::getDeviceName(void) {
    // check if connected
    if (!isConnected()) {
        return "NOT CONNECTED";
    }

    // get service
    BLERemoteService* deviceName = _client->getService(BLEUUID("00001800-0000-1000-8000-00805f9b34fb"));
    if (deviceName == nullptr) {
        return "ERROR";
    }

    // get value
    std::string value = deviceName->getValue(BLEUUID("00002a00-0000-1000-8000-00805f9b34fb"));
    if (value.length() == 0) {
        return "ERROR";
    }
    return value.c_str();
}

/**************************************************************************/
/*!
    @brief Get device model number
    @return Model number
*/
/**************************************************************************/
String CometBlue::getModelNumber(void) {
    // check if connected
    if (!isConnected()) {
        return "NOT CONNECTED";
    }

    // get service
    BLERemoteService* deviceName = _client->getService(BLEUUID(SERVICE_INFO));
    if (deviceName == nullptr) {
        return "ERROR";
    }

    // get value
    std::string value = deviceName->getValue(BLEUUID("00002a24-0000-1000-8000-00805f9b34fb"));
    if (value.length() == 0) {
        return "ERROR";
    }
    return value.c_str();
}

/**************************************************************************/
/*!
    @brief Get device firmware revision
    @return Firmware revision
*/
/**************************************************************************/
String CometBlue::getFirmwareRevision(void) {
    // check if connected
    if (!isConnected()) {
        return "NOT CONNECTED";
    }

    // get service
    BLERemoteService* deviceName = _client->getService(BLEUUID(SERVICE_INFO));
    if (deviceName == nullptr) {
        return "ERROR";
    }

    // get value
    std::string value = deviceName->getValue(BLEUUID("00002a26-0000-1000-8000-00805f9b34fb"));
    if (value.length() == 0) {
        return "ERROR";
    }
    return value.c_str();
}

/**************************************************************************/
/*!
    @brief Get device firmware revision
    @return Firmware revision
*/
/**************************************************************************/
String CometBlue::getFirmwareRevision2(void) {
    // check if connected
    if (!isConnected()) {
        return "NOT CONNECTED";
    }

    // get value
    std::string value = _remoteService->getValue(BLEUUID("47e9ee2d-47e9-11e4-8939-164230d1df67"));
    if (value.length() == 0) {
        return "ERROR";
    }
    return value.c_str();
}

/**************************************************************************/
/*!
    @brief Get device software revision
    @return Software revision
*/
/**************************************************************************/
String CometBlue::getSoftwareRevision(void) {
    // check if connected
    if (!isConnected()) {
        return "NOT CONNECTED";
    }

    // get service
    BLERemoteService* deviceName = _client->getService(BLEUUID(SERVICE_INFO));
    if (deviceName == nullptr) {
        return "ERROR";
    }

    // get value
    std::string value = deviceName->getValue(BLEUUID("00002a28-0000-1000-8000-00805f9b34fb"));
    if (value.length() == 0) {
        return "ERROR";
    }
    return value.c_str();
}

/**************************************************************************/
/*!
    @brief Get device manufacturer name
    @return Manufacturer name
*/
/**************************************************************************/
String CometBlue::getManufacturerName(void) {
    // check if connected
    if (!isConnected()) {
        return "NOT CONNECTED";
    }

    // get service
    BLERemoteService* deviceName = _client->getService(BLEUUID(SERVICE_INFO));
    if (deviceName == nullptr) {
        return "ERROR";
    }

    // get value
    std::string value = deviceName->getValue(BLEUUID("00002a29-0000-1000-8000-00805f9b34fb"));
    if (value.length() == 0) {
        return "ERROR";
    }
    return value.c_str();
}

/**************************************************************************/
/*!
    @brief Get signal strength to bluetooth device
    @return Signal strength percentage [0-100]
*/
/**************************************************************************/
uint8_t CometBlue::getSignalStrength(void) {
    // check if connected
    if (!isConnected()) {
        return 0;
    }
    int rssi = _client->getRssi();

    if (rssi >= -50) {
        return 100;
    } else if (rssi <= -100) {
        return 0;
    }
    return 2 * (rssi + 100);
}

/**************************************************************************/
/*!
    @brief Get battery charge
    @return Battery charge percentage [0-100]
*/
/**************************************************************************/
uint8_t CometBlue::getBattery(void) {
    // check if connected
    if (!isConnected()) {
        return 0;
    }

    // get value
    std::string value = _remoteService->getValue(BLEUUID(BAT_UUID));
    if (value.length() != 1) {
        return 0;
    }
    return value[0];
}

/**************************************************************************/
/*!
    @brief Get device date and time in UTC
    @return time_t struct
*/
/**************************************************************************/
time_t CometBlue::getDateTime(void) {
    // check if connected
    if (!isConnected()) {
        return time(NULL);
    }

    // get value
    std::string value = _remoteService->getValue(BLEUUID(DATETIME_UUID));
    if (value.length() != 5) {
        return time(NULL);
    }

    // decode date
    struct tm devicetime;
    devicetime.tm_sec = 0;
    devicetime.tm_min = value[0] - 1;
    devicetime.tm_hour = value[1] - 1;
    devicetime.tm_mday = value[2];
    devicetime.tm_mon = value[3] - 1;
    devicetime.tm_year = value[4] + 100;
    return mktime(&devicetime);
}

/**************************************************************************/
/*!
    @brief Get LCD timeout
    @return LCDTimeout struct
*/
/**************************************************************************/
LCDTimeout CometBlue::getLCDTimeout(void) {
    LCDTimeout ret;
    // check if connected
    if (!isConnected()) {
        return ret;
    }

    // get value
    std::string value = _remoteService->getValue(BLEUUID("47e9ee2e-47e9-11e4-8939-164230d1df67"));
    if (value.length() != 2) {
        return ret;
    }
    ret = {value[0], value[1]};
    return ret;
}

/**************************************************************************/
/*!
    @brief Get device status, bit-codes:
    childlock    0x80
    manual mode  0x1
    adapting     0x400
    not ready    0x200
    installing   0x400 | 0x200 | 0x100
    motor moving 0x100
    antifrost activated 0x10
    satisfied   0x80000
    low battery 0x800
    @return status
*/
/**************************************************************************/
uint32_t CometBlue::getStatus(void) {
    // check if connected
    if (!isConnected()) {
        return 0;
    }

    // get value
    std::string value = _remoteService->getValue(BLEUUID("47e9ee2a-47e9-11e4-8939-164230d1df67"));
    if (value.length() != 3) {
        return 0;
    }
    uint32_t result = (((uint32_t)value[2]) << 16) | (((uint32_t)value[1]) << 8) | ((uint32_t)value[0]);
    return result;
}

/**************************************************************************/
/*!
    @brief Get temperature data from device
    @return Temperatures struct
*/
/**************************************************************************/
Temperatures CometBlue::getTemperatures(void) {
    Temperatures ret;
    // check if connected
    if (!isConnected()) {
        return ret;
    }

    // get value
    std::string value = _remoteService->getValue(BLEUUID("47e9ee2b-47e9-11e4-8939-164230d1df67"));
    if (value.length() != 7) {
        return ret;
    }
    ret = {value[0] / 2.0, value[1] / 2.0 , value[2] / 2.0, value[3] / 2.0, value[4] / 2.0, value[5], value[6]};
    return ret;
}

/**************************************************************************/
/*!
    @brief Sets new device pin
    @param pin New pin
    @return true if successful
*/
/**************************************************************************/
bool CometBlue::setPin(uint32_t pin) {
    // check if connected
    if (!isConnected()) {
        return false;
    }

    // get characteristic
    BLERemoteCharacteristic* pRemotePin = _remoteService->getCharacteristic(BLEUUID(PIN_UUID));
    if (pRemotePin == nullptr) {
        return false;
    }

    // write data
    uint8_t pinencoded[4] = {(uint8_t)(pin & 0xFF), (uint8_t)((pin >> 8) & 0xFF), (uint8_t)((pin >> 16) & 0xFF), (uint8_t)((pin >> 24) & 0xFF)};
    pRemotePin->writeValue(pinencoded, sizeof(pinencoded), true);
    return true;
}

/**************************************************************************/
/*!
    @brief Sets device date and time in UTC
    @param time time_t struct
    @return true if successful
*/
/**************************************************************************/
bool CometBlue::setDateTime(time_t time) {
    // check if connected
    if (!isConnected()) {
        return false;
    }

    // get characteristic
    BLERemoteCharacteristic* pDateTime = _remoteService->getCharacteristic(BLEUUID(DATETIME_UUID));
    if (pDateTime == nullptr) {
        return false;
    }

    // write data
    tm dt = *localtime(&time);
    uint8_t datetime[5] = {dt.tm_min + 1, dt.tm_hour + 1, dt.tm_mday, dt.tm_mon + 1, dt.tm_year - 100};
    pDateTime->writeValue(datetime, sizeof(datetime), true);
    return true;
}

/**************************************************************************/
/*!
    @brief Sets device lcd timeout
    @param timeout in seconds
    @return true if successful
*/
/**************************************************************************/
bool CometBlue::setLCDTimeout(uint8_t timeout) {
    // check if connected
    if (!isConnected()) {
        return false;
    }

    // get characteristic
    BLERemoteCharacteristic* pLCDTimeout = _remoteService->getCharacteristic(BLEUUID("47e9ee2e-47e9-11e4-8939-164230d1df67"));
    if (pLCDTimeout == nullptr) {
        return false;
    }

    // write data
    uint8_t timeoutencoded[2] = {timeout, 0};
    pLCDTimeout->writeValue(timeoutencoded, sizeof(timeoutencoded), true);
    return true;
}

/**************************************************************************/
/*!
    @brief Sets device status code
    @param status Value
    @return true if successful
*/
/**************************************************************************/
bool CometBlue::setStatus(uint32_t status) {
    // check if connected
    if (!isConnected()) {
        return false;
    }

    // get characteristic
    BLERemoteCharacteristic* pStatus = _remoteService->getCharacteristic(BLEUUID("47e9ee2a-47e9-11e4-8939-164230d1df67"));
    if (pStatus == nullptr) {
        return false;
    }

    // write data
    uint8_t statusencoded[3] = {(uint8_t)(status & 0xFF), (uint8_t)((status >> 8) & 0xFF), (uint8_t)((status >> 16) & 0xFF)};
    pStatus->writeValue(statusencoded, sizeof(statusencoded), true);
    return true;
}

/**************************************************************************/
/*!
    @brief Sets device temperatures
    @param temp Temperatures struct
    @return true if successful
*/
/**************************************************************************/
bool CometBlue::setTemperatures(Temperatures temp) {
    // check if connected
    if (!isConnected()) {
        return false;
    }

    // get characteristic
    BLERemoteCharacteristic* pTemp = _remoteService->getCharacteristic(BLEUUID(TEMP_UUID));
    if (pTemp == nullptr) {
        return false;
    }

    // write data
    uint8_t tempencoded[7] = {128, temp.manual_temp * 2, temp.target_temp_low * 2, temp.target_temp_high * 2, temp.offset_temp * 2, temp.window_open_detect, temp.window_open_minutes};
    pTemp->writeValue(tempencoded, sizeof(tempencoded), true);
    return true;
}

String CometBlue::convertString(std::string str) {
    String ret = "";
    for (uint8_t i=0; i<str.length(); i++) {
        ret += String(str[i], DEC) + " ";
    }
    return ret;
}
