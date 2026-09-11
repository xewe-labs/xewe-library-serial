// XeWeSerial: formatted output and typed input prompts
#include <XeWeSerial.h>

xewe::SerialPort serial;

void setup() {
    serial.begin();   // 115200 baud; see xewe::SerialPortConfig for options

    serial.print_header("XeWeSerial Demo");

    std::string name = serial.get_string("What is your name?", 1, 32);
    int         age  = serial.get_int("How old are you?", 0, 150);

    if (serial.get_yn("Print a summary?")) {
        std::string age_str = std::to_string(age);
        serial.print_table({
            {"Field", "Value"},
            {"Name",  name},
            {"Age",   age_str},
        }, "Summary");
    }

    serial.print("Now echoing lines you type:");
}

void loop() {
    serial.loop();   // non-blocking: assembles lines from the RX buffer

    if (serial.has_line()) {
        serial.print("> " + serial.read_line());
    }
}
