#define enA 13       
#define in1 12        
#define in2 11      
#define in3 10        
#define in4 9     
#define enB 8       
#define L_IR A1      
#define R_IR A2       
#define ECHO_ULTR A2    
#define TRIGGER_ULTR A3   
#define SERVO A5     

int Set = 15;       //пороговое расстояние для обнаружения препятствий
int distance_L, distance_F, distance_R;

void setup() {
    Serial.begin(9600);
    pinMode(R_S, INPUT);         
    pinMode(L_S, INPUT);       
    pinMode(echo, INPUT);      
    pinMode(trigger, OUTPUT);  
    pinMode(enA, OUTPUT);     
    pinMode(in1, OUTPUT);        
    pinMode(in2, OUTPUT);      
    pinMode(in3, OUTPUT);       
    pinMode(in4, OUTPUT);       
    pinMode(enB, OUTPUT);       
    pinMode(servo, OUTPUT);    
    
    analogWrite(enA, 200);       //скорости для мотора
    analogWrite(enB, 200);       
    
    for (int angle = 70; angle <= 140; angle += 5) {
        servoPulse(servo, angle);
    }
    for (int angle = 140; angle >= 0; angle -= 5) {
        servoPulse(servo, angle);
    }
    for (int angle = 0; angle <= 70; angle += 5) {
        servoPulse(servo, angle);
    }
    
    distance_F = Ultrasonic_read(); //чтение расстояния впереди
    delay(500);
}

void loop() {
    distance_F = Ultrasonic_read();
    Serial.print("D F="); Serial.println(distance_F);
    
    if ((digitalRead(R_S) == 0) && (digitalRead(L_S) == 0)) {
        if (distance_F > Set) {
            forward();
        } else {
            Check_side();
        }
    } else if ((digitalRead(R_S) == 1) && (digitalRead(L_S) == 0)) {
        turnRight();
    } else if ((digitalRead(R_S) == 0) && (digitalRead(L_S) == 1)) {
        turnLeft();
    }
    
    delay(10);
}

void servoPulse(int pin, int angle) {
    int pwm = (angle * 11) + 500;
    digitalWrite(pin, HIGH);
    delayMicroseconds(pwm);
    digitalWrite(pin, LOW);
    delay(50);
}

long Ultrasonic_read() {
    digitalWrite(trigger, LOW);
    delayMicroseconds(2);
    digitalWrite(trigger, HIGH);
    delayMicroseconds(10);
    long time = pulseIn(echo, HIGH);
    return time / 29 / 2;
}

void compareDistance() {
    if (distance_L > distance_R) {
        turnLeft();
        delay(500);
        forward();
        delay(600);
        turnRight();
        delay(500);
        forward();
        delay(600);
        turnRight();
        delay(400);
    } else {
        turnRight();
        delay(500);
        forward();
        delay(600);
        turnLeft();
        delay(500);
        forward();
        delay(600);
        turnLeft();
        delay(400);
    }
}

void Check_side() {
    Stop();
    delay(100);
    for (int angle = 70; angle <= 140; angle += 5) {
        servoPulse(servo, angle);
    }
    delay(300);
    distance_R = Ultrasonic_read();
    Serial.print("D R="); Serial.println(distance_R);
    delay(100);
    for (int angle = 140; angle >= 0; angle -= 5) {
        servoPulse(servo, angle);
    }
    delay(500);
    distance_L = Ultrasonic_read();
    Serial.print("D L="); Serial.println(distance_L);
    delay(100);
    for (int angle = 0; angle <= 70; angle += 5) {
        servoPulse(servo, angle);
    }
    delay(300);
    compareDistance();
}

void forward() { //вперед
    digitalWrite(in1, LOW);
    digitalWrite(in2, HIGH);
    digitalWrite(in3, HIGH);
    digitalWrite(in4, LOW);
}

void backward() { //назад
    digitalWrite(in1, HIGH);
    digitalWrite(in2, LOW);
    digitalWrite(in3, LOW);
    digitalWrite(in4, HIGH);
}

void turnRight() { //направо
    digitalWrite(in1, LOW);
    digitalWrite(in2, HIGH);
    digitalWrite(in3, LOW);
    digitalWrite(in4, HIGH);
}

void turnLeft() { //налево
    digitalWrite(in1, HIGH);
    digitalWrite(in2, LOW);
    digitalWrite(in3, HIGH);
    digitalWrite(in4, LOW);
}

void Stop() { //остановка
    digitalWrite(in1, LOW);
    digitalWrite(in2, LOW);
    digitalWrite(in3, LOW);
    digitalWrite(in4, LOW);
}
