#pragma once

#include <filesystem>
#include <string>
#include <string_view>
#include <vector>

namespace generalFunctions {

// Reads all bytes without changing line endings, including zero bytes.
// Throws std::runtime_error on file errors.
[[nodiscard]] std::string readFile(const std::filesystem::path& path);

// Reads lines without LF/CRLF endings; preserves empty lines.
// A final newline does not create an extra empty line.
[[nodiscard]] std::vector<std::string> readLines(const std::filesystem::path& path);

// Creates or truncates a file. Parent directories must already exist.
// Writes bytes unchanged. Replacement is not atomic.
// Throws std::runtime_error on open/write/close errors.
void writeFile(const std::filesystem::path& path, std::string_view content);

// Creates or appends. Does not insert a newline automatically.
void appendFile(const std::filesystem::path& path, std::string_view content);

} // namespace generalFunctions
