add_rules("mode.debug", "mode.release")

set_languages("c++11")

target("skiplist")
    set_kind("binary")
    add_files("src/*.cpp")
