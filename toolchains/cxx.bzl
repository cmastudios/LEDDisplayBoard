load(
    "@prelude//cxx:cxx_toolchain_types.bzl",
    "BinaryUtilitiesInfo",
    "CCompilerInfo",
    "CvtresCompilerInfo",
    "CxxCompilerInfo",
    "CxxInternalTools",
    "CxxPlatformInfo",
    "CxxToolchainInfo",
    "LinkerInfo",
    "LinkerType",
    "PicBehavior",
    "RcCompilerInfo",
    "ShlibInterfacesMode",
)
load("@prelude//cxx:headers.bzl", "HeaderMode")
load("@prelude//cxx:linker.bzl", "is_pdb_generated")
load("@prelude//decls:common.bzl", "buck")
load("@prelude//linking:link_info.bzl", "LinkOrdering", "LinkStyle")
load("@prelude//linking:lto.bzl", "LtoMode")
load("@prelude//os_lookup:defs.bzl", "OsLookup")
CxxToolsInfo = provider(
    fields = {
        "archiver": provider_field(typing.Any, default = None),
        "archiver_type": provider_field(typing.Any, default = None),
        "asm_compiler": provider_field(typing.Any, default = None),
        "asm_compiler_type": provider_field(typing.Any, default = None),
        "compiler": provider_field(typing.Any, default = None),
        "compiler_type": provider_field(typing.Any, default = None),
        "cvtres_compiler": provider_field(typing.Any, default = None),
        "cxx_compiler": provider_field(typing.Any, default = None),
        "linker": provider_field(typing.Any, default = None),
        "linker_type": LinkerType,
        "rc_compiler": provider_field(typing.Any, default = None),
    },
)
def _cxx_tools_info_toolchain_impl(ctx: AnalysisContext):
    return _cxx_toolchain_from_cxx_tools_info(ctx, ctx.attrs.cxx_tools_info[CxxToolsInfo])

