target("cbox_camera")
add_deps("cbox_array", "cbox_queue")
set_kind("shared")

add_defines("CBOX_CAMERA_ENABLE_LOG")

add_includedirs(".")
add_includedirs("include", { public = true })
add_files("*.c", "v4l2/*.c")
