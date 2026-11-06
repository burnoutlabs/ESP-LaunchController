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

## Licence

MIT; see [`LICENSE`](LICENSE). `src/main.cpp` started from an ESPUI example;
its notice is in [`THIRD_PARTY_NOTICES.md`](THIRD_PARTY_NOTICES.md).
