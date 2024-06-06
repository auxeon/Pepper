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
    "sandbox/boxconfs.dat",
    "sandbox/daft_punk_get_lucky.mp3",
    "sandbox/ffdp_wrong_side_of_heaven_cover_abhikalp_unakal.mp3",
  ],
)

cxx_binary(
    name="app",
    compatible_with=COMPATIBLE_WITH,
    srcs=[
      "sandbox/main.c",
      "core/ps_chronon.c",
      "core/ps_events.c",
      "core/ps_window.c",
    ],
    include_directories=[
      ".",
      "sandbox",
      "core",
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
      # "sandbox/boxconfs.dat",
      # "sandbox/daft_punk_get_lucky.mp3",
      # "sandbox/ffdp_wrong_side_of_heaven_cover_abhikalp_unakal.mp3",
    ],
)
