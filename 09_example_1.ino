// 예시: 범위 기반 필터링
// dist_raw: 센서에서 읽은 거리
// dist: 실제로 사용할 보정된 거리
// dist_prev: 직전 측정값 (전역 또는 static 변수로 관리)

int dist_raw = analogRead(sensorPin);  // 예시: 센서 값 읽기
int dist; 
static int dist_prev = 0;  // 프로그램 실행 중 유지되도록 static 사용

// 허용 범위 (예: 10 ~ 400cm)
int minRange = 10;
int maxRange = 400;

if (dist_raw >= minRange && dist_raw <= maxRange) {
    // 범위 안에 들어오면 정상 값 사용
    dist = dist_raw;
} else {
    // 범위를 벗어나면 직전 값 사용
    dist = dist_prev;
}

// 현재 값을 직전 값으로 갱신
dist_prev = dist;
