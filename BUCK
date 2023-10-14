# constants
COMPATIBLE_WITH = [
    "config//os:macos",
    "config//os:linux",
    "config//os:windows",
]

filegroup(
  name = "resources",
  compatible_with=COMPATIBLE_WITH,
  visibility=["PUBLIC"],
  srcs=[
    "resources/boxconfs.dat",
    "resources/daft_punk_get_lucky.mp3",
    "resources/ffdp_wrong_side_of_heaven_cover_abhikalp_unakal.mp3",
  ],
)

cxx_library(
  name = "pepper",
  srcs = [
    "pepper.c",
  ],
  exported_preprocessor_flags=[
    "-DGLFW",
  ],
  link_style = "static",
  exported_headers = [
    "pepper.h",
  ],
  public_include_directories = [
    ".",
    "core",
  ],
  visibility = ["PUBLIC"],
  exported_deps = [
    "//deps:glad",
    "//deps:glfw",
    "//deps:miniaudio",
  ],
)

cxx_binary(
    name="app",
    compatible_with=COMPATIBLE_WITH,
    srcs=[
      "app.c",
    ],
    include_directories=[
      ".",
    ],
    compiler_flags=[
      "-O3",
    ],
    link_style = "static",
    deps=[
      ":pepper",
    ],
    resources=[
      ":resources",
    ],
)

cxx_binary(
    name="test",
    compatible_with=COMPATIBLE_WITH,
    srcs=[
      "tests.c",
    ],
    include_directories=[
      ".",
    ],
    compiler_flags=[
      "-g",
    ],
    link_style = "static",
    deps=[
      ":pepper",
    ],
    resources=[
      ":resources",
    ],
)
