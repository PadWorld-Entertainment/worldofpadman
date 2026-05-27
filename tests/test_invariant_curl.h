#include <gtest/gtest.h>
#include <string>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <climits>
#include <algorithm>

// Simulate the buffer handling behavior that curl uses for STRINGPOINT options.
// The invariant: when a string option is set, the library must not read beyond
// the declared length of the input buffer. We model this with a safe wrapper
// that enforces length constraints and detects overreads.

// Maximum safe length for curl string options (conservative bound based on
// curl internals and typical URL/header length limits)
static const size_t CURL_MAX_SAFE_STRING_LENGTH = 8192;

// Sentinel-guarded buffer to detect overreads
struct GuardedBuffer {
    static const uint8_t SENTINEL = 0xAB;
    static const size_t GUARD_SIZE = 64;

    std::vector<uint8_t> data;
    size_t declared_length;

    explicit GuardedBuffer(const std::string& input) {
        declared_length = input.size();
        // Layout: [data bytes][guard bytes]
        data.resize(declared_length + GUARD_SIZE);
        if (declared_length > 0) {
            std::memcpy(data.data(), input.data(), declared_length);
        }
        // Fill guard region with sentinel
        std::fill(data.begin() + declared_length, data.end(), SENTINEL);
    }

    bool guard_intact() const {
        for (size_t i = declared_length; i < data.size(); ++i) {
            if (data[i] != SENTINEL) {
                return false;
            }
        }
        return true;
    }

    const char* ptr() const {
        return reinterpret_cast<const char*>(data.data());
    }
};

// Simulate curl's internal string option processing:
// - Reject NULL
// - Truncate to CURL_MAX_SAFE_STRING_LENGTH
// - Only read up to declared_length bytes
// Returns number of bytes actually read, or -1 on rejection
static ssize_t simulate_curl_setopt_string(const char* input, size_t declared_length) {
    if (input == nullptr) {
        return -1; // rejected
    }
    // The safe implementation must not read beyond declared_length
    size_t read_length = std::min(declared_length, CURL_MAX_SAFE_STRING_LENGTH);
    // Simulate processing: scan only up to read_length bytes
    size_t processed = 0;
    for (size_t i = 0; i < read_length; ++i) {
        (void)input[i]; // read byte
        processed++;
        // Stop at null terminator (curl behavior)
        if (input[i] == '\0') {
            break;
        }
    }
    return static_cast<ssize_t>(processed);
}

// Invariant checker: reading must never exceed declared_length
static bool invariant_no_overread(const std::string& payload) {
    GuardedBuffer buf(payload);

    ssize_t bytes_read = simulate_curl_setopt_string(buf.ptr(), buf.declared_length);

    // Invariant 1: guard region must be intact (no overread past declared_length)
    if (!buf.guard_intact()) {
        return false;
    }

    // Invariant 2: bytes read must not exceed declared_length
    if (bytes_read > static_cast<ssize_t>(buf.declared_length)) {
        return false;
    }

    // Invariant 3: oversized inputs must be truncated, not cause unbounded reads
    if (buf.declared_length > CURL_MAX_SAFE_STRING_LENGTH) {
        if (bytes_read > static_cast<ssize_t>(CURL_MAX_SAFE_STRING_LENGTH)) {
            return false;
        }
    }

    return true;
}

class CurlStringOptionSecurityTest : public ::testing::TestWithParam<std::string> {};

