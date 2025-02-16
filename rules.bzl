
load(
    "@prelude//cxx:cxx_toolchain_types.bzl",
    "CxxToolchainInfo",
    "BinaryUtilitiesInfo",
    "CxxCompilerInfo",
)


def _gnu_hex_impl(ctx):
    output_hex = ctx.actions.declare_output(ctx.label.name + ".hex")
    cmd = cmd_args([
        ctx.attrs.toolchain[CxxToolchainInfo].binary_utilities_info.objcopy,
        "-O",
        "ihex",
        ctx.attrs.elf,
        output_hex.as_output(),
    ])
    ctx.actions.run(cmd, category = "compile")

    return [
        DefaultInfo(default_output = output_hex),
        RunInfo(args = cmd_args(output_hex)),
    ]

gnu_hex = rule(
    impl = _gnu_hex_impl,
    attrs = {
        "deps": attrs.list(attrs.dep(), default=[]),
        "elf": attrs.source(),
        "toolchain": attrs.toolchain_dep(default="toolchains//:cxx"),
    }
)