load(
    "@prelude//python:toolchain.bzl",
    "PythonToolchainInfo",
)

def _build_images_c_impl(ctx):
    output_c = ctx.actions.declare_output(ctx.label.name + ".c")
    output_h = ctx.actions.declare_output(ctx.label.name + ".h")

    cmd = cmd_args([
        ctx.attrs.toolchain[PythonToolchainInfo].interpreter,
        ctx.attrs.script,
        output_c.as_output(),
        output_h.as_output(),
        ctx.attrs.sources,
    ])
    ctx.actions.run(cmd, category = "compile")

    return [
        DefaultInfo(default_output = output_c, sub_targets = {"h": [DefaultInfo(default_output = output_h)]}),
        RunInfo(args = cmd_args(output_c)),
    ]

build_images_c = rule(
    impl = _build_images_c_impl,
    attrs = {
        "script": attrs.source(),
        "sources": attrs.list(attrs.source(), default=[]),
        "toolchain": attrs.toolchain_dep(default="toolchains//:python"),
    },
)