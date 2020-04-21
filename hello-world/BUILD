load("@mediapipe_repository//mediapipe/framework/port:build_config.bzl", "mediapipe_cc_proto_library")

proto_library(
    name = "repeat_n_times_calculator_proto",
    srcs = ["RepeatNTimesCalculator.proto"],
    visibility = ["//visibility:public"],
    deps = [
        "@mediapipe_repository//mediapipe/framework:calculator_proto",
    ],
)

mediapipe_cc_proto_library(
    name = "repeat_n_times_calculator_cc_proto",
    srcs = ["RepeatNTimesCalculator.proto"],
    cc_deps = [
        "@mediapipe_repository//mediapipe/framework:calculator_cc_proto",
    ],
    visibility = ["//visibility:public"],
    deps = [":repeat_n_times_calculator_proto"],
)

# Правило сборки калькулятора. Указано название, список исходников и зависимости
cc_library(
    name = "repeat_n_times_calculator",
    srcs = ["RepeatNTimesCalculator.cpp"],
    visibility = [
        "//visibility:public",
    ],
    deps = [
        ":repeat_n_times_calculator_cc_proto",
        "@mediapipe_repository//mediapipe/framework:calculator_framework",
        "@mediapipe_repository//mediapipe/framework/port:status",
    ],
    alwayslink = 1,
)

# Правило сборки исполняемого файла, который будет запускать граф.
cc_binary(
    name = "HelloMediapipe",
    srcs = ["main.cpp"],
    deps = [
        "repeat_n_times_calculator",
        "@mediapipe_repository//mediapipe/framework/port:logging",
        "@mediapipe_repository//mediapipe/framework/port:parse_text_proto",
        "@mediapipe_repository//mediapipe/framework/port:status",
    ],
)
