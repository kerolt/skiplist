add_rules("mode.debug", "mode.release")
add_rules("plugin.compile_commands.autoupdate", {outputdir = ".vscode"})

set_languages("c++11")

target("skiplist")
    add_includedirs("include/")
    set_kind("binary")
    set_targetdir("bin")
    add_files("src/*.cpp")
