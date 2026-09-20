# XeWeSerial

> Full reference: [`doc/`](doc/) · Agent rules: [`doc/AGENTS.md`](doc/AGENTS.md)

Non-blocking serial console.

Supported cores: ESP32, ESP8266, RP2040, Renesas (Uno R4) and the Arduino mbed cores —
anything whose toolchain provides C++17 and a C++ standard library. 8-bit AVR (Uno R3, Nano,
Nano Every) is **not** supported: avr-gcc ships no `<string>`, `<vector>` or `<string_view>`.
Only ESP32-C3/C6/S3 are compile-verified on hardware; the rest are verified at the language
level by the host portability check in `publish-arduino-library`.

```cpp
#include <XeWeSerial.h>

xewe::SerialPort serial;

void setup() {
    serial.begin();                                    // xewe::SerialPortConfig{baud_rate, buffers, echo, ...}
    serial.print_header("Hello");
    if (serial.get_yn("Continue?")) { /* ... */ }
}

void loop() {
    serial.loop();                                     // assemble lines without blocking
    if (serial.has_line()) serial.print("> " + serial.read_line());
}
```

* **Output:** `print` (with alignment, wrapping, box edges), `printf`, `print_header`,
  `print_separator`, `print_table` / `render_table`.
* **Prompts** (block until answered, with optional retries/timeout/default):
  `get_string`, `get_int`, `get_uint8/16/32`, `get_float`, `get_yn`, `get_menu_choice`.
* **Line input:** `loop`, `has_line`, `read_line`, `clear_input`.

In `print_header`, a `\sep` token inside the message draws a separator line.

Depends on XeWeUtils. Three examples in [`examples/`](examples/): `01_HelloSerial`,
`02_Prompts`, `03_SetupWizard`.
