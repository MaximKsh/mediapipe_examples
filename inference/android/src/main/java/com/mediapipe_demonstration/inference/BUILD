package(default_visibility = ["//visibility:public"])

load("@io_bazel_rules_kotlin//kotlin:kotlin.bzl", "kt_android_library")

cc_binary(
    name = "libmediapipe_jni.so",
    linkshared = 1,
    deps = [
        "//inference:mobile_calculators",
        "@mediapipe_repository//mediapipe/java/com/google/mediapipe/framework/jni:mediapipe_framework_jni",
    ],
)

cc_library(
    name = "mediapipe_jni_lib",
    srcs = [":libmediapipe_jni.so"],
    alwayslink = 1,
)

genrule(
    name = "copy_binary_graph",
    srcs = ["//inference:mobile_binary_graph"],
    outs = ["mobile_binary_graph.binarypb"],
    cmd = "cp $< $@",
)

kt_android_library(
    name = "lib",
    srcs = glob(["*.kt"]),
    assets = [
        ":copy_binary_graph",
        "//inference:mobilenetv2_imagenet.tflite",
    ],
    assets_dir = "",
    custom_package = "com.mediapipe_demonstration.inference",
    manifest = "AndroidManifest.xml",
    resource_files = glob(["res/**"]),
    visibility = ["//visibility:public"],
    exports = [
        ":mediapipe_jni_lib",
        "@mediapipe_repository//third_party:opencv",
    ],
    deps = [
        "@maven//:androidx_concurrent_concurrent_futures",
        "@maven//:com_google_guava_guava",
        "@maven//:androidx_appcompat_appcompat",
        "@maven//:androidx_core_core",
        "@maven//:androidx_core_core_ktx",
        "@maven//:androidx_drawerlayout_drawerlayout",
        "@maven//:androidx_fragment_fragment",
        "@maven//:androidx_lifecycle_lifecycle_common",
        "@maven//:androidx_lifecycle_lifecycle_viewmodel",
        "@mediapipe_repository//mediapipe/java/com/google/mediapipe/components:android_components",
        "@mediapipe_repository//mediapipe/java/com/google/mediapipe/framework:android_framework",
        "@mediapipe_repository//mediapipe/java/com/google/mediapipe/glutil",
        "@mediapipe_repository//third_party:androidx_constraint_layout",
    ],
)

android_binary(
    name = "Inference",
    manifest = "AndroidManifest.xml",
    deps = [":lib"],
)
