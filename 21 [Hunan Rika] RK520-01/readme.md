# RK520-01 배선도
![RK520배선도](https://user-images.githubusercontent.com/37902752/155929504-8da248a2-d38b-4d6f-87c9-2ed04c915835.png)

### 데이터값 계산하기
* Example

|응답 예시|프로토콜|
|----|----------|
|Example respond|01 03 04 01 23 01 64 0A7E|

4,5번째 바이트: 01 23  
6,7번째 바이트: 01 64  
* Temperature = 0x0123 / 10 = 291 / 10 = 29.1      
* Humidity = 0x0164 / 10 = 356 / 10 = 35.6 