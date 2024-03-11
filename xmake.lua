add_rules("mode.debug", "mode.release")
add_rules("plugin.compile_commands.autoupdate", {outputdir = ".vscode"})

set_languages("c++11")
add_includedirs("include/")

target("skiplist")
    set_kind("binary")
    set_targetdir("bin")
    add_files("src/*.cpp")

target("test_stress")
    set_kind("binary")
    set_targetdir("bin")
    add_files("test/test_stress.cpp")