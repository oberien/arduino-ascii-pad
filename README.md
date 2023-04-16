# ASCII-PAD

### Build
If needed, install board:
```
arduino-cli config init
arduino-cli config add board_manager.additional_urls "https://github.com/earlephilhower/arduino-pico/releases/download/global/package_rp2040_index.json"
arduino-cli core update-index
arduino-cli core install rp2040:rp2040
```

Build:
```sh
arduino-cli compile --fqbn rp2040:rp2040:seeed_xiao_rp2040
```

Flash:
```sh
# may require sudo when executing for the first time
arduino-cli upload -vtp /dev/ttyACMX --fqbn rp2040:rp2040:seeed_xiao_rp2040
```

Build & Flash:
```sh
arduino-cli compile --upload -tp /dev/ttyACMX --fqbn rp2040:rp2040:seeed_xiao_rp2040
```

