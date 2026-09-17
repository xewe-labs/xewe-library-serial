# XeWeSerial

> Full reference: [`doc/`](doc/) · Agent rules: [`doc/AGENTS.md`](doc/AGENTS.md)

Non-blocking serial console for ESP32.

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

Depends on XeWeUtils. See `examples/Prompts`.
