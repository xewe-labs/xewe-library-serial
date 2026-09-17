# SerialPortConfig and begin

`src/SerialPort/SerialPort.h` — how the port is opened.

## SerialPortConfig

```cpp
struct SerialPortConfig {
    unsigned long baud_rate        = 115200;
    std::size_t   tx_buffer_size   = 2048;
    std::size_t   rx_buffer_size   = 1024;
    uint32_t      startup_delay_ms = 1000;  // give USB CDC time to enumerate
    bool          echo             = true;  // echo typed characters back
};
```

| Field | Default | |
|---|---|---|
| `baud_rate` | `115200` | passed to `Serial.begin` |
| `tx_buffer_size` | `2048` | set **before** `Serial.begin`; a table or header can be several hundred bytes in one burst |
| `rx_buffer_size` | `1024` | the hardware buffer, separate from the 255-byte line buffer |
| `startup_delay_ms` | `1000` | a plain `delay()` after opening the port |
| `echo` | `true` | echo each received character back, so a terminal shows what is typed |

## begin

```cpp
void begin(const SerialPortConfig& cfg = {});
```

Stores `echo`, sets the TX and RX buffer sizes, calls `Serial.begin(baud_rate)`, then waits
`startup_delay_ms`.

```cpp
xewe::SerialPort serial;

void setup() {
    serial.begin();                                  // defaults
    serial.begin({.baud_rate = 921600, .echo = false});
}
```

**`begin()` blocks for a second by default.** The delay exists because a USB CDC port (ESP32-C3,
C6 and S3 with `CDCOnBoot`) needs time to enumerate on the host — without it the first lines of
output are printed into a port nobody is listening to yet. Set `startup_delay_ms = 0` when you are
on a real UART and care about boot time.

Buffer sizes must be set before `Serial.begin`, which is why they live here and cannot be changed
afterwards.

## Notes

* `SerialPort` holds no dynamic state beyond its 255-byte line buffer; one instance per physical
  port, usually a global.
* Calling `begin()` twice re-opens the port and waits again.
* This library defines **no** `DEBUG_` flag. The `DEBUG_<Class>` convention in
  [XeWeUtils](https://github.com/xewe-labs/xewe-library-utils/blob/main/doc/debug.md) does not
  apply to anything here.
