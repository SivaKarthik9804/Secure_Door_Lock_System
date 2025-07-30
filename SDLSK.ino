#include <Keypad.h>
#include <LiquidCrystal.h>
#include <Servo.h>

#define Password_Length 5 // 4 digits + null

Servo myservo;
LiquidCrystal lcd(A0, A1, A2, A3, A4, A5);

int pos = 0;
const int buzzer = 8;

char Data[Password_Length];
char Master[Password_Length] = "1234";
byte data_count = 0;

bool door = false;
char customKey;

const byte ROWS = 4;
const byte COLS = 4;
char keys[ROWS][COLS] = {
{'1', '2', '3', 'A'},
{'4', '5', '6', 'B'},
{'7', '8', '9', 'C'},
{'*', '0', '#', 'D'}
};
byte rowPins[ROWS] = {0, 1, 2, 3};
byte colPins[COLS] = {4, 5, 6, 7};

Keypad customKeypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

void setup() {
pinMode(buzzer, OUTPUT);
myservo.attach(9, 2000, 2400);
ServoClose();
lcd.begin(16, 2);
lcd.print("Welcome!");
delay(2000);
lcd.clear();
lcd.print("Enter Password: *"); // Static * as a placeholder
}

void loop() {
if (door) {
customKey = customKeypad.getKey();
if (customKey == '#') {
ServoClose();
lcd.clear();
lcd.print("Locked");
delay(2000);
lcd.clear();
lcd.print("Welcome!");
delay(2000);
lcd.clear();
lcd.print("Enter Password: *");
door = false;
}
} else {
customKey = customKeypad.getKey();
if (customKey) {
Data[data_count] = customKey;
lcd.setCursor(data_count, 1);
lcd.print(Data[data_count]);
data_count++;
}

if (data_count == Password_Length - 1) { // 4 digits entered
if (!strcmp(Data, Master)) {
// Correct password
lcd.clear();
tone(buzzer, 1000, 300);
ServoOpen();
lcd.print("Door is Open");
delay(5000); // Door open for 5 sec

// 5-sec countdown to close
unsigned long startTime = millis();
while (millis() - startTime < 5000) {
lcd.setCursor(0, 1);
lcd.print("Closing in: ");
lcd.print((5000 - (millis() - startTime)) / 1000);
lcd.print("s ");
delay(100);
}
ServoClose();
lcd.clear();
lcd.print("Locked");
delay(2000);
lcd.clear();
lcd.print("Welcome!");
delay(2000);
lcd.clear();
lcd.print("Enter Password: *");
door = false;
} else {
// Wrong password
lcd.clear();
tone(buzzer, 500, 2000);
lcd.print("Wrong Password");
delay(500); // Delay before second line
lcd.setCursor(0, 1);
lcd.print("Alert sent to Boss");
delay(1500); // Remaining time for both messages
lcd.clear();
lcd.print("Enter Password: *");
}
clearData();
}
}
}

void clearData() {
while (data_count != 0) {
Data[data_count--] = 0;
}
}

void ServoClose() {
for (pos = 90; pos >= 0; pos -= 10) {
myservo.write(pos);
delay(15);
}
}

void ServoOpen() {
for (pos = 0; pos <= 90; pos += 10) {
myservo.write(pos);
delay(15);
}
}
