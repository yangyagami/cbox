add_rules("mode.debug", "mode.release", "mode.check")

add_rules("plugin.compile_commands.autoupdate", {outputdir = "$(projectdir)"})

includes("camera")
includes("test/camera")
