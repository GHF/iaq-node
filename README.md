# iaq-node
Indoor air quality (IAQ) data acquisition (DAQ) node running [ESPHome](https://esphome.io/).

This is designed to be an always-on measurement node that publishes data over a home network to a [Home Assistant](https://www.home-assistant.io/) server.

## Hardware
- [TTGO T-Display](http://www.lilygo.cn/prod_view.aspx?Id=1126) ESP32 (Wi-Fi/BLE) dev board
- [Plantower PMS3003](https://www.plantower.com/en/products_33/73.html) laser airborne particle counter
- [SVM30-J](https://sensirion.com/products/catalog/SVM30/) sensor module (SGP30 VOC gas sensor with SHTC1 temperature/humidity sensor)
- [SCD30](https://sensirion.com/products/catalog/SCD30/) NDIR CO₂ gas sensor

## Software
1. Install ESPHome on development machine
2. Copy and edit `secrets.yaml` from `secrets.yaml.example` template
3. Edit `site.yaml` if necessary to configure node names and IDs
4. `esphome iaq-node.yaml run`
