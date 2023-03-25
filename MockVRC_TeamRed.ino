#include <PS2X_lib.h> // thư viện tay cầm PS2
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h> // thư viện chip điều khiển động cơ

PS2X ps2; // tay cầm PS2
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(); // chip ĐK động cơ

#define MAX_SPD  2458
#define MIN_SPD  820
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200); // khởi tạo truyền dữ liệu với máy tính qua serial console (115200 baud)
  
  while(ps2.config_gamepad(13,11,10,12) != 0) { // khởi tạo tay cầm
    Serial.println("PS2 error");
    delay(500);
  }
  /* khởi tạo chip ĐK động cơ */
  pwm.begin();
  pwm.setOscillatorFrequency(27000000);
  pwm.setPWMFreq(50);
  Wire.setClock(400000);
  
  Serial.println("OK");
}

void dkhien_dc(int dc, int sp) {
  if( sp >= 0) {
    pwm.setPWM(dc*2, 0, sp);
    pwm.setPWM(dc * 2 +1, 0, 0);
  }
  else {
    pwm.setPWM(dc*2, 0, 0);
    pwm.setPWM(dc*2 +1, 0, -sp);
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  ps2.read_gamepad(false,false); // đọc từ tay cầm

  int left = map(ps2.Analog(PSS_LY), 0, 255, 4095, -4095); // lấy giá trị trục Y joystick trái và map từ 0..255 -> 4095..-4095
  int right = map(ps2.Analog(PSS_RY), 0, 255, -4095, 4095);

  dkhien_dc(0, left);
  dkhien_dc(3, right); 
}
