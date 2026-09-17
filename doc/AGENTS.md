# AGENTS.md — xewe-library-serial

Rules for coding agents working **anywhere in this repository**, not only in `doc/`.
Organization-wide rules are in
[`.github/AGENTS.md`](https://github.com/xewe-labs/.github/blob/main/AGENTS.md) and win where this
file is silent: never publish, never tag or push, never commit unasked, never flash a board.

## This library

* **`src/` has exactly one top-level header,** `src/XeWeSerial.h`. Everything else lives in
  `src/SerialPort/`. Arduino puts every library's `src/` on the include path, so a second
  top-level header collides with other libraries.
* **XeWeUtils is included only through its entry header** (`#include <XeWeUtils.h>`), never by
  reaching into its folders. Every library added to an `#include` must be declared in
  `library.properties` → `depends=`.
* **This library must not include `XeWeOS.h`.** It works standalone; XeWe OS depends on it, not
  the other way round.

## Do not break these

* **Prompt semantics are load-bearing.** `retry_count == 0` means infinite and `timeout_ms == 0`
  means no timeout. Modules across the organization call `get_yn()` and `get_string()` with no
  retry arguments and rely on the call not returning until it is answered. Changing either default
  silently changes every first-boot setup flow.
* **`get_core` sets `success_sink` on every exit path.** A new prompt type must go through it, or
  callers lose the only signal that distinguishes a default from an answer.
* **The 255-byte `INPUT_BUFFER_SIZE` is documented behaviour**, including the silent split of a
  longer line. Growing it is fine; changing `get_string`'s `max_length == 0` fallback, which
  resolves to `INPUT_BUFFER_SIZE - 1`, is a behaviour change to announce.
* **Table cells are `std::string_view`.** Do not add an overload that stores them, and do not
  "fix" a caller by passing a temporary — the lifetime rule is the API.
* **Output is CRLF** (`xewe::str::kCRLF`) everywhere. Terminals on the other end assume it.
* **`print` wraps only when `message_width > 0`.** Callers pass `0` constantly; making wrapping
  unconditional reflows every boot message in every firmware.
* This library defines **no** `DEBUG_` flag. If you add one, default it to `0` behind
  `#ifndef DEBUG_<Class>` and document it.

## When changing this library

* `library.json` is **generated** from `library.properties`
  (`python3 publish.py manifest` in
  [`publish-arduino-library`](https://github.com/xewe-labs/publish-arduino-library)). Never
  hand-edit it; `check` fails when it is stale.
* Versions are **lockstep** across all XeWe libraries. Never bump this one alone.
* Source files start with the SPDX header from
  [`.github/guidelines/license-header.txt`](https://github.com/xewe-labs/.github/blob/main/guidelines/license-header.txt).
  Markdown files do not.
* **Documentation is part of the change.** A new or changed public function updates its page in
  `doc/` in the same breath — this reference is written to be exhaustive, so a gap is a bug.
* Check your work without publishing anything:

  ```bash
  python3 publish-arduino-library/publish.py check xewe-library-serial
  ```
