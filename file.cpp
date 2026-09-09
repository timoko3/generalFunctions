#include "file.h"

#include <algorithm>
#include <array>
#include <cstddef>
#include <fstream>
#include <stdexcept>
#include <utility>

namespace generalFunctions {
namespace {

constexpr std::size_t fileBufferSize = 64 * 1024;

[[noreturn]] void fileError(const char* operation,
                           const std::filesystem::path& path)
{
    throw std::runtime_error(std::string(operation) + ": " + path.string());
}

void writeContent(const std::filesystem::path& path,
                  std::string_view content, std::ios::openmode mode)
{
    std::ofstream file(path, std::ios::binary | std::ios::out | mode);
    if (!file) {
        fileError("Cannot open file for writing", path);
    }

    // Bounded chunks keep conversion to streamsize safe for large inputs.
    while (!content.empty()) {
        const auto count = std::min(content.size(), fileBufferSize);
        file.write(content.data(), static_cast<std::streamsize>(count));
        if (!file) {
            fileError("Cannot write file", path);
        }
        content.remove_prefix(count);
    }

    // Buffered errors may appear only when flushing/closing the file.
    file.close();
    if (!file) {
        fileError("Cannot close file after writing", path);
    }
}

} // namespace

std::string readFile(const std::filesystem::path& path)
{
    std::ifstream file(path, std::ios::binary);
    if (!file) {
        fileError("Cannot open file for reading", path);
    }

    std::string result;
    std::array<char, fileBufferSize> buffer{};
    while (file.read(buffer.data(),
                     static_cast<std::streamsize>(buffer.size()))) {
        result.append(buffer.data(), buffer.size());
    }
    result.append(buffer.data(), static_cast<std::size_t>(file.gcount()));

    // A short final read sets failbit and eofbit; this is normal.
    if (file.bad() || !file.eof()) {
        fileError("Cannot read file", path);
    }
    return result;
}

std::vector<std::string> readLines(const std::filesystem::path& path)
{
    std::ifstream file(path, std::ios::binary);
    if (!file) {
        fileError("Cannot open file for reading", path);
    }

    std::vector<std::string> lines;
    std::string line;
    while (std::getline(file, line)) {
        // Remove CR only if it was followed by the LF delimiter.
        if (!file.eof() && !line.empty() && line.back() == '\r') {
            line.pop_back();
        }
        lines.push_back(std::move(line));
    }
    if (file.bad() || !file.eof()) {
        fileError("Cannot read file", path);
    }
    return lines;
}

void writeFile(const std::filesystem::path& path, std::string_view content)
{
    writeContent(path, content, std::ios::trunc);
}

void appendFile(const std::filesystem::path& path, std::string_view content)
{
    writeContent(path, content, std::ios::app);
}

} // namespace generalFunctions
