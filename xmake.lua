add_rules("mode.debug", "mode.release", "mode.check")

add_rules("plugin.compile_commands.autoupdate", {outputdir = "$(projectdir)"})


add_cflags("-Wall", "-Werror", "-Wextra")

includes("camera", "test/camera")

includes("array", "test/array")

includes("queue", "test/queue")

