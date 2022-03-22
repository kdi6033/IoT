#include <pgmspace.h>
 
#define SECRET
#define THINGNAME ""                         //change this
 
const char WIFI_SSID[] = "";               //change this
const char WIFI_PASSWORD[] = "";           //change this
const char AWS_IOT_ENDPOINT[] = "";       //change this

// 사물 인증서 (파일 이름: xxxxxxxxxx-certificate.pem.crt)
const char AWS_CERT_CRT[] PROGMEM = R"EOF(
-----BEGIN CERTIFICATE-----

-----END CERTIFICATE-----
)EOF";
// 사물 인증서 프라이빗 키 (파일 이름: xxxxxxxxxx-private.pem.key)
const char AWS_CERT_PRIVATE[] PROGMEM = R"EOF(
-----BEGIN RSA PRIVATE KEY-----

-----END RSA PRIVATE KEY-----
)EOF";
// Amazon Trust Services(ATS) 엔드포인트 CA 인증서 (서버인증 > "RSA 2048비트 키: Amazon Root CA 1" 다운로드)
const char AWS_CERT_CA[] PROGMEM = R"EOF(
-----BEGIN CERTIFICATE-----

-----END CERTIFICATE-----
)EOF";
