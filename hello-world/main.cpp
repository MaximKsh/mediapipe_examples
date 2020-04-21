#include <iostream>
#include <fstream>
#include "mediapipe/framework/calculator_graph.h"
#include "mediapipe/framework/port/logging.h"
#include "mediapipe/framework/port/parse_text_proto.h"
#include "mediapipe/framework/port/status.h"

mediapipe::Status RunGraph() {
    std::ifstream file("./hello-world/graph.pbtxt");
    std::string graph_file_content;
    graph_file_content.assign(
        std::istreambuf_iterator<char>(file), 
        std::istreambuf_iterator<char>());

    mediapipe::CalculatorGraphConfig config = 
        mediapipe::ParseTextProtoOrDie<mediapipe::CalculatorGraphConfig>(graph_file_content);

    mediapipe::CalculatorGraph graph;
    MP_RETURN_IF_ERROR(graph.Initialize(config));
    ASSIGN_OR_RETURN(mediapipe::OutputStreamPoller poller, graph.AddOutputStreamPoller("out"));

    MP_RETURN_IF_ERROR(graph.StartRun({}));

    auto input_packet = mediapipe::MakePacket<std::string>("Hello!").At(mediapipe::Timestamp(0));
    MP_RETURN_IF_ERROR(graph.AddPacketToInputStream("in", input_packet));

    MP_RETURN_IF_ERROR(graph.CloseInputStream("in"));

    mediapipe::Packet packet;
    while (poller.Next(&packet)) {
        std::cout << packet.Get<std::string>() << std::endl;
    }
    return graph.WaitUntilDone();
}


int main(int argc, char** argv) {
    google::InitGoogleLogging(argv[0]);
    
    CHECK(RunGraph().ok());

    return 0;
}

// bazel-2.0.0 build --define MEDIAPIPE_DISABLE_GPU=1 //hello-world:HelloMediapipe
// ./bazel-bin/hello-world/HelloMediapipe