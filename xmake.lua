add_rules("mode.debug", "mode.release")

if is_plat("windows") and not is_host("windows") then
    set_toolchains("mingw")
end

target("PapyrusSourceHeadliner")
    set_kind("binary")
    set_languages("c++17")
    add_files("PapyrusSourceHeadliner/*.cpp")
    add_includedirs("PapyrusSourceHeadliner")
    set_version("1.1.0")

    if is_plat("windows") then
        add_links("kernel32", "user32")
        add_defines("WIN32", "_WIN32")
        set_extension(".exe")
    elseif is_plat("linux") then
        add_links("pthread")
        add_defines("LINUX")
    end

    on_config(function (target)
        if target:is_plat("windows") and target:toolchain("mingw") then
            target:add("ldflags", "-static", {force = true})
        end
    end)

    add_cxxflags("-Wall", "-Wextra")
    if is_mode("release") then
        add_cxxflags("-O3")
        add_defines("NDEBUG")
    else
        add_cxxflags("-g")
        add_defines("DEBUG")
    end