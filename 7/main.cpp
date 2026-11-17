#include <Arduino.h>
//#include "code.h"

const int LED1 = 11, LED2 = 10, LED3 = 9;
const int LED4 = 8;
const int BUTTON = 2;
const int POT = A0;
const int SERVICE_LED = 13;

void task1() {
    static const int PERIOD1 = 200, PERIOD2 = 750, PERIOD3 = 1300;
    static unsigned long last1 = 0, last2 = 0, last3 = 0;
    static bool state1 = false, state2 = false, state3 = false;
    unsigned long curMills = millis();
    if (curMills - last1 >= PERIOD1) {
        last1 = curMills;
        state1 = !state1;
        digitalWrite(LED1, state1);
    }
    if (curMills - last2 >= PERIOD2) {
        last2 = curMills;
        state2 = !state2;
        digitalWrite(LED2, state2);
    }
    if (curMills - last3 >= PERIOD3) {
        last3 = curMills;
        state3 = !state3;
        digitalWrite(LED3, state3);
    }
}
void task2() {
    static unsigned long ts1 = 0, ts2 = 0;
    static unsigned long count1 = 0, count2 = 0;
    static unsigned long lastNoise = 0;
    static bool init = false;

    unsigned long now = millis();
    if (!init) { ts1 = now; ts2 = now; init = true; }

    if (now - ts1 >= 100) {
        ts1 = now;
        count1++;
    }

    while (now - ts2 >= 100) {
        ts2 += 100;
        count2++;
    }

    if (now - lastNoise >= 1000) {
        lastNoise = now;
        delay(37);
    }

    static unsigned long lastPrint2 = 0;
    if (now - lastPrint2 >= 60000) {
        lastPrint2 = now;
        Serial.print("ts=millis: ");
        Serial.print(count1);
        Serial.print("  ts+=100: ");
        Serial.println(count2);
    }
}

enum StopwatchState { STOPPED, RUNNING, PAUSED };

void task3() {
    static StopwatchState state = STOPPED;
    static unsigned long startTime = 0, accumulated = 0, lastPrint = 0, lastBlink = 0;
    static bool blinkState = false;
    static bool lastBtn = HIGH;
    static unsigned long btnChange = 0, pressStart = 0;
    static bool pressing = false;

    unsigned long now = millis();
    bool btn = digitalRead(BUTTON);

    if (btn != lastBtn) { btnChange = now; lastBtn = btn; }

    if (now - btnChange > 30) {
        if (btn == LOW && !pressing) {
            pressing = true;
            pressStart = now;
        } else if (btn == HIGH && pressing) {
            pressing = false;
            unsigned long held = now - pressStart;
            if (held < 1000) {
                if (state == STOPPED) {
                    state = RUNNING;
                    startTime = now;
                    accumulated = 0;
                } else if (state == RUNNING) {
                    state = PAUSED;
                    accumulated += now - startTime;
                } else {
                    state = RUNNING;
                    startTime = now;
                }
            } else {
                state = STOPPED;
                accumulated = 0;
            }
        }
    }

    unsigned long elapsed = accumulated;
    if (state == RUNNING) elapsed += now - startTime;

    if (now - lastPrint >= 100) {
        lastPrint = now;
        unsigned long ms = elapsed % 1000;
        unsigned long s = (elapsed / 1000) % 60;
        unsigned long m = elapsed / 60000;
        if (m < 10) Serial.print('0');
        Serial.print(m); Serial.print(':');
        if (s < 10) Serial.print('0');
        Serial.print(s); Serial.print('.');
        if (ms < 100) Serial.print('0');
        if (ms < 10) Serial.print('0');
        Serial.println(ms);
    }

    if (state == RUNNING) {
        digitalWrite(LED4, HIGH);
    } else if (state == PAUSED) {
        if (now - lastBlink >= 250) { lastBlink = now; blinkState = !blinkState; digitalWrite(LED4, blinkState); }
    } else {
        digitalWrite(LED4, LOW);
    }
}

enum TrafficState { IDLE, CAR_GREEN_BLINK, CAR_YELLOW, CAR_RED, PED_GREEN, PED_BLINK };

void task4() {
    static TrafficState state = IDLE;
    static unsigned long stateStart = 0, lastBlink = 0;
    static bool blinkState = false;
    static bool lastBtn = HIGH;
    static unsigned long btnChange = 0;
    static int blinkCount = 0;

    unsigned long now = millis();
    bool btn = digitalRead(BUTTON);

    if (btn != lastBtn) { btnChange = now; lastBtn = btn; }
    bool pressed = (now - btnChange > 30) && (btn == LOW);

    // служебный светодиод 250 мс
    static unsigned long lastSvc = 0;
    static bool svc = false;
    if (now - lastSvc >= 250) { lastSvc = now; svc = !svc; digitalWrite(SERVICE_LED, svc); }

    switch (state) {
        case IDLE:
            digitalWrite(11, HIGH); digitalWrite(10, LOW); digitalWrite(9, LOW);
            digitalWrite(8, LOW); digitalWrite(7, HIGH);
            if (pressed) { state = CAR_GREEN_BLINK; stateStart = now; blinkCount = 0; lastBlink = now; }
            break;

        case CAR_GREEN_BLINK:
            if (now - lastBlink >= 300) {
                lastBlink = now;
                blinkState = !blinkState;
                digitalWrite(11, blinkState);
                if (!blinkState) blinkCount++;
                if (blinkCount >= 3) { state = CAR_YELLOW; stateStart = now; }
            }
            break;

        case CAR_YELLOW:
            digitalWrite(11, LOW); digitalWrite(10, HIGH);
            if (now - stateStart >= 2000) { state = CAR_RED; stateStart = now; }
            break;

        case CAR_RED:
            digitalWrite(10, LOW); digitalWrite(9, HIGH);
            digitalWrite(8, HIGH); digitalWrite(7, LOW);
            if (now - stateStart >= 5000) { state = PED_GREEN; stateStart = now; }
            break;

        case PED_GREEN:
            if (now - stateStart >= 5000) { state = PED_BLINK; stateStart = now; lastBlink = now; blinkState = true; }
            break;

        case PED_BLINK:
            if (now - lastBlink >= 300) {
                lastBlink = now;
                blinkState = !blinkState;
                digitalWrite(8, blinkState);
                if (now - stateStart >= 3000) { state = IDLE; stateStart = now; }
            }
            break;
    }
}

struct Task {
    void (*handler)();
    unsigned long period;
    unsigned long lastRun;
    bool enabled;
    unsigned long runCount;
    unsigned long maxDuration;
    bool oneShot;
    unsigned long delayMs;
};

void blinkTask() { static bool s = false; s = !s; digitalWrite(LED1, s); }
void potTask() { Serial.print("pot="); Serial.println(analogRead(POT)); }
void btnTask() { digitalWrite(LED4, digitalRead(BUTTON) == LOW ? HIGH : LOW); }
void svcTask() { static bool s = false; s = !s; digitalWrite(SERVICE_LED, s); }

Task tasks[] = {
    { blinkTask, 200, 0, true, 0, 0, false, 0 },
    { potTask, 2000, 0, true, 0, 0, false, 0 },
    { btnTask, 10, 0, true, 0, 0, false, 0 },
    { svcTask, 500, 0, true, 0, 0, false, 0 }
};
const int TASK_COUNT = 4;

void runScheduler() {
    unsigned long now = millis();
    for (int i = 0; i < TASK_COUNT; i++) {
        if (!tasks[i].enabled) continue;
        if (now - tasks[i].lastRun >= tasks[i].period) {
            unsigned long t0 = micros();
            tasks[i].handler();
            unsigned long dt = micros() - t0;
            tasks[i].lastRun = now;
            tasks[i].runCount++;
            if (dt > tasks[i].maxDuration) tasks[i].maxDuration = dt;
            if (tasks[i].oneShot) tasks[i].enabled = false;
        }
    }
}

void task5() {
    runScheduler();

    static bool lastBtn = HIGH;
    static unsigned long btnChange = 0;
    unsigned long now = millis();
    bool btn = digitalRead(BUTTON);
    if (btn != lastBtn) { btnChange = now; lastBtn = btn; }
    if (now - btnChange > 30 && btn == LOW) {
        tasks[3].enabled = !tasks[3].enabled;
        Serial.print("svcTask enabled=");
        Serial.println(tasks[3].enabled);
    }
}

#define DHTPIN 3
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

void task6() {
    static unsigned long lastRead = 0, lastBlink = 0;
    static bool blinkState = false;
    static int failCount = 0;
    static unsigned long interval = 2000;
    static bool alarm = false;

    unsigned long now = millis();

    if (now - lastBlink >= 100) { lastBlink = now; blinkState = !blinkState; digitalWrite(LED1, blinkState); }
    digitalWrite(LED4, digitalRead(BUTTON) == LOW ? HIGH : LOW);

    if (now - lastRead >= interval) {
        lastRead = now;
        unsigned long t0 = micros();
        float t = dht.readTemperature();
        unsigned long dt = micros() - t0;

        Serial.print("dht read took ");
        Serial.print(dt);
        Serial.println(" us");

        if (isnan(t)) {
            failCount++;
            if (failCount >= 3) { alarm = true; interval = 5000; }
        } else {
            failCount = 0;
            alarm = false;
            interval = 2000;
            Serial.print("T=");
            Serial.println(t, 1);
        }
    }

    if (alarm) {
        static unsigned long lastAlarmBlink = 0;
        static bool ab = false;
        if (now - lastAlarmBlink >= 100) { lastAlarmBlink = now; ab = !ab; digitalWrite(LED2, ab); }
    }
}
void setup() {
    Serial.begin(9600);
    pinMode(LED1, OUTPUT);
    pinMode(LED2, OUTPUT);
    pinMode(LED3, OUTPUT);
    pinMode(LED4, OUTPUT);
    pinMode(SERVICE_LED, OUTPUT);
    pinMode(BUTTON, INPUT_PULLUP);
    dht.begin();
}

void loop() {
    // Раскоментировать нужную задачу
    // task1();
    // task2();
    // task3();
    // task4();
    // task5();
    // task6();
}