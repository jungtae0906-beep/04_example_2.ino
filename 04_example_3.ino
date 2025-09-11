#define PIN_LED 13
unsigned int count, toggle;

void setup() {
  pinMode(PIN_LED, OUTPUT);         // LED 핀을 출력으로 설정
  Serial.begin(115200);             // 시리얼 포트 초기화
  Serial.println("Hello World!");   // TX LED 깜빡임
  count = 0;
  toggle = 0;
  digitalWrite(PIN_LED, toggle);    // LED 초기 상태 OFF
}

void loop() {
  Serial.print("Count: ");
  Serial.println(++count);          // 카운트 출력 → TX LED 깜빡임

  toggle = toggle_state(toggle);    // LED 상태 반전
  digitalWrite(PIN_LED, toggle);    // LED 상태 갱신
  delay(1000);                      // 1초 대기
}

int toggle_state(int toggle) {
  return !toggle;   // 현재 상태를 반전시킴
}
