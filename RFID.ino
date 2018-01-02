#include <SPI.h>
#include <MFRC522.h>

MFRC522 rfid(10, 9); // create MFRC522

#define R 3 // RGB LED pins (PWM-capable)
#define G 5
#define B 6

void setup()
{
	Serial.begin(9600); // optional Serial comms
	SPI.begin();
	rfid.PCD_Init(); // init MFRC522

	pinMode(R, OUTPUT);
	pinMode(G, OUTPUT);
	pinMode(B, OUTPUT);
}

void loop()
{
	if (!rfid.PICC_IsNewCardPresent() || !rfid.PICC_ReadCardSerial()) { // only continue if card detected
		delay(50);
		return;
	}

	String cand; // candidate UID
	String match = "163225117233"; // CHANGE TO YOUR CARD'S UID

	for (byte i = 0; i < rfid.uid.size; i++) {
		cand += rfid.uid.uidByte[i];
	}

	if (cand == match) {
		digitalWrite(R, LOW);
		analogWrite(G, 60);

		delay(500);
		LEDoff();
	}
	else {
		analogWrite(R, 60);
		digitalWrite(G, LOW);

		delay(500);
		LEDoff();
	}
}

void LEDoff() {
	digitalWrite(R, LOW);
	digitalWrite(G, LOW);
	digitalWrite(B, LOW);
}
