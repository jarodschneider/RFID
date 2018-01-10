# RFsec

Simple authentication demo based on UID: authorized UID causes LED to light up green, but unauthorized attempt results in a three-strikes rule: after three subsequent unauthorized attempts, an alarm will sound and the LED will flash red. This alarm must be deactivated by reading the authorized card and inputting a numeric passcode into the keypad.

Disclaimer: using a card's UID as authentication is possibly unsecure, as UID is easily changed/emulated. This demo is only meant to serve as my introduction to RFID and should NOT be used in any serious security applications

Dependencies (see libraries directory):
* [Keypad](https://github.com/Chris--A/Keypad)
* [rfid](https://github.com/miguelbalboa/rfid)

![Alt text](RFIDsec_bb.png?raw=true)

![Alt text](RFIDsec_schem.png?raw=true)
