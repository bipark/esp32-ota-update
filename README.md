# ESP32 OTA UPDATOR
이 프로그램은 ESP32에서 인터넷을 통하여 소프트웨어를 자동으로 업데이트 하는 용도의 소프트웨어 입니다. 첨부의 코드를 기반으로 프로젝트를 구성하면 디바이스가 시작할때마다 서버에서 버전을 확인하고 업데이트를 실행 합니다.

## 사용법  
- esp32_ota_update.ino 파일을 이용해 기능을 구현하고 아두이노 IDE로 빌드하여 ESP32에 설치 합니다.

- Arduino IDE 메뉴에서 Sketch - Export Compiled Binary 실행하면 일반적으로 Build 폴더 아래에 바이너리 파일이 생성됩니다.

- version.json 파일은 업데이트할 버전 번호를 입력하고
  
- http 서버에 업데이트해서 빌드한 바이너리 파일과 첨부된 version.json을 복사해둡니다.

## 기타
- 일부 ESP32 디바이스에서 SPIFFS 때문에  오류가 날수 있습니다. SPIFFS가 아닌 다른 방법으로 파일을 저장하는 방법으로 수정하면 됩니다.

- http://practical.kr
