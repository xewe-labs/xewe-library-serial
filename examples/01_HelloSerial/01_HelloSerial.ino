// XeWeSerial (low): opening the port, printing, and reading lines without blocking.
#include <XeWeSerial.h>

xewe::SerialPort serial;

void setup() {
    // Defaults are 115200 baud, 2048/1024 byte buffers, a 1000 ms startup delay
    // (USB CDC needs time to enumerate) and echo on. Override what you need:
    serial.begin({.baud_rate = 115200, .echo = true});

    serial.print_header("XeWeSerial");          // +---+ | title | +---+
    serial.print("Type something and press enter.");
    serial.print_separator();                   // +--------------------+
    serial.print_spacer(50, "|");               // |                    |

    serial.printf("up %lu ms", millis());       // printf appends CRLF for you

    // Wrapping and alignment only happen when message_width > 0.
    serial.print("this long sentence is wrapped and centred inside a box",
                 xewe::str::kCRLF, "|", 'c', 'w', 46, 1, 1);

    serial.print_separator();
}

void loop() {
    serial.loop();                              // drains the port, assembles lines; never blocks

    if (serial.has_line()) {
        const std::string line = serial.read_line();
        serial.printf("you said: %s", line.c_str());
    }
}
