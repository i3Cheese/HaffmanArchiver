#include <string>
#include <fstream>
#include <utility>

template <typename... Args>
inline std::ofstream OpenFile(Args&&... args) {
    std::ofstream f_stream(std::forward<Args>(args)...);
    f_stream.exceptions(std::ios_base::failbit);
}
