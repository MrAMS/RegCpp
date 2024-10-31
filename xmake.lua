add_requires("catch2")

target("FrameworkTest")
    set_kind("binary")
    set_languages("cxx17")
    add_includedirs(".")
    add_packages("catch2")
    add_files("test.cpp")