
// 사물 인증서 (파일 이름: xxxxxxxxxx-certificate.pem.crt)
const char cert_str[] PROGMEM = R"EOF(
-----BEGIN CERTIFICATE-----
CMxTNbykYmafHlaaTY2cYXlts02qml89/8+QKp2UJ6fZerY6ojuZBUpnVrqaPDg2
3L3oMFT4izxL8tiaw5xaWpfBQo6c8mnh3oHdWFMDNhcIdzoQ4ja2CJIScBL06EEv
hzFRf0yqHKk6WYJP1j9QVXU6J/BtkVgLxJXAG5dtbMwhr2AA+Y6+en0ihGeN+Q==
-----END CERTIFICATE-----
)EOF";
// 사물 인증서 프라이빗 키 (파일 이름: xxxxxxxxxx-private.pem.key)
const char key_str[] PROGMEM = R"EOF(
-----BEGIN RSA PRIVATE KEY-----
T+Iyllkl3EKeBDuYL+nzRFQ8/l1l2Jf8OKr8R6qNj875aHiHYquc9ETlPdLuB901
zSoVHCku8Z9YlhhFeHkpwbZ2j4QEfRCZXz2CEwcmrm03r+bFTK2nJ949MiOp1esh
Bx4srQKBgQCjb2WqglrEK/y5grA/AS9AfL6An0M4GZIc4c+K9rIL839rHdBUjUZD
zPpXX6MBkB8/ncc/w+WkSOh+Fs7PrIRZSxVVGTxQD8AblofO98slf7FGH8XT6/MO
o5X9ixjJc0U4DpeyrK0yTy6PgfXPtcoJef2b4EFDV9HFWoNuN2UniQ==
-----END RSA PRIVATE KEY-----

)EOF";
// Amazon Trust Services(ATS) 엔드포인트 CA 인증서 (서버인증 > "RSA 2048비트 키: Amazon Root CA 1" 다운로드)
const char ca_str[] PROGMEM = R"EOF(
-----BEGIN CERTIFICATE-----
o/ufQJVtMVT8QtPHRh8jrdkPSHCa2XV4cdFyQzR1bldZwgJcJmApzyMZFo6IQ6XU
5MsI+yMRQ+hDKXJioaldXgjUkK642M4UwtBV8ob2xJNDd2ZhwLnoQdeXeGADbkpy
rqXRfboQnoZsG4q5WTP468SQvvG5
-----END CERTIFICATE-----
)EOF";
