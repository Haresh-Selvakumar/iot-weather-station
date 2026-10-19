# IoT Weather Station

ESP32 and DHT22 temperature/humidity monitoring with ThingSpeak telemetry and MATLAB email alerts. Created by Haresh Selvakumar.

## Files

- `sketch.ino`: sensor reading, validity checks, Wi-Fi connection, and cloud upload.
- `diagram.json`, `libraries.txt`, `Velxio_Import.zip`: supplied simulator configuration.
- `weather_alert.m`: ThingSpeak MATLAB Analysis email alert.
- `cloud_commissioning.m`: controlled cloud test values, distinct from sensor measurements.
- `Screenshots/` and `Project_Report.pdf`: submitted evidence and report.

## Setup

1. Use an ESP32 with a DHT22 data pin on GPIO 15; consult `diagram.json` for the supplied simulator wiring.
2. Install the dependencies listed in `libraries.txt` and the Arduino ESP32 core.
3. Configure Wi-Fi in `connectWiFi()`. The supplied sketch uses the simulator network `Velxio-GUEST`.
4. Create your own ThingSpeak channel with field 1 for temperature in Celsius, field 2 for relative humidity in percent, and field 3 for the alarm flag.
5. Set the write key in your private working copy. Replace the example channel ID and read/alert key placeholders in the MATLAB scripts with your own configuration.
6. Configure ThingSpeak React/MATLAB Analysis to run the alert script when the alarm field is set. Keep private keys out of commits.

The alarm triggers when temperature exceeds 35 C or humidity exceeds 80 percent. Upload cycles are separated by at least 20 seconds after a request completes. Invalid readings are skipped.

## Scope and validation

The supplied sketch uses HTTP for demonstration and is intended for a disposable test channel. Cloud commissioning writes are synthetic tests, not physical sensor readings. Submitted screenshots record prior demonstrations; hardware, simulator, and live ThingSpeak execution have not been rerun during repository preparation.
