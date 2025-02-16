# A list of available rules and their signatures can be found here: https://buck2.build/docs/prelude/globals/


load("@prelude//platforms:defs.bzl", "execution_platform", "host_configuration")

execution_platform(
    name = "embedded",
    cpu_configuration = "@prelude//cpu:arm32",
    os_configuration = "@prelude//os:none",
    use_windows_path_separators = host_info().os.is_windows,
    visibility = ["PUBLIC"],
)


configured_alias(
    name = "boot",
    actual = "//Software/boot:app_hex",
    platform = ":embedded",
    visibility = ["PUBLIC"],
)