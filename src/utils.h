#pragma once

#include "lexer.h"
#include <cstddef>
#include <new>

namespace Compiler {

class ArenaAllocator {
  public:
    explicit ArenaAllocator(size_t chunkSize);
    ~ArenaAllocator();

    ArenaAllocator(const ArenaAllocator&) = delete;
    ArenaAllocator& operator=(const ArenaAllocator&) = delete;

    template <typename T, typename... Args>
    T* alloc(Args&&... args) {
        if (m_Offset + sizeof(T) > m_Buffer + m_Size) {
            throw std::bad_alloc();
        }

        std::byte* start = m_Offset;
        m_Offset += sizeof(T);
        return new (start) T(std::forward<Args>(args)...);
    }

  private:
    const size_t m_Size;

    std::byte* m_Buffer;
    std::byte* m_Offset;
};

void Error(SourceLocation loc, const std::string& msg);
void Error(const std::string& msg);

} // namespace Compiler
