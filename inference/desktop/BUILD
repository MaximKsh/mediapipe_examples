# Правило сборки исполняемого файла, который будет запускать граф.
cc_binary(
    name = "Inference",
    srcs = ["main.cpp"],
    copts = [
        "-DENABLE_PRECOMPILED_HEADERS=OFF",
    ],
    deps = [
        "@mediapipe_repository//mediapipe/calculators/image:image_transformation_calculator",
        "@mediapipe_repository//mediapipe/calculators/tflite:tflite_converter_calculator",
        "@mediapipe_repository//mediapipe/calculators/tflite:tflite_inference_calculator",
        "@mediapipe_repository//mediapipe/calculators/tflite:tflite_tensors_to_floats_calculator",
        "@mediapipe_repository//mediapipe/framework/port:logging",
        "@mediapipe_repository//mediapipe/framework/port:parse_text_proto",
        "@mediapipe_repository//mediapipe/framework/port:status",
        "@mediapipe_repository//mediapipe/framework/port:opencv_imgcodecs",
    ],
)
