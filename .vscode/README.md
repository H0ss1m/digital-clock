# VS Code Configuration

| File | Functionality |
|---|---|
| `extensions.json` | Recommends PlatformIO IDE and avoids two unwanted PlatformIO extension alternatives |
| `c_cpp_properties.json` | Supplies include paths, AVR defines, compiler path, C standard, and MCU flags for IntelliSense |
| `launch.json` | Provides PlatformIO debug configurations using the generated `firmware.elf` |

Run `pio run` first when debugging so `.pio/build/ATmega328P/firmware.elf` and AVR configuration are generated.
