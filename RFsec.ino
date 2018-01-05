#include <SPI.h>
#include <MFRC522.h>
#include <Keypad.h>

MFRC522 rfid(10, 9); // create MFRC522

#define R 15 // RGB LED pins (PWM-capable)
#define G 16
#define B 17

int alarm = 0; // 3 strikes rule

// setup keypad
char keys[4][4] = {
	{ '1','2','3','A' },
	{ '4','5','6','B' },
	{ '7','8','9','C' },
	{ '*','0','#','D' }
};
byte rowPins[4] = { 14,2,3,4 };
byte colPins[4] = { 5,6,7,8 };
Keypad key = Keypad(makeKeymap(keys), rowPins, colPins, 4, 4);

void setup()
{
	Serial.begin(9600); // optional Serial comms
	SPI.begin();
	rfid.PCD_Init(); // init MFRC522

	pinMode(19, OUTPUT); // buzzer pin

	pinMode(R, OUTPUT);
	pinMode(G, OUTPUT);
	pinMode(B, OUTPUT);
}

void loop()
{
	if (alarm >= 3) {
		Serial.println("ALARM");
		// really annoying buzzer alarm
		for (int i = 0; i<10; i++)
		{
			digitalWrite(19, HIGH);
			delay(50);//wait for 1ms
			digitalWrite(19, LOW);
			delay(2);//wait for 1ms
		}
		digitalWrite(R, HIGH);
		delay(10);
		LEDoff();
		for (int i = 0; i<70; i++)
		{
			digitalWrite(19, HIGH);
			delay(5);//wait for 1ms
			digitalWrite(19, LOW);
			delay(2);//wait for 1ms
		}
		digitalWrite(R, HIGH);
		delay(10);
		LEDoff();
	}

	if (!rfid.PICC_IsNewCardPresent() || !rfid.PICC_ReadCardSerial()) { // only continue if card detected
		delay(50);
		return;
	}

	byte cand[4]; // candidate UID
	byte match[4] = { 0xA3, 0xE1, 0x75, 0xE9 }; // CHANGE TO YOUR CARD'S UID

	for (byte i = 0; i < rfid.uid.size; i++) {
		cand[i] = rfid.uid.uidByte[i];
	}

	if (cand[0] == match[0] && cand[1] == match[1] && cand[2] == match[2] && cand[2] == match[2]) {
		// stuff authorized card triggers

		Serial.println("CORRECT CARD");
		if (alarm >= 3) {
			// begin disarm attempt
			String s = getCode();
			if (s == "1234#") { // PLACE CORRECT CODE HERE
				digitalWrite(G, HIGH);
				delay(200);
				LEDoff();
				delay(200);
				digitalWrite(G, HIGH);
				delay(200);
				LEDoff();
				Serial.println("ALARM OFF");
				alarm = 0;
				delay(200);
			}
			else {
				Serial.println("INCORRECT CODE");
				return;
			}
		}
		else {
			alarm = 0;
		}

		digitalWrite(R, LOW);
		digitalWrite(G, HIGH);

		delay(500);
		LEDoff();
	}
	else {
		// stuff unauthorized card triggers
		if (alarm < 3) {
			digitalWrite(R, HIGH);
			digitalWrite(G, LOW);

			delay(500);
			LEDoff();

			Serial.println("WRONG");
			alarm++;
		}
		else {
			// ALARM
			//digitalWrite(R, HIGH);
			//delay(1000);
			//LEDoff();
			//delay(1000);
			//digitalWrite(R, HIGH);
			//delay(1000);
			//LEDoff();
		}
	}
}

void LEDoff() {
	digitalWrite(R, LOW);
	digitalWrite(G, LOW);
	digitalWrite(B, LOW);
}

String getCode() {
	String s = "";
	Serial.println();
	Serial.print("ENTER CODE TO DISARM:");
	while (s.length() < 5) {
		char customKey = key.getKey();

		if (customKey) {
			s += customKey;
			Serial.print(" ");
			Serial.print(customKey);
		}
	}
	Serial.println();
	return s;
}