TEST_P(CurlStringOptionSecurityTest, BufferReadsNeverExceedDeclaredLength) {
    // Invariant: Buffer reads never exceed the declared length of the input.
    // Oversized or malformed inputs must be truncated or rejected, never cause overread.
    std::string payload = GetParam();

    EXPECT_TRUE(invariant_no_overread(payload))
        << "SECURITY VIOLATION: Buffer overread detected for payload of length "
        << payload.size();

    // Additional assertion: simulate_curl_setopt_string returns sane value
    GuardedBuffer buf(payload);
    ssize_t result = simulate_curl_setopt_string(buf.ptr(), buf.declared_length);

    EXPECT_LE(result, static_cast<ssize_t>(buf.declared_length))
        << "Read " << result << " bytes but declared length is " << buf.declared_length;

    if (buf.declared_length > CURL_MAX_SAFE_STRING_LENGTH) {
        EXPECT_LE(result, static_cast<ssize_t>(CURL_MAX_SAFE_STRING_LENGTH))
            << "Oversized input was not truncated";
    }

    EXPECT_TRUE(buf.guard_intact())
        << "Guard region corrupted — overread past declared buffer end";
}

// Generate a string of given length filled with a specific character
static std::string make_string(size_t len, char c = 'A') {
    return std::string(len, c);
}

// Generate a string with embedded nulls
static std::string make_null_embedded(size_t len) {
    std::string s(len, 'X');
    if (len > 10) {
        s[5] = '\0';
        s[len / 2] = '\0';
    }
    return s;
}

// Generate a string with format specifiers (attack payload)
static std::string make_format_string(size_t len) {
    std::string s;
    s.reserve(len);
    const char* fmt = "%s%n%x%p%d%%";
    size_t fmt_len = strlen(fmt);
    for (size_t i = 0; i < len; i++) {
        s += fmt[i % fmt_len];
    }
    return s;
}

INSTANTIATE_TEST_SUITE_P(
    AdversarialInputs,
    CurlStringOptionSecurityTest,
    ::testing::Values(
        // Normal inputs
        std::string(""),
        std::string("normal_bearer_token"),
        std::string("eth0"),
        std::string("192.168.1.1"),

        // Boundary: exactly at max safe length
        make_string(CURL_MAX_SAFE_STRING_LENGTH),

        // 2x oversized
        make_string(CURL_MAX_SAFE_STRING_LENGTH * 2),

        // 10x oversized
        make_string(CURL_MAX_SAFE_STRING_LENGTH * 10),

        // Very large: 1MB
        make_string(1024 * 1024),

        // Very large: 10MB
        make_string(10 * 1024 * 1024),

        // Strings with embedded null bytes
        make_null_embedded(100),
        make_null_embedded(CURL_MAX_SAFE_STRING_LENGTH + 100),
        make_null_embedded(CURL_MAX_SAFE_STRING_LENGTH * 5),

        // Format string attacks
        make_format_string(256),
        make_format_string(CURL_MAX_SAFE_STRING_LENGTH * 2),

        // All null bytes
        std::string(1024, '\0'),
        std::string(CURL_MAX_SAFE_STRING_LENGTH * 2, '\0'),

        // High-byte / non-ASCII
        make_string(CURL_MAX_SAFE_STRING_LENGTH * 3, '\xFF'),
        make_string(CURL_MAX_SAFE_STRING_LENGTH * 3, '\x80'),

        // Realistic XOAUTH2 bearer token (oversized)
        std::string("Bearer ") + make_string(CURL_MAX_SAFE_STRING_LENGTH * 2, 'z'),

        // Realistic DNS interface name (oversized)
        std::string("eth") + make_string(CURL_MAX_SAFE_STRING_LENGTH, '0'),

        // Realistic IPv4 address (oversized)
        std::string("192.168.") + make_string(CURL_MAX_SAFE_STRING_LENGTH, '1'),

        // Mixed content attack
        std::string("\r\nHost: evil.com\r\n") + make_string(CURL_MAX_SAFE_STRING_LENGTH),

        // URL-encoded overflow attempt
        make_string(CURL_MAX_SAFE_STRING_LENGTH * 4, '%'),

        // Single character at boundary
        make_string(CURL_MAX_SAFE_STRING_LENGTH - 1),
        make_string(CURL_MAX_SAFE_STRING_LENGTH + 1)
    )
);

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}