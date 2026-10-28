
add_executable(TapeLabTests
    SmokeTests.cpp
    TapeSaturationTests.cpp
    ToneFilterTests.cpp
    WetDryMixerTests.cpp
)

target_compile_features(TapeLabTests
    PRIVATE
        cxx_std_20
)

target_link_libraries(TapeLabTests
    PRIVATE
        TapeLabDSP
        Catch2::Catch2WithMain
)

# Register Catch2 test cases with CTest
list(APPEND CMAKE_MODULE_PATH
    "${catch2_SOURCE_DIR}/extras"
)

include(Catch)

catch_discover_tests(TapeLabTests)
