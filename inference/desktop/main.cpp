#include <iostream>
#include <fstream>
#include <memory>
#include "mediapipe/framework/calculator_graph.h"
#include "mediapipe/framework/port/logging.h"
#include "mediapipe/framework/port/parse_text_proto.h"
#include "mediapipe/framework/port/status.h"
#include "mediapipe/framework/port/opencv_imgcodecs_inc.h"
#include "mediapipe/framework/formats/image_frame.h"
#include "mediapipe/framework/formats/image_frame_opencv.h"

mediapipe::Status RunGraph() {
    std::ifstream file("./inference/graph.pbtxt");
    std::string graph_file_content;
    graph_file_content.assign(
        std::istreambuf_iterator<char>(file), 
        std::istreambuf_iterator<char>());

    mediapipe::CalculatorGraphConfig config = 
        mediapipe::ParseTextProtoOrDie<mediapipe::CalculatorGraphConfig>(graph_file_content);

    
    auto img_mat = cv::imread("./inference/img.jpg");

    // Wrap Mat into an ImageFrame.
    auto input_frame = std::make_unique<mediapipe::ImageFrame>(
        mediapipe::ImageFormat::SRGB, img_mat.cols, img_mat.rows,
        mediapipe::ImageFrame::kDefaultAlignmentBoundary);
    cv::Mat input_frame_mat = mediapipe::formats::MatView(input_frame.get());
    img_mat.copyTo(input_frame_mat);

    mediapipe::CalculatorGraph graph;
    MP_RETURN_IF_ERROR(graph.Initialize(config));
    ASSIGN_OR_RETURN(mediapipe::OutputStreamPoller poller, graph.AddOutputStreamPoller("out"));

    MP_RETURN_IF_ERROR(graph.StartRun({}));

    auto frame = mediapipe::Adopt(input_frame.release()).At(mediapipe::Timestamp(0));
    MP_RETURN_IF_ERROR(graph.AddPacketToInputStream("in", frame));

    MP_RETURN_IF_ERROR(graph.CloseInputStream("in"));

    mediapipe::Packet packet;
    while (poller.Next(&packet)) {
        auto predictions = packet.Get<std::vector<float>>();
        int idx = std::max_element(predictions.begin(), predictions.end()) - predictions.begin();
        std::cout << idx << std::endl;
        
    }
    return graph.WaitUntilDone();


    return mediapipe::OkStatus();
}


int main(int argc, char** argv) {
    google::InitGoogleLogging(argv[0]);
    google::InstallFailureSignalHandler();
    FLAGS_logtostderr = true;
    
    mediapipe::Status run_status = RunGraph();
    if (!run_status.ok()) {
        LOG(ERROR) << "Failed to run the graph: " << run_status.message();
    } else {
        LOG(INFO) << "Success!";
    }

    return 0;
}

// bazel-2.0.0 build --define MEDIAPIPE_DISABLE_GPU=1 //inference/desktop:Inference
// ./bazel-bin/inference/desktop/Inference