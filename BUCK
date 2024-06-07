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

cxx_binary(
    name="app",
    compatible_with=COMPATIBLE_WITH,
    srcs=[
      "tests.c",
      "ps_chronon.c",
      "ps_events.c",
      "ps_window.c",
      "pch.c",
    ],
    include_directories=[
      ".",
      "deps",
    ],
    compiler_flags=[
      "-g",
    ],
    link_style = "static",
    deps=[
      "//deps:glad",
      "//deps:glfw",
      "//deps:miniaudio",
    ],
    resources=[
      ":resources",
    ],
)
