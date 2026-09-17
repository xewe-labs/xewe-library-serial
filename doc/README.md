# XeWeSerial documentation

Complete reference for `xewe::SerialPort`. The [README](../README.md) is the short version: what
the library is and a minimal sketch.

**[AGENTS.md](AGENTS.md) — read this first if you are a coding agent.** It applies to the whole
repository, not just this folder.

| Page | Covers |
|---|---|
| [config.md](config.md) | `SerialPortConfig` fields and defaults, `begin()` |
| [output.md](output.md) | `print`, `printf`, `printf_fmt`, `print_separator`, `print_spacer`, `print_header`, `print_table`, `render_table` |
| [prompts.md](prompts.md) | `get_string`, `get_int`, `get_uint8/16/32`, `get_float`, `get_yn`, `get_menu_choice`, and the shared retry/timeout/default/`success_sink` parameters |
| [input.md](input.md) | `loop`, `has_line`, `read_line`, `clear_input` and the line buffer |

```cpp
#include <XeWeSerial.h>
```

## Things that surprise people

* **`retry_count == 0` means infinite retries**, not zero. `timeout_ms == 0` means wait forever.
  A prompt with both left at their defaults never returns until it is answered.
* `success_sink` is the only way to tell a real answer from a returned default.
* `print` wraps and aligns **only when `message_width > 0`**.
* `print_table` cells are `std::string_view`s that must outlive the call.
* The line buffer is 255 bytes; a longer line is silently split in two.
* `begin()` blocks for `startup_delay_ms` — one second by default.
* Output is always CRLF, and `printf` appends it.

## Dependencies

[XeWeUtils](https://github.com/xewe-labs/xewe-library-utils). Used by
[XeWeCli](https://github.com/xewe-labs/xewe-library-cli) and
[XeWeOS](https://github.com/xewe-labs/xewe-library-os).

See [`examples/Prompts`](../examples/Prompts) for a runnable sketch.
