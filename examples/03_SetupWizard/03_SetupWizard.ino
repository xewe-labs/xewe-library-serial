// XeWeSerial (high): a guided setup that can give up.
// Shows the three prompt parameters that are easy to get wrong:
//   retry_count == 0  -> INFINITE retries; the call only returns on valid input
//   timeout_ms  == 0  -> no timeout; it waits forever
//   success_sink      -> the only way to tell a real answer from the default
#include <XeWeSerial.h>

xewe::SerialPort serial;

struct Config {
    std::string name;
    uint8_t     mode       = 1;
    uint8_t     brightness = 128;
    float       gamma      = 2.2f;
    bool        defaulted  = false;   // true if any step fell back
};

void setup() {
    serial.begin();
    serial.print_header("Device Setup\\sepanswer, or wait 15 s to accept the default");

    Config cfg;
    bool   ok = false;

    // Blocks until answered: retry_count and timeout_ms both default to 0.
    cfg.name = serial.get_string("Device name?", 1, 32);

    // Menus are 1-based: leaving min/max at their defaults derives them from
    // the option list, so this accepts 1..3.
    cfg.mode = serial.get_menu_choice("Mode?", {"Solid", "Fade", "Rainbow"});

    // 3 attempts, 15 s each, then fall back to 128 and set ok = false.
    cfg.brightness = serial.get_uint8("Brightness 0-255?", 0, 255, 3, 15000, 128, ok);
    if (!ok) {
        cfg.defaulted = true;
        serial.print("No answer; using 128.");
    }

    cfg.gamma = serial.get_float("Gamma 1.0-3.0?", 1.0f, 3.0f, 3, 15000, 2.2f, ok);
    if (!ok) {
        cfg.defaulted = true;
        serial.print("No answer; using 2.2.");
    }

    if (!serial.get_yn("Save?", 0, 0, true)) {   // infinite retries: must be answered
        serial.print("Discarded.");
        return;
    }

    // render_table returns the table instead of printing it, so the same render
    // can also go to a log or a web response. print_table writes exactly this
    // string and nothing else.
    const std::string brightness_str = std::to_string(cfg.brightness);
    const std::string gamma_str      = String(cfg.gamma, 2).c_str();
    const std::string mode_str       = std::to_string(cfg.mode);

    const std::string summary = serial.render_table({
        {"Setting",    "Value"},
        {"Name",       cfg.name},           // every cell is a string_view:
        {"Mode",       mode_str},           // it must outlive this call, so no
        {"Brightness", brightness_str},     // inline std::to_string(...) here
        {"Gamma",      gamma_str},
    }, "Saved");

    serial.printf("(%u bytes, reusable)", static_cast<unsigned>(summary.size()));
    serial.print(summary, "");   // already CRLF-terminated, so no extra line ending

    if (cfg.defaulted) serial.print("Some values were defaulted.");
}

void loop() {
    serial.loop();
}
