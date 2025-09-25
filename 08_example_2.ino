// 08_example_2_fixed.ino
// Arduino pin assignment
#define PIN_LED   9       // PWM 가능 핀
#define PIN_TRIG  12      // sonar sensor TRIGGER
#define PIN_ECHO  13      // sonar sensor ECHO

// configurable parameters
#define SND_VEL 346.0       // sound velocity at 24°C (m/s)
#define INTERVAL 25UL       // sampling interval (msec)
#define PULSE_DURATION 10   // ultra-sound trigger pulse (usec)
#define DIST_MIN 100.0f     // mm
#define DIST_MAX 300.0f     // mm

// timeout (usec) : INTERVAL의 절반
#define TIMEOUT ((INTERVAL / 2UL) * 1000UL)
// duration(us) -> distance(mm): 0.001(ms/us) * 0.5 * 346(m/s) -> 0.173 (mm/us)
#define SCALE (0.001f * 0.5f * SND_VEL)

unsigned long last_sampling_time = 0;   // msec

float USS_measure(int TRIG, int ECHO) {
  // Trigger pulse
  digitalWrite(TRIG, HIGH);
  delayMicroseconds(PULSE_DURATION);
  digitalWrite(TRIG, LOW);

  // 왕복 시간(us)
  unsigned long dur = pulseIn(ECHO, HIGH, TIMEOUT);
  if (dur == 0UL) return 0.0f; // timeout(미검출)
  return (float)dur * SCALE;   // mm
}

// 거리(mm) -> LED PWM(0~255, active-low: 0=가장밝음, 255=꺼짐)
int distanceToPWM(float d_mm) {
  if (d_mm <= 0.0f) return 255;                 // 미검출
  if (d_mm < DIST_MIN || d_mm > DIST_MAX) return 255; // 범위 밖 -> 꺼짐

  // 삼각형 프로파일: 200mm에서 최대 밝기(=PWM 0)
  const float CENTER = 200.0f;
  float deviation = fabsf(d_mm - CENTER);  // 0 ~ 100
  // deviation=0 -> 0, deviation=100 -> 255
  float pwm_f = (deviation / 100.0f) * 255.0f;

  // 150/250mm에서 duty 50% ≈ 127.5 달성
  if (pwm_f < 0.0f) pwm_f = 0.0f;
  if (pwm_f > 255.0f) pwm_f = 255.0f;
  return (int)(pwm_f + 0.5f);  // 반올림
}

void setup() {
  pinMode(PIN_LED, OUTPUT);
  pinMode(PIN_TRIG, OUTPUT);
  pinMode(PIN_ECHO, INPUT);
  digitalWrite(PIN_TRIG, LOW);   // Sonar off

  Serial.begin(57600);
  last_sampling_time = millis(); // 타이머 기준선 초기화
}

void loop() {
  // 25ms 샘플링 (논블로킹)
  unsigned long now = millis();
  if ((now - last_sampling_time) < INTERVAL) return;
  last_sampling_time += INTERVAL;  // 주기 고정(드리프트 최소화)

  // 거리 측정
  float distance = USS_measure(PIN_TRIG, PIN_ECHO);

  // 밝기 계산 및 적용
  int pwm = distanceToPWM(distance);
  analogWrite(PIN_LED, pwm);   // active-low: 0=밝음, 255=꺼짐

  // 모니터링 출력
  Serial.print("Min:");       Serial.print(DIST_MIN);
  Serial.print(",distance:"); Serial.print(distance, 1);
  Serial.print(" mm, Max:");  Serial.print(DIST_MAX);
  Serial.print(", PWM:");     Serial.println(pwm);
}
