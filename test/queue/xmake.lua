target("queue_test")
add_includedirs("$(projectdir)/3rdparty/minunit")
add_tests("queue_test")
set_default("false")
add_deps("cbox_queue")
set_kind("binary")
add_files("*.c")
