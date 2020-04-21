package(default_visibility = ["//visibility:public"])

load("//mediapipe/framework/tool:mediapipe_graph.bzl", "mediapipe_binary_graph")

cc_library(
    name = "mobile_calculators",
    deps = [        
        "@mediapipe_repository//mediapipe/calculators/image:image_transformation_calculator",
        "@mediapipe_repository//mediapipe/calculators/tflite:tflite_converter_calculator",
        "@mediapipe_repository//mediapipe/calculators/tflite:tflite_inference_calculator",
        "@mediapipe_repository//mediapipe/calculators/tflite:tflite_tensors_to_floats_calculator",
    ],
)

mediapipe_binary_graph(
    name = "mobile_binary_graph",
    graph = "graph.pbtxt",
    output_name = "mobile.binarypb",
    deps = [":mobile_calculators"],
)

exports_files(
    srcs = ["mobilenetv2_imagenet.tflite"],
)
