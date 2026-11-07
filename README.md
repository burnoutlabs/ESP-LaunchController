# ESP-LaunchController

A simple model rocket launch controller based on ESP32: a phone or laptop
connects over Wi-Fi to a web UI ([ESPUI](https://github.com/s00500/ESPUI))
served by the board.

## Status

Work in progress. The web UI and Wi-Fi setup are in place; the launch controls
are not wired to outputs yet, so the board doesn't drive an igniter. Open
items are tracked in [issues](../../issues).

## Build

PlatformIO, for a SparkFun ESP32-S2 Thing Plus C:

```
pio run -t upload
pio device monitor
```

## Wi-Fi setup

On boot the board joins the network saved in its EEPROM. If that fails, it
starts its own access point named `ESP-LaunchController` at `192.168.1.1`;
connect to it and use the **WiFi Credentials** tab to save a network.

The access point is WPA2-protected. By default each board generates its own
random password the first time it needs one, keeps it in EEPROM, and prints it
to the serial monitor (`pio device monitor`) every time it starts the access
point. To use a fixed password instead, set it at build time; nothing is stored
in the repository:

```
PLATFORMIO_BUILD_FLAGS='-DAP_PASSWORD=\"your-password\"' pio run -t upload
```

It must be 8 to 63 characters, or the build fails. The board never prints the
password of the network it joins.

## Licence

MIT; see [`LICENSE`](LICENSE). `src/main.cpp` started from an ESPUI example;
its notice is in [`THIRD_PARTY_NOTICES.md`](THIRD_PARTY_NOTICES.md).
