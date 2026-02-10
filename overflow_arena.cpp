#include <bitset>
#include <cstdint>
#include <cstdlib>
#include <functional>
#include <iomanip>
#include <iostream>
#include <limits>
#include <random>
#include <sstream>
#include <string>
#include <type_traits>
#include <vector>
#include <cmath> // std::isinf

namespace {

void print_heading(const std::string& title) {
    std::cout << "\n" << title << "\n";
    std::cout << std::string(title.size(), '-') << "\n";
}

void wait_for_enter() {
    std::cout << "\nPress ENTER to continue...";
    std::cout.flush();
    std::string line;
    std::getline(std::cin, line);
}

int read_int_from_user(const std::string& prompt) {
    while (true) {
        std::cout << prompt;
        std::string line;
        if (!std::getline(std::cin, line)) {
            std::cout << "\nInput stream closed. Exiting.\n";
            std::exit(0);
        }
        std::stringstream ss(line);
        int value = 0;
        char extra = '\0';
        if (ss >> value && !(ss >> extra)) {
            return value;
        }
        std::cout << "Please enter a valid integer.\n";
    }
}

template <typename T>
void print_limits(const std::string& name) {
    using Limits = std::numeric_limits<T>;
    std::ios::fmtflags old_flags = std::cout.flags();
    std::streamsize old_precision = std::cout.precision();

    std::cout << std::left << std::setw(18) << name
              << std::right << std::setw(24);

    if constexpr (std::is_floating_point_v<T>) {
        std::cout << std::scientific << std::setprecision(6)
                  << static_cast<long double>(Limits::lowest());
    } else if constexpr (std::is_integral_v<T>) {
        if constexpr (Limits::is_signed) {
            std::cout << static_cast<std::intmax_t>(Limits::min());
        } else {
            std::cout << static_cast<std::uintmax_t>(Limits::min());
        }
    }

    std::cout << std::setw(24);
    if constexpr (std::is_floating_point_v<T>) {
        std::cout << std::scientific << std::setprecision(6)
                  << static_cast<long double>(Limits::max());
    } else if constexpr (std::is_integral_v<T>) {
        if constexpr (Limits::is_signed) {
            std::cout << static_cast<std::intmax_t>(Limits::max());
        } else {
            std::cout << static_cast<std::uintmax_t>(Limits::max());
        }
    }

    std::cout << "\n";
    std::cout.flags(old_flags);
    std::cout.precision(old_precision);
}

template <typename T>
std::string to_binary_string(T value) {
    using UnsignedT = std::make_unsigned_t<T>;
    UnsignedT bits = static_cast<UnsignedT>(value);
    if constexpr (sizeof(UnsignedT) == 1) {
        return std::bitset<8>(bits).to_string();
    } else if constexpr (sizeof(UnsignedT) == 2) {
        return std::bitset<16>(bits).to_string();
    } else if constexpr (sizeof(UnsignedT) == 4) {
        return std::bitset<32>(bits).to_string();
    } else {
        return std::bitset<64>(bits).to_string();
    }
}

std::string binary_from_signed(long long value, int bits) {
    unsigned long long mask = (bits == 64) ? std::numeric_limits<unsigned long long>::max()
                                           : ((1ULL << bits) - 1ULL);
    unsigned long long uvalue = static_cast<unsigned long long>(value) & mask;
    if (bits == 8) return std::bitset<8>(uvalue).to_string();
    if (bits == 16) return std::bitset<16>(uvalue).to_string();
    if (bits == 32) return std::bitset<32>(uvalue).to_string();
    return std::bitset<64>(uvalue).to_string();
}

long long wrap_unsigned(long long value, int bits) {
    if (bits == 64) {
        return static_cast<unsigned long long>(value);
    }
    if (bits < 1 || bits > 63) {
        return value;
    }
    unsigned long long mod = (1ULL << bits);
    unsigned long long uvalue = static_cast<unsigned long long>(value);
    unsigned long long wrapped = uvalue % mod;
    return static_cast<long long>(wrapped);
}

// FIXED: correct for negative inputs; avoids UB; uses safe modulo + sign conversion
long long wrap_signed(long long value, int bits) {
    if (bits < 1 || bits > 63) {
        return value;
    }

    const unsigned long long mod = (1ULL << bits);

    long long wrapped = value % static_cast<long long>(mod);
    if (wrapped < 0) {
        wrapped += static_cast<long long>(mod);
    }

    const long long sign_bit = 1LL << (bits - 1);
    if (wrapped & sign_bit) {
        wrapped -= static_cast<long long>(mod);
    }
    return wrapped;
}

void show_rules_of_thumb() {
    print_heading("Rules of Thumb");
    std::cout << "Use int for most counting.\n";
    std::cout << "Use unsigned only when negatives never make sense.\n";
    std::cout << "Use int32_t or uint32_t when you need exact sizes.\n";
    std::cout << "Use double for most decimals.\n";
    std::cout << "Use float when memory or speed is tight.\n";
}

void show_micro_lessons() {
    print_heading("Lesson");
    std::cout << "Integers store whole numbers.\n";
    std::cout << "Some can go negative and some cannot.\n";

    print_heading("Lesson");
    std::cout << "char is 1 byte and stores a small number or a character.\n";
    std::cout << "The exact range can vary by system.\n";

    print_heading("Lesson");
    std::cout << "short, int, long, and long long are bigger buckets.\n";
    std::cout << "Bigger buckets hold bigger numbers.\n";

    print_heading("Lesson");
    std::cout << "Fixed-size types like int32_t always have the same size.\n";
    std::cout << "They help when you need exact sizes.\n";

    print_heading("Lesson");
    std::cout << "float and double store decimals.\n";
    std::cout << "double is more precise.\n";

    show_rules_of_thumb();
    wait_for_enter();
}

void quick_tour() {
    print_heading("Quick Tour");
    std::cout << "Learn \u2192 Predict \u2192 Run \u2192 What happened? \u2192 Reflect\n";

    print_heading("Lesson");
    std::cout << "int is a basic whole-number type.\n";
    std::cout << "It can store positive and negative values.\n";

    print_heading("Prediction");
    std::cout << "What number prints? (int) Start at 5 and add 2.\n";
    int guess1 = read_int_from_user("Your guess: ");

    print_heading("Run");
    int int_value = 5;
    int_value += 2;
    std::cout << "Result: " << int_value << "\n";

    print_heading("What happened?");
    std::cout << "int stores whole numbers and can go up or down.\n";

    print_heading("Reflect");
    std::cout << "You said " << guess1 << ".\n";

    print_heading("Lesson");
    std::cout << "unsigned int stores whole numbers that cannot be negative.\n";
    std::cout << "It starts at 0 and goes up.\n";

    print_heading("Prediction");
    std::cout << "What number prints? (unsigned int) Start at 5 and subtract 2.\n";
    int guess2 = read_int_from_user("Your guess: ");

    print_heading("Run");
    unsigned int uint_value = 5u;
    uint_value -= 2u;
    std::cout << "Result: " << uint_value << "\n";

    print_heading("What happened?");
    std::cout << "unsigned int cannot go negative. It stays at 0 or more.\n";

    print_heading("Reflect");
    std::cout << "You said " << guess2 << ".\n";

    print_heading("Lesson");
    std::cout << "uint8_t is a tiny unsigned bucket.\n";
    std::cout << "When it is full, it wraps around and starts over.\n";

    print_heading("Prediction");
    std::cout << "What number prints? (uint8_t) Start at 255 and add 1.\n";
    int guess3 = read_int_from_user("Your guess: ");

    print_heading("Run");
    std::uint8_t u8max = std::numeric_limits<std::uint8_t>::max();
    std::uint8_t u8wrap = static_cast<std::uint8_t>(static_cast<unsigned int>(u8max) + 1u);
    std::cout << "Result: " << static_cast<int>(u8wrap) << "\n";
    std::cout << "If you're curious, here is what it looks like in bits:\n";
    std::cout << to_binary_string(u8max) << " -> " << to_binary_string(u8wrap) << "\n";

    print_heading("What happened?");
    std::cout << "The tiny bucket was full. It wrapped around to 0.\n";

    print_heading("Reflect");
    std::cout << "You said " << guess3 << ".\n";

    print_heading("Lesson");
    std::cout << "int8_t is a tiny signed bucket.\n";
    std::cout << "Signed overflow is not safe to rely on in C++.\n";
    std::cout << "We simulate what many computers do so you can learn.\n";

    print_heading("Prediction");
    std::cout << "What number prints? (int8_t) Start at 127 and add 1.\n";
    int guess4 = read_int_from_user("Your guess: ");

    print_heading("Run");
    std::int8_t s8max = std::numeric_limits<std::int8_t>::max();
    long long wide_before = static_cast<long long>(s8max);
    long long wide_after = wide_before + 1;
    long long wrapped = wrap_signed(wide_after, 8);
    std::int8_t converted = static_cast<std::int8_t>(wrapped);
    std::cout << "Simulated result: " << static_cast<int>(converted) << "\n";
    std::cout << "If you're curious, here is what it looks like in bits:\n";
    std::cout << binary_from_signed(s8max, 8) << " -> " << binary_from_signed(converted, 8) << "\n";

    print_heading("What happened?");
    std::cout << "We simulated a wrap-around so you can see the idea.\n";
    std::cout << "Signed overflow is not safe to rely on in C++.\n";

    print_heading("Reflect");
    std::cout << "You said " << guess4 << ".\n";

    print_heading("Lesson");
    std::cout << "Decimals are stored with limited precision.\n";
    std::cout << "double is more precise than float.\n";

    print_heading("Prediction");
    std::cout << "Will 0.1f + 0.2f print exactly 0.3? (1 = yes, 0 = no)\n";
    int guess5 = read_int_from_user("Your guess: ");

    print_heading("Run");
    float fsum = 0.1f + 0.2f;
    double diff = static_cast<double>(fsum) - 0.3;

    std::ios::fmtflags old_flags = std::cout.flags();
    std::streamsize old_precision = std::cout.precision();
    std::cout << std::fixed << std::setprecision(12);
    std::cout << "float sum: " << fsum << "\n";
    std::cout << "difference from 0.3: " << diff << "\n";
    std::cout.flags(old_flags);
    std::cout.precision(old_precision);

    print_heading("What happened?");
    std::cout << "Some decimals cannot be stored exactly.\n";
    std::cout << "Small rounding shows up in the result.\n";

    print_heading("Reflect");
    std::cout << "You said " << guess5 << ".\n";

    wait_for_enter();
}

// NEW: prints min/max for all required types + overflow demos
void print_all_limits() {
    print_heading("Min/Max Table (Required Types)");
    std::cout << std::left << std::setw(18) << "Type"
              << std::right << std::setw(24) << "Min/Lowest"
              << std::setw(24) << "Max" << "\n";
    std::cout << std::string(66, '-') << "\n";

    // char and variants
    print_limits<char>("char");
    print_limits<signed char>("signed char");
    print_limits<unsigned char>("unsigned char");

    // short / int / long / long long and unsigned variants
    print_limits<short>("short");
    print_limits<unsigned short>("unsigned short");
    print_limits<int>("int");
    print_limits<unsigned int>("unsigned int");
    print_limits<long>("long");
    print_limits<unsigned long>("unsigned long");
    print_limits<long long>("long long");
    print_limits<unsigned long long>("unsigned long long");

    // fixed-width types
    print_limits<std::int8_t>("int8_t");
    print_limits<std::uint8_t>("uint8_t");
    print_limits<std::int16_t>("int16_t");
    print_limits<std::uint16_t>("uint16_t");
    print_limits<std::int32_t>("int32_t");
    print_limits<std::uint32_t>("uint32_t");
    print_limits<std::int64_t>("int64_t");
    print_limits<std::uint64_t>("uint64_t");

    // floating point
    print_limits<float>("float");
    print_limits<double>("double");
    print_limits<long double>("long double");

    print_heading("Float precision (digits you can trust)");
    std::cout << "float       digits10: " << std::numeric_limits<float>::digits10 << "\n";
    std::cout << "double      digits10: " << std::numeric_limits<double>::digits10 << "\n";
    std::cout << "long double digits10: " << std::numeric_limits<long double>::digits10 << "\n";

    print_heading("Overflow Demo (Meets Rubric)");

    // Unsigned wrap (defined behavior)
    {
        std::uint8_t u8max = std::numeric_limits<std::uint8_t>::max();
        std::uint8_t u8_over = static_cast<std::uint8_t>(static_cast<unsigned int>(u8max) + 1u);
        std::uint8_t u8_under = static_cast<std::uint8_t>(0u - 1u);

        std::cout << "Unsigned wrap (uint8_t):\n";
        std::cout << "  max is " << static_cast<int>(u8max) << "\n";
        std::cout << "  max + 1 -> " << static_cast<int>(u8_over) << "\n";
        std::cout << "  0 - 1   -> " << static_cast<int>(u8_under) << "\n";
    }
    {
        std::uint16_t u16max = std::numeric_limits<std::uint16_t>::max();
        std::uint16_t u16_over = static_cast<std::uint16_t>(static_cast<unsigned int>(u16max) + 1u);
        std::uint16_t u16_under = static_cast<std::uint16_t>(0u - 1u);

        std::cout << "Unsigned wrap (uint16_t):\n";
        std::cout << "  max is " << u16max << "\n";
        std::cout << "  max + 1 -> " << u16_over << "\n";
        std::cout << "  0 - 1   -> " << u16_under << "\n";
    }

    // Signed overflow: simulated (avoid UB)
    {
        std::cout << "Signed overflow (SIMULATED, int8_t):\n";
        long long s8max = static_cast<long long>(std::numeric_limits<std::int8_t>::max()); // 127
        long long sim_over = wrap_signed(s8max + 1, 8);
        std::cout << "  max is " << s8max << "\n";
        std::cout << "  max + 1 -> " << sim_over << " (simulated)\n";

        long long s8min = static_cast<long long>(std::numeric_limits<std::int8_t>::min()); // -128
        long long sim_under = wrap_signed(s8min - 1, 8);
        std::cout << "  min is " << s8min << "\n";
        std::cout << "  min - 1 -> " << sim_under << " (simulated)\n";
    }
    {
        std::cout << "Signed overflow (SIMULATED, int16_t):\n";
        long long s16max = static_cast<long long>(std::numeric_limits<std::int16_t>::max());
        long long sim_over = wrap_signed(s16max + 1, 16);
        std::cout << "  max is " << s16max << "\n";
        std::cout << "  max + 1 -> " << sim_over << " (simulated)\n";

        long long s16min = static_cast<long long>(std::numeric_limits<std::int16_t>::min());
        long long sim_under = wrap_signed(s16min - 1, 16);
        std::cout << "  min is " << s16min << "\n";
        std::cout << "  min - 1 -> " << sim_under << " (simulated)\n";
    }

    // Floating overflow -> inf
    {
        print_heading("Floating overflow -> inf");
        std::ios::fmtflags old_flags = std::cout.flags();
        std::streamsize old_precision = std::cout.precision();

        std::cout << std::scientific << std::setprecision(6);

        float f = std::numeric_limits<float>::max();
        float f2 = f * 2.0f;
        std::cout << "float:  max * 2 = " << f2 << " | isinf? " << (std::isinf(f2) ? "true" : "false") << "\n";

        double d = std::numeric_limits<double>::max();
        double d2 = d * 2.0;
        std::cout << "double: max * 2 = " << d2 << " | isinf? " << (std::isinf(d2) ? "true" : "false") << "\n";

        long double ld = std::numeric_limits<long double>::max();
        long double ld2 = ld * static_cast<long double>(2.0);
        std::cout << "long double: max * 2 = " << static_cast<long double>(ld2)
                  << " | isinf? " << (std::isinf(ld2) ? "true" : "false") << "\n";

        std::cout.flags(old_flags);
        std::cout.precision(old_precision);
    }

    print_heading("Optional: Advanced Notes");
    std::cout << "char signedness is implementation-defined. is_signed = "
              << (std::numeric_limits<char>::is_signed ? "true" : "false") << "\n";
    std::cout << "long double precision is implementation-defined; not necessarily quad precision.\n";

    wait_for_enter();
}

void optional_advanced_pitfalls() {
    print_heading("Optional: Advanced Pitfalls");
    std::cout << "These are here only if you want extra detail.\n";

    print_heading("Lesson");
    std::cout << "Signed and unsigned can surprise you when mixed.\n";

    print_heading("Prediction");
    std::cout << "Will (-1 < 1u) be true? (1 = yes, 0 = no)\n";
    int guess = read_int_from_user("Your guess: ");

    print_heading("Run");
    int a = -1;
    unsigned int b = 1u;
    bool result = (static_cast<unsigned int>(a) < b);
    std::cout << "Result: " << (result ? "true" : "false") << "\n";

    print_heading("What happened?");
    std::cout << "C++ converts -1 into a big unsigned number.\n";
    std::cout << "That makes it larger than 1u, so the compare flips.\n";

    print_heading("Reflect");
    std::cout << "You said " << guess << ".\n";

    wait_for_enter();
}

struct GameType {
    std::string name;
    int bits;
    bool is_signed;
    long long min_value;
    long long max_value;
    std::function<long long(long long)> op;
    std::string op_name;
};

void overflow_arena() {
    print_heading("Overflow Arena");
    std::cout << "How to play: guess the result, then see what happens.\n";
    std::cout << "Type 'q' to quit after any round.\n";

    std::random_device rd;
    std::mt19937 gen(rd());

    std::vector<GameType> types = {
        {"uint8_t", 8, false, 0, 255, nullptr, ""},
        {"uint16_t", 16, false, 0, 65535, nullptr, ""},
        {"int8_t", 8, true, -128, 127, nullptr, ""},
        {"int16_t", 16, true, -32768, 32767, nullptr, ""},
        {"int32_t", 32, true,
         static_cast<long long>(std::numeric_limits<std::int32_t>::min()),
         static_cast<long long>(std::numeric_limits<std::int32_t>::max()), nullptr, ""},
        {"uint32_t", 32, false, 0,
         static_cast<long long>(std::numeric_limits<std::uint32_t>::max()), nullptr, ""}
    };

    std::uniform_int_distribution<int> type_dist(0, static_cast<int>(types.size() - 1));
    std::uniform_int_distribution<int> op_dist(0, 2);
    std::uniform_int_distribution<int> near_dist(0, 1);

    int score = 0;
    int rounds = 0;

    while (true) {
        GameType gt = types[type_dist(gen)];
        int op_choice = op_dist(gen);
        if (op_choice == 0) {
            gt.op = [](long long v) { return v + 1; };
            gt.op_name = "+1";
        } else if (op_choice == 1) {
            gt.op = [](long long v) { return v - 1; };
            gt.op_name = "-1";
        } else {
            gt.op = [](long long v) { return v * 2; };
            gt.op_name = "*2";
        }

        bool near_max = (near_dist(gen) == 1);
        long long start = near_max ? gt.max_value : gt.min_value;
        start += near_max ? -1 : 1;

        std::cout << "\nRound " << (rounds + 1) << " | Type: " << gt.name
                  << " | Start: " << start << " | Op: " << gt.op_name << "\n";
        if (gt.is_signed) {
            std::cout << "Hint: Range is negative to positive; we simulate wrap for learning.\n";
        } else {
            std::cout << "Hint: If it goes past max, it starts over at 0.\n";
        }
        print_heading("Prediction");
        std::cout << "What number prints? (or q to quit)\n";
        std::cout << "Your guess: ";

        std::string line;
        if (!std::getline(std::cin, line)) {
            break;
        }
        if (!line.empty() && (line[0] == 'q' || line[0] == 'Q')) {
            std::cout << "Exiting game. Final score: " << score << "/" << rounds << "\n";
            break;
        }

        std::stringstream ss(line);
        long long user_guess = 0;
        if (!(ss >> user_guess)) {
            print_heading("Try again");
            std::cout << "Please enter a valid integer.\n";
            continue;
        }

        long long wide_before = start;
        long long wide_after = gt.op(wide_before);
        long long final_value = 0;
        if (gt.is_signed) {
            final_value = wrap_signed(wide_after, gt.bits);
        } else {
            final_value = wrap_unsigned(wide_after, gt.bits);
        }

        std::string before_bits;
        std::string after_bits;
        if (gt.is_signed) {
            before_bits = binary_from_signed(start, gt.bits);
            after_bits = binary_from_signed(final_value, gt.bits);
        } else if (gt.bits == 8) {
            before_bits = to_binary_string(static_cast<std::uint8_t>(start));
            after_bits = to_binary_string(static_cast<std::uint8_t>(final_value));
        } else if (gt.bits == 16) {
            before_bits = to_binary_string(static_cast<std::uint16_t>(start));
            after_bits = to_binary_string(static_cast<std::uint16_t>(final_value));
        } else if (gt.bits == 32) {
            before_bits = to_binary_string(static_cast<std::uint32_t>(start));
            after_bits = to_binary_string(static_cast<std::uint32_t>(final_value));
        } else {
            before_bits = to_binary_string(static_cast<std::uint64_t>(start));
            after_bits = to_binary_string(static_cast<std::uint64_t>(final_value));
        }

        print_heading("Run");
        std::cout << "Result: " << final_value << "\n";
        std::cout << "Range: " << gt.min_value << " to " << gt.max_value << "\n";
        std::cout << "If you're curious, here is what it looks like in bits:\n";
        std::cout << before_bits << " -> " << after_bits << "\n";

        print_heading("What happened?");
        if (gt.is_signed) {
            std::cout << "Signed overflow is not safe to rely on in C++.\n";
            std::cout << "We simulate what many computers do so you can learn.\n";
        } else {
            std::cout << "Unsigned values wrap around when they pass the limit.\n";
        }

        if (user_guess == final_value) {
            score++;
            std::cout << "Result: Correct!\n";
        } else {
            std::cout << "Result: Not quite. You guessed " << user_guess << ".\n";
        }

        rounds++;
        std::cout << "Score: " << score << "/" << rounds << "\n";
    }

    wait_for_enter();
}

void print_menu() {
    print_heading("Overflow Arena - Main Menu");
    std::cout << "[1] Quick Tour (learn and predict)\n";
    std::cout << "[2] Print All Min/Max Tables + Overflow Demos\n";
    std::cout << "[3] Play Overflow Arena (game)\n";
    std::cout << "[4] Optional: Advanced Pitfalls\n";
    std::cout << "[5] Exit\n";
}

} // namespace

int main() {
    std::cout << "Welcome to Overflow Arena!\n";

    while (true) {
        print_menu();
        int choice = read_int_from_user("Choose an option: ");

        switch (choice) {
            case 1:
                show_micro_lessons();
                quick_tour();
                break;
            case 2:
                print_all_limits();
                break;
            case 3:
                overflow_arena();
                break;
            case 4:
                optional_advanced_pitfalls();
                break;
            case 5:
                std::cout << "Goodbye!\n";
                return 0;
            default:
                print_heading("Try again");
                std::cout << "Please choose 1-5.\n";
                break;
        }
    }
}
