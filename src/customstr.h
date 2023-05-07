/* The usage of the string datatype caused problem during compilation so this is abomination exists.
* If anyone knows how to resolve this compilation issue, please tell me !
* 
* Data-structure used to store char array. It provides dynamic allocation and some method to insert or delete chars.
* Should be full of memory management problems, but arm-none-eabi-g++ (the compilator I use) refused to link libsdt++ correcly.
*/
#ifndef CUSTOM_STR
#define CUSTOM_STR
#include <cstdlib>
#include <algorithm>

/*Number of char in a char array (does not count the null char at the end)
*/
size_t strlen(const char* str) {
    // Initialize the length to zero
    size_t len = 0;

    // Loop through the string until the null terminator is reached
    while (*str != '\0') {
        // Increment the length
        len++;

        // Move to the next character
        str++;
    }

    // Return the length
    return len;
}

/* I had to reimplement this shit without the 'new' operator
*/
void* memcpy(void* dst, const void* src, size_t n) {
    // Cast the pointers to char pointers for byte-wise copy
    char* d = static_cast<char*>(dst);
    const char* s = static_cast<const char*>(src);

    // Copy n bytes from src to dst
    for (size_t i = 0; i < n; i++) {
        d[i] = s[i];
    }

    // Return the destination pointer
    return dst;
}

/* I had to reimplement this too shit without the 'new' operator
*/
void* memmove(void* dst, const void* src, size_t n) {
    // Cast the pointers to char pointers for byte-wise copy
    char* d = static_cast<char*>(dst);
    const char* s = static_cast<const char*>(src);

    // If the destination and source buffers overlap, copy the bytes in reverse order
    if (d > s && d < s + n) {
        for (size_t i = n; i > 0; i--) {
            d[i - 1] = s[i - 1];
        }
    }
    else {
        // Otherwise, copy the bytes in the regular order
        for (size_t i = 0; i < n; i++) {
            d[i] = s[i];
        }
    }

    // Return the destination pointer
    return dst;
}

class CustomSTR {
public:
    CustomSTR() : m_data{} {}

    CustomSTR(const char* str) {
        size_t size = strlen(str);
        memcpy(m_data, str, size);
        m_data[size] = '\0';
    }

    CustomSTR(const CustomSTR& other) {
        size_t size = other.length();
        memcpy(m_data, other.m_data, size + 1);
    }

    ~CustomSTR() = default; //The delete operator does not work :(

    CustomSTR& operator=(const CustomSTR& other) {
        if (this == &other) {
            return *this;
        }

        size_t size = other.length();
        memcpy(m_data, other.m_data, size + 1);
        return *this;
    }

    size_t length() const {
        return strlen(m_data);
    }

    char* get() {
        return m_data;
    }
    void erase(size_t pos, size_t len) {
        size_t str_len = length();
        if (pos > str_len) {
            return;
        }
        if (pos + len > str_len) {
            len = str_len - pos;
        }
        memmove(m_data + pos, m_data + pos + len, str_len - pos - len + 1);
    }

    /*Return the chars between index p1 and p2 (including those)
    */
    void window(int p1, int p2, char* buffer, int bufferSize)
    {
        int len = length();
        if (p1 < 0 || p1 >= len) {  // Invalid p1 index
            if (bufferSize > 0) {
                buffer[0] = '\0';
            }
            return;
        }
        if (p2 < p1) {  // Invalid p2 index
            if (bufferSize > 0) {
                buffer[0] = '\0';
            }
            return;
        }
        int windowSize = std::min(p2 - p1 + 1, len - p1);
        if (bufferSize <= windowSize) {  // Buffer is too small
            if (bufferSize > 0) {
                buffer[0] = '\0';
            }
            return;
        }
        for (int i = 0; i < windowSize; i++) {
            buffer[i] = m_data[p1 + i];
        }
        buffer[windowSize] = '\0';
    }

    /*Insert a char at a given position in the array
    */
    void insert(char c, size_t pos) {
        size_t len = length();
        if (pos > len) {
            pos = len;
        }
        for (size_t i = len; i > pos; --i) {
            m_data[i] = m_data[i - 1];
        }
        m_data[pos] = c;
        m_data[len + 1] = '\0';
    }

private:
    static constexpr size_t MAX_SIZE = 32;
    char m_data[MAX_SIZE + 1];
};


#endif