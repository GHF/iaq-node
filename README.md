# iaq-node
Indoor air quality (IAQ) data acquisition (DAQ) node running [ESPHome](https://esphome.io/).

This is designed to be an always-on measurement node that publishes data over a home network to a [Home Assistant](https://www.home-assistant.io/) server.

## Hardware
- [TTGO T-Display](https://www.amazon.com/TTGO-T-Display-Bluetooth-Development-Arduino/dp/B07XQ5G279/) ESP32 (Wi-Fi/BLE) dev board
- [Plantower PMS3003](http://www.plantower.com/en/content/?107.html) laser airborne particle counter
- [SVM30-J](https://www.sensirion.com/en/environmental-sensors/multi-gas-humidity-and-temperature-module-svm30/) sensor module (SGP30 VOC gas sensor with SHTC1 temperature/humidity sensor)
- [SCD30](https://www.sensirion.com/en/environmental-sensors/carbon-dioxide-sensors/carbon-dioxide-sensors-co2/) NDIR CO₂ gas sensor

## Software
1. Install ESPHome on development machine
2. Copy and edit `secrets.yaml` from `secrets.yaml.example` template
3. Edit `site.yaml` if necessary to configure node names and IDs
4. `esphome iaq-node.yaml run`
