#pragma once
#include "FileBase.hpp"
#include <string>

namespace FsLib
{
    class InputFile : public FsLib::FileBase
    {
        public:
            InputFile(void) = default;
            // This contructor calls Open for you.
            InputFile(const std::string &FilePath);
            // Opens file for reading. IsOpen can be checked to see if operation succeeded.
            void Open(const std::string &FilePath);
            // Attempts to read ReadSize in bytes to Buffer. Returns number of bytes read on success or 0 on failure.
            size_t Read(void *Buffer, size_t ReadSize);
            // Attempts to read a line from file or until '\n' or '\r' is hit. Returns true on success or false on failure. Line is written to LineOut
            bool ReadLine(std::string &LineOut);
            // Reads a single byte from file
            char GetCharacter(void);
    };
} // namespace FsLib