# iaq-node
Indoor air quality (IAQ) data acquisition (DAQ) node running [ESPHome](https://esphome.io/).

![Animated image of the physical node, with sensors and a display. The display shows the temperature and humidity. A USB C cable extends from the display and a prismatic lithium battery is visible. A thumb comes into view and presses on a tactile button on the display unit to change the display contents to particulate density, then again to gas concentrations.](doc/iaq_demo.webp)

This is the software for an always-on measurement node that publishes data over a home network to a [Home Assistant](https://www.home-assistant.io/) server.

## Hardware
- [TTGO T-Display](http://www.lilygo.cn/prod_view.aspx?Id=1126) ESP32 (Wi-Fi/BLE) dev board
- [Plantower PMS3003](https://www.plantower.com/en/products_33/73.html) laser airborne particle counter
- [SVM30-J](https://sensirion.com/products/catalog/SVM30/) sensor module (SGP30 VOC gas sensor with SHTC1 temperature/humidity sensor)
- [SCD30](https://sensirion.com/products/catalog/SCD30/) NDIR CO₂ gas sensor

## Software
1. Copy and edit `secrets.yaml` from `secrets.yaml.example` template
  - `api_encryption_key` is a [random 32-byte pre-shared key in base64](https://esphome.io/components/api.html)
2. Edit `site.yaml` if necessary to configure node names and IDs
3. `pipenv install`
4. `pipenv run esphome run iaq-node.yaml`
