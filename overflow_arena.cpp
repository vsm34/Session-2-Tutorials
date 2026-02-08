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

/* ===== FIXED / HARDENED ===== */
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
/* ============================ */

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

/* ---------------- MAIN PROGRAM ---------------- */

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
         static_cast<long long>(std::numeric_limits<std::int32_t>::max()),
         nullptr, ""},
        {"uint32_t", 32, false, 0,
         static_cast<long long>(std::numeric_limits<std::uint32_t>::max()),
         nullptr, ""}
    };

    std::uniform_int_distribution<int> type_dist(0, static_cast<int>(types.size() - 1));
    std::uniform_int_distribution<int> op_dist(0, 2);
    std::uniform_int_distribution<int> near_dist(0, 1);

    int score = 0;
    int rounds = 0;

    while (true) {
        GameType gt = types[type_dist(gen)];
        int op_choice = op_dist(gen);

        if (op_choice == 0) { gt.op = [](long long v){ return v + 1; }; gt.op_name = "+1"; }
        else if (op_choice == 1) { gt.op = [](long long v){ return v - 1; }; gt.op_name = "-1"; }
        else { gt.op = [](long long v){ return v * 2; }; gt.op_name = "*2"; }

        bool near_max = (near_dist(gen) == 1);
        long long start = near_max ? gt.max_value : gt.min_value;
        start += near_max ? -1 : 1;

        std::cout << "\nRound " << (rounds + 1)
                  << " | Type: " << gt.name
                  << " | Start: " << start
                  << " | Op: " << gt.op_name << "\n";

        std::cout << "Your guess (or q): ";
        std::string line;
        if (!std::getline(std::cin, line)) break;
        if (!line.empty() && (line[0] == 'q' || line[0] == 'Q')) break;

        std::stringstream ss(line);
        long long guess;
        if (!(ss >> guess)) continue;

        long long wide_after = gt.op(start);
        long long final_value =
            gt.is_signed ? wrap_signed(wide_after, gt.bits)
                         : wrap_unsigned(wide_after, gt.bits);

        std::cout << "Result: " << final_value << "\n";

        if (guess == final_value) score++;
        rounds++;

        std::cout << "Score: " << score << "/" << rounds << "\n";
    }

    wait_for_enter();
}

void print_menu() {
    print_heading("Overflow Arena - Main Menu");
    std::cout << "[1] Quick Tour\n";
    std::cout << "[2] Print Min/Max Tables\n";
    std::cout << "[3] Play Overflow Arena\n";
    std::cout << "[4] Exit\n";
}

} // namespace

int main() {
    std::cout << "Welcome to Overflow Arena!\n";

    while (true) {
        print_menu();
        int choice = read_int_from_user("Choose an option: ");

        if (choice == 1) show_micro_lessons();
        else if (choice == 2) print_limits<int>("int");
        else if (choice == 3) overflow_arena();
        else if (choice == 4) return 0;
    }
}
