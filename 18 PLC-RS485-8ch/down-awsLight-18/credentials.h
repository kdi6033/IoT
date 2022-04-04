
// 사물 인증서 (파일 이름: xxxxxxxxxx-certificate.pem.crt)
const char cert_str[] PROGMEM = R"EOF(
-----BEGIN CERTIFICATE-----
A1UdDgQWBBQ9wkE3//oMNVYU9RN6N660jdCVxjAMBgNVHRMBAf8EAjAAMA4GA1Ud
DwEB/wQEAwIHgDANBgkqhkiG9w0BAQsFAAOCAQEAM9qCqnaOxe2A+UX/1OJU6q9K
P1Te6Cn2rj61Q3YeLZ2a390zz3Cc0obZ78AH+BqL8DldUxnVOZorD4zKb1kiYY/2
RQEA6FxvLZuV2yBn7ZOutVaJlMr+pDTBpCcSdQpiJx00xd0/DnOMle9aI/u8f5PL
fdh2gI6GmOJr1VA6lpCCwK8sE8w6TDphT+R2g2uT+Pywh/zlftubfNCU4mu++Q8/
OnDe0y9RbnlSucLpB10bwKR/GFFDTKhzGCjPZ6FDYdRnOOM5j9JLnuL3biFqDC98
eBs0dzGZ7zLmrXt2x8UGRvyEyqHAkN4jQs2SIGpjr5kYXbtczaflhp47dWy5zg==
-----END CERTIFICATE-----
)EOF";
// 사물 인증서 프라이빗 키 (파일 이름: xxxxxxxxxx-private.pem.key)
const char key_str[] PROGMEM = R"EOF(
-----BEGIN RSA PRIVATE KEY-----
hcUclxFZxOfUEwwbojOj2zLhlcFetLXhN4SupnB7e5MBC1rkFZA6HLgAO5obpXJl
NRDE7tNC6wrpEt1l99jYNAU/3w1FVhbYRvTx19W05BPW060AbZNzMBUCgYEA3Alx
6FQW0MtDuytw19g0ysXnxlRFktavcYXgfPKcmMuAb0ektMTkfAsZoS9uGn2T2r4U
b/hixKHsvpuM0Rq9oKR8oc/c59UmC1stdUxBi4kDWATAKSLCKTx2xs7Oxz6PVoMX
7bAtLKL6ppASBRVor/6Cuvs65++sxS7v04bOLbcCgYAUFQAf+eFFz7hZcshE3Tl1
99WP0E58ZBmt4r7Hts3N1w63hZ0zo2WyAhzC/jFceLNb38C3gJQmgyvOCStCRNiL
87MB9xeNt1hogvvHUM9esKyUpPPCl6qgSZbJGODnyFlz7X2j2B2yxqUkajK9s9wo
uMK7174EfCC3UsCvBnFI4QKBgEzuuHbkv71s9FeCzseI/pxPKcR8Pn8itODaUC0f
noUF0fgWco4JsWF1eBaVQMfeVE3M29RyYgYiH2ZdeJS75sU9/Z2N0cz1B4I89N3o
NZqV4ydpMc6wKnbEfzrvn/il9PPojfs69/B9Y2nNQZ+pZpryP78ki+oAMMYYrR2m
KnxtAoGAOBGAlr0dCZcvMownmKMYNBw4J9GxpA/7oRXrnIcyAP53HF6QD0o/TZhf
pZe2ILPjxtOsLHl60YO+aK1sYTY0AZ9Tvh07BMr8fxV/oxTBsCVWJG9MN+x8KFXr
Se8MqvZ8+bMwzTU90y04vg7ONca2z8RtcVWOsibGhdzuKXz4eCw=
-----END RSA PRIVATE KEY-----
)EOF";
// Amazon Trust Services(ATS) 엔드포인트 CA 인증서 (서버인증 > "RSA 2048비트 키: Amazon Root CA 1" 다운로드)
const char ca_str[] PROGMEM = R"EOF(
-----BEGIN CERTIFICATE-----
jgSubJrIqg0CAwEAAaNCMEAwDwYDVR0TAQH/BAUwAwEB/zAOBgNVHQ8BAf8EBAMC
AYYwHQYDVR0OBBYEFIQYzIU07LwMlJQuCFmcx7IQTgoIMA0GCSqGSIb3DQEBCwUA
A4IBAQCY8jdaQZChGsV2USggNiMOruYou6r4lK5IpDB/G/wkjUu0yKGX9rbxenDI
U5PMCCjjmCXPI6T53iHTfIUJrU6adTrCC2qJeHZERxhlbI1Bjjt/msv0tadQ1wUs
N+gDS63pYaACbvXy8MWy7Vu33PqUXHeeE6V/Uq2V8viTO96LXFvKWlJbYK8U90vv
o/ufQJVtMVT8QtPHRh8jrdkPSHCa2XV4cdFyQzR1bldZwgJcJmApzyMZFo6IQ6XU
5MsI+yMRQ+hDKXJioaldXgjUkK642M4UwtBV8ob2xJNDd2ZhwLnoQdeXeGADbkpy
rqXRfboQnoZsG4q5WTP468SQvvG5
-----END CERTIFICATE-----
)EOF";
