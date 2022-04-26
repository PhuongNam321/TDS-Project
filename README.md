# TDS-Project

Dự án: Thiết kế một bút đo chỉ số EC/TDS (chỉ số chất lượng nước) có khả năng kết nối Internet cho phép cấu hình/đọc số liệu từ xa hoặc các tính năng mở rộng khác như cảnh báo ngưỡng, cập nhật tự động số liệu theo chu kỳ lấy mẫu lên máy chủ.

Sensor sử dụng trong dự lần này:
+ DFRobot Gravity: Analog TDS Sensor/Meter for Arduino
+ Ds18b20
+ Nodemcu esp8266
+ Led đơn

Web Sever : BlynkCloud

Ý tưởng: Sử dụng các sensor TDS và Ds18b20 2 cảm biến này sẽ cho chúng ta biết được các chỉ số TDS và nhiệt độ tuy nhiên chúng ta phải có phương trình xử lý lại 2 số liệu vì nó vẫn đang ở dạng Analog và Digital. Sử dụng board mạch Nodemcu esp8266 để lấy dự liệu từ 2 cảm biến tính toán lại và đưa lên sever Blynk.

