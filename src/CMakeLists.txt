add_executable(
        archiver
        archiver.cpp
        archive.cpp
        unarchive.cpp
)

add_catch(test_priority_queue test_priority_queue.cpp)
add_catch(test_bits_stream test_bits_stream.cpp)