def _cxx_toolchain_from_cxx_tools_info(ctx: AnalysisContext, cxx_tools_info: CxxToolsInfo, target_name = "x86_64"):
    os = ctx.attrs._target_os_type[OsLookup].platform
    archiver_supports_argfiles = os != "macos"
    additional_linker_flags = []


    binary_extension = ""
    object_file_extension = "o"
    static_library_extension = "a"
    shared_library_name_default_prefix = "lib"
    shared_library_name_format = "{}.so"
    shared_library_versioned_name_format = "{}.so.{}"


    linker_type = LinkerType("gnu")
    pic_behavior = PicBehavior("supported")

    if cxx_tools_info.compiler_type == "clang":
        llvm_link = RunInfo(args = ["llvm-link"])
    else:
        llvm_link = None

    return [
        DefaultInfo(),
        CxxToolchainInfo(
            internal_tools = ctx.attrs._internal_tools[CxxInternalTools],
            linker_info = LinkerInfo(
                linker = _run_info(cxx_tools_info.linker),
                linker_flags = additional_linker_flags + ctx.attrs.link_flags,
                post_linker_flags = ctx.attrs.post_link_flags,
                archiver = _run_info(cxx_tools_info.archiver),
                archiver_type = cxx_tools_info.archiver_type,
                archiver_supports_argfiles = archiver_supports_argfiles,
                generate_linker_maps = True,
                lto_mode = LtoMode("none"),
                type = linker_type,
                link_binaries_locally = True,
                link_libraries_locally = True,
                archive_objects_locally = True,
                use_archiver_flags = True,
                static_dep_runtime_ld_flags = [],
                static_pic_dep_runtime_ld_flags = [],
                shared_dep_runtime_ld_flags = [],
                independent_shlib_interface_linker_flags = [],
                shlib_interfaces = ShlibInterfacesMode("disabled"),
                link_style = LinkStyle(ctx.attrs.link_style),
                link_weight = 1,
                binary_extension = binary_extension,
                object_file_extension = object_file_extension,
                shared_library_name_default_prefix = shared_library_name_default_prefix,
                shared_library_name_format = shared_library_name_format,
                shared_library_versioned_name_format = shared_library_versioned_name_format,
                static_library_extension = static_library_extension,
                force_full_hybrid_if_capable = False,
                is_pdb_generated = is_pdb_generated(linker_type, ctx.attrs.link_flags),
                link_ordering = ctx.attrs.link_ordering,
            ),
            bolt_enabled = False,
            binary_utilities_info = BinaryUtilitiesInfo(
                nm = RunInfo(args = ["nm"]),
                objcopy = RunInfo(args = ["objcopy"]),
                objdump = RunInfo(args = ["objdump"]),
                ranlib = RunInfo(args = ["ranlib"]),
                strip = RunInfo(args = ["strip"]),
                dwp = None,
                bolt_msdk = None,
            ),
            cxx_compiler_info = CxxCompilerInfo(
                compiler = _run_info(cxx_tools_info.cxx_compiler),
                preprocessor_flags = [],
                compiler_flags = ctx.attrs.cxx_flags,
                compiler_type = cxx_tools_info.compiler_type,
            ),
            c_compiler_info = CCompilerInfo(
                compiler = _run_info(cxx_tools_info.compiler),
                preprocessor_flags = [],
                compiler_flags = ctx.attrs.c_flags,
                compiler_type = cxx_tools_info.compiler_type,
            ),
            as_compiler_info = CCompilerInfo(
                compiler = _run_info(cxx_tools_info.compiler),
                compiler_type = cxx_tools_info.compiler_type,
            ),
            asm_compiler_info = CCompilerInfo(
                compiler = _run_info(cxx_tools_info.asm_compiler),
                compiler_type = cxx_tools_info.asm_compiler_type,
            ),
            cvtres_compiler_info = CvtresCompilerInfo(
                compiler = _run_info(cxx_tools_info.cvtres_compiler),
                preprocessor_flags = [],
                compiler_flags = ctx.attrs.cvtres_flags,
                compiler_type = cxx_tools_info.compiler_type,
            ),
            rc_compiler_info = RcCompilerInfo(
                compiler = _run_info(cxx_tools_info.rc_compiler),
                preprocessor_flags = [],
                compiler_flags = ctx.attrs.rc_flags,
                compiler_type = cxx_tools_info.compiler_type,
            ),
            header_mode = HeaderMode("symlink_tree_only"),
            cpp_dep_tracking_mode = ctx.attrs.cpp_dep_tracking_mode,
            pic_behavior = pic_behavior,
            llvm_link = llvm_link,
        ),
        CxxPlatformInfo(name = target_name),
    ]

def _run_info(args):
    return None if args == None else RunInfo(args = [args])

cxx_tools_info_toolchain = rule(
    impl = _cxx_tools_info_toolchain_impl,
    attrs = {
        "c_flags": attrs.list(attrs.string(), default = []),
        "cpp_dep_tracking_mode": attrs.string(default = "makefile"),
        "cvtres_flags": attrs.list(attrs.string(), default = []),
        "cxx_flags": attrs.list(attrs.string(), default = []),
        "cxx_tools_info": attrs.exec_dep(providers = [CxxToolsInfo], default = select({
            "DEFAULT": "prelude//toolchains/cxx/clang:path_clang_tools",
            "config//os:windows": "prelude//toolchains/msvc:msvc_tools",
        })),
        "link_flags": attrs.list(attrs.string(), default = []),
        "link_ordering": attrs.option(attrs.enum(LinkOrdering.values()), default = None),
        "link_style": attrs.enum(
            LinkStyle.values(),
            default = "shared",
            doc = """
            The default value of the `link_style` attribute for rules that use this toolchain.
            """,
        ),
        "post_link_flags": attrs.list(attrs.string(), default = []),
        "rc_flags": attrs.list(attrs.string(), default = []),
        "_internal_tools": attrs.default_only(attrs.exec_dep(providers = [CxxInternalTools], default = "prelude//cxx/tools:internal_tools")),
        "_target_os_type": buck.target_os_type_arg(),
    },
    is_toolchain_rule = True,
)