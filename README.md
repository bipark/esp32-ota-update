# ESP32 OTA UPDATOR
이 프로그램은 ESP32에서 인터넷을 통하여 소프트웨어를 자동으로 업데이트 하는 용도의 소프트웨어 입니다. 첨부의 코드를 기반으로 프로젝트를 구성하면 디바이스가 시작할때마다 서버에서 버전을 확인하고 업데이트를 실행 합니다.

## 사용법  
- esp32_ota_update.ino 파일을 이용해 기능을 구현하고 아두이노 IDE로 빌드하여 ESP32에 설치 합니다.

- Arduino IDE 메뉴에서 Sketch - Export Compiled Binary 실행하면 일반적으로 Build 폴더 아래에 바이너리 파일이 생성됩니다.

- version.json 파일은 업데이트할 버전 번호를 입력하고
  
- http 서버에 업데이트해서 빌드한 바이너리 파일과 첨부된 version.json을 복사해둡니다.

## 파일 시스템 선택 (SPIFFS vs FFat)
이 프로그램은 버전 정보를 저장하기 위해 ESP32의 파일 시스템을 사용합니다. SPIFFS와 FFat 두 가지 파일 시스템을 선택적으로 사용할 수 있습니다.

### SPIFFS 사용하기
1. Arduino IDE에서 SPIFFS 사용을 위한 설정:
   - Tools > Partition Scheme에서 "Default with spiffs" 또는 적절한 SPIFFS 파티션 스키마 선택
   - 최소 1MB의 SPIFFS 파티션 크기 권장

2. 코드에서 SPIFFS 활성화:
   ```cpp
   const bool USE_SPIFFS = true;  // esp32_ota_update.ino 파일 상단에 위치
   ```

3. SPIFFS 특징:
   - 작은 파일에 적합
   - 디렉토리 구조 미지원
   - 상대적으로 느린 읽기/쓰기 속도
   - 최소 파티션 크기가 작음

### FFat 사용하기
1. Arduino IDE에서 FFat 사용을 위한 설정:
   - Tools > Partition Scheme에서 "Default with ffat" 또는 적절한 FAT 파티션 스키마 선택
   - 최소 1MB의 FAT 파티션 크기 권장

2. 코드에서 FFat 활성화:
   ```cpp
   const bool USE_SPIFFS = false;  // esp32_ota_update.ino 파일 상단에 위치
   ```

3. FFat 특징:
   - 큰 파일에 적합
   - 디렉토리 구조 지원
   - 빠른 읽기/쓰기 속도
   - SPIFFS보다 큰 최소 파티션 크기 필요

### 파일 시스템 선택 시 고려사항
1. 메모리 사용량:
   - SPIFFS: 작은 파일, 제한된 공간에 적합
   - FFat: 큰 파일, 많은 저장 공간이 필요한 경우 적합

2. 성능:
   - SPIFFS: 상대적으로 느림, 단순한 구조
   - FFat: 빠른 속도, 복잡한 파일 시스템 구조 지원

3. 호환성:
   - SPIFFS: ESP32에서 널리 사용됨, 레거시 코드와 호환성 좋음
   - FFat: 최신 기능, FAT32 파일 시스템 호환

## 기타
- 일부 ESP32 디바이스에서 SPIFFS 때문에 오류가 날수 있습니다. 이런 경우 FFat로 전환하여 사용하시면 됩니다.

- http://practical.kr
