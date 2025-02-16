load("@prelude//cxx:cxx_toolchain_types.bzl", "LinkerType")
load("cxx.bzl", "CxxToolsInfo")


def _path_gnu_tools_impl(ctx) -> list[Provider]:
    return [
        DefaultInfo(),
        CxxToolsInfo(
            compiler = ctx.attrs.prefix + "gcc",
            compiler_type = "gcc",
            cxx_compiler = ctx.attrs.prefix + "g++",
            asm_compiler = ctx.attrs.prefix + "gcc",
            asm_compiler_type = "gcc",
            rc_compiler = None,
            cvtres_compiler = None,
            archiver = ctx.attrs.prefix + "ar",
            archiver_type = "gnu",
            linker = ctx.attrs.prefix + "g++",
            linker_type = LinkerType("gnu"),
        ),
    ]

path_gnu_tools = rule(
    impl = _path_gnu_tools_impl,
    attrs = {
        "prefix": attrs.string(),
    },
)