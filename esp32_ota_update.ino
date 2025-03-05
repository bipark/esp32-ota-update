#include <WiFi.h>
#include <HTTPUpdate.h>
#include <WiFiClient.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <SPIFFS.h>

const char* ssid = "your_ssid";
const char* password = "your_password";
const char* serverAddress = "http://your-server-address.com:8080"; // 서버 주소
const char* VERSION_FILE = "/version.txt"; // SPIFFS에 저장될 버전 파일
float CURRENT_VERSION = 1.0; // 현재 펌웨어 버전 (초기값)

// 버전 정보를 SPIFFS에 저장
void saveVersion(float version) {
    if (!SPIFFS.begin(true)) {
        Serial.println("SPIFFS 마운트 실패");
        return;
    }
    
    File file = SPIFFS.open(VERSION_FILE, "w");
    if (!file) {
        Serial.println("버전 파일 열기 실패");
        return;
    }
    
    file.println(version);
    file.close();
    Serial.printf("버전 %.1f 저장됨\n", version);
}

// 저장된 버전 정보 읽기
float loadVersion() {
    if (!SPIFFS.begin(true)) {
        Serial.println("SPIFFS 마운트 실패");
        return CURRENT_VERSION;
    }
    
    if (!SPIFFS.exists(VERSION_FILE)) {
        Serial.println("저장된 버전 정보 없음, 초기 버전 사용");
        saveVersion(CURRENT_VERSION); // 초기 버전 저장
        return CURRENT_VERSION;
    }
    
    File file = SPIFFS.open(VERSION_FILE, "r");
    if (!file) {
        Serial.println("버전 파일 열기 실패");
        return CURRENT_VERSION;
    }
    
    String versionStr = file.readStringUntil('\n');
    file.close();
    
    float savedVersion = versionStr.toFloat();
    Serial.printf("저장된 버전: %.1f\n", savedVersion);
    return savedVersion;
}

// 서버에서 버전 정보를 확인하는 함수
float checkServerVersion() {
    HTTPClient http;
    float serverVersion = 0.0;
    
    // version.json 파일을 요청
    String versionUrl = String(serverAddress) + "/version.json";
    http.begin(versionUrl);
    
    int httpCode = http.GET();
    if (httpCode == HTTP_CODE_OK) {
        String payload = http.getString();
        
        // JSON 파싱을 위한 메모리 할당
        StaticJsonDocument<200> doc;
        DeserializationError error = deserializeJson(doc, payload);
        
        if (!error) {
            serverVersion = doc["version"].as<float>();
            Serial.printf("서버 버전: %.1f\n", serverVersion);
        } else {
            Serial.println("JSON 파싱 실패");
        }
    } else {
        Serial.printf("버전 확인 실패, HTTP 코드: %d\n", httpCode);
    }
    
    http.end();
    return serverVersion;
}

void performOTAUpdate() {
    WiFiClient client;
    
    // 먼저 서버 버전 확인
    float serverVersion = checkServerVersion();
    
    if (serverVersion <= CURRENT_VERSION) {
        Serial.println("현재 최신 버전입니다.");
        return;
    }
    
    Serial.println("새로운 버전이 있습니다. 업데이트를 시작합니다...");
    
    // 펌웨어 파일 URL
    String firmwareUrl = String(serverAddress) + "/ota_sample.ino.bin";
    
    // OTA 업데이트 시작
    t_httpUpdate_return ret = httpUpdate.update(client, firmwareUrl);
    
    switch(ret) {
        case HTTP_UPDATE_FAILED:
            Serial.printf("HTTP_UPDATE_FAILED Error (%d): %s\n", 
                          httpUpdate.getLastError(), 
                          httpUpdate.getLastErrorString().c_str());
            break;
        
        case HTTP_UPDATE_NO_UPDATES:
            Serial.println("업데이트 파일이 없습니다");
            break;
        
        case HTTP_UPDATE_OK:
            Serial.println("업데이트 성공");
            saveVersion(serverVersion); // 업데이트 성공 시 새 버전 저장
            break;
    }
}

void connectToWiFi() {
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("\nWiFi connected");
    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());
}

void setup() {
    Serial.begin(115200);
    
    // 저장된 버전 정보 로드
    CURRENT_VERSION = loadVersion();
    Serial.printf("현재 펌웨어 버전: %.1f\n", CURRENT_VERSION);
    
    connectToWiFi();
    performOTAUpdate();
}

void loop() {
    Serial.println("Your code here...");
    delay(1000);
}
 