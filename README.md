# ESP-LaunchController
A simple model rocket launch controller based on ESP32.

## Archived: not safe to use

This project was abandoned after two days of work (2024-10-01 to 2024-10-02)
and is archived. **It is not a launch controller. Do not connect it to a live
igniter.** It is an ESPUI demo sketch with launch-themed controls, and it has
these known hazards and defects (as of commit `b65cdae`):

- **Open Wi-Fi access point with no password.** If the stored Wi-Fi
  credentials fail, `connectWifi()` starts an access point with
  `WiFi.softAP(HOSTNAME)` and no password, so anyone in radio range can reach
  the web UI, including the "Launch" button.
- **No ignition circuit, continuity check or interlocks.** The "Power"
  switcher and "Launch" button only print their id and value to serial
  (`generalCallback`); no GPIO is driven. There is no arming key, continuity
  sense, countdown, hold-to-fire or failsafe on Wi-Fi or UI disconnect, and the
  "Status" label is a fixed string that never changes. Any fire circuit added
  to this code would inherit none of those protections.
- **`while (!Serial)` in `setup()`.** On a native-USB board the controller
  waits for a serial monitor and will not boot in the field without one.
- **EEPROM overrun.** `EEPROM.begin(100)` reserves 100 bytes, but the Save
  handler writes the password up to address 126 and
  `readStringFromEEPROM(stored_pass, 32, 96)` reads up to address 127.
- **Wi-Fi password printed to serial.** The Save handler prints the SSID and
  the password in clear text.

Other leftovers from the demo and from an unrelated project (`src/rlht.*`, the
"Attitude Control" pad, the unused periodic-update block) remain in the code.
None of this has been fixed, and the firmware has not been rebuilt since it was
archived.

Anyone building a real launch controller should start from written safety
requirements (a physical arm key that cuts fire power independently of the
microcontroller, separate igniter power, a low-current continuity check, fire
only while armed and held) rather than from this code.

## Licence

MIT; see [`LICENSE`](LICENSE). `src/main.cpp` is derived from an ESPUI example;
its origin and the ESPUI MIT notice are in
[`THIRD_PARTY_NOTICES.md`](THIRD_PARTY_NOTICES.md).
