target("cbox_camera")
add_deps("cbox_array")
set_kind("shared")

add_defines("CBOX_CAMERA_ENABLE_LOG")

add_includedirs(".")
add_includedirs("include", { public = true })
add_files("*.c", "v4l2/*.c")
