#include <iostream>
#include <fstream>
#include "mediapipe/framework/calculator_graph.h"
#include "mediapipe/framework/port/logging.h"
#include "mediapipe/framework/port/parse_text_proto.h"
#include "mediapipe/framework/port/status.h"

mediapipe::Status RunGraph() {
    // Загрузка графа из файла
    std::ifstream file("./hello-world/graph.pbtxt");
    std::string graph_file_content;
    graph_file_content.assign(
        std::istreambuf_iterator<char>(file), 
        std::istreambuf_iterator<char>());
    mediapipe::CalculatorGraphConfig config = 
        mediapipe::ParseTextProtoOrDie<mediapipe::CalculatorGraphConfig>(graph_file_content);
    // Инициализация графа
    mediapipe::CalculatorGraph graph;
    MP_RETURN_IF_ERROR(graph.Initialize(config));
    // Подписка на выходной поток
    ASSIGN_OR_RETURN(mediapipe::OutputStreamPoller poller, graph.AddOutputStreamPoller("out"));
    // Запуск графа
    MP_RETURN_IF_ERROR(graph.StartRun({}));
    // Отправка пакета на вход и закрытие входного потока
    auto input_packet = mediapipe::MakePacket<std::string>("Hello!").At(mediapipe::Timestamp(0));
    MP_RETURN_IF_ERROR(graph.AddPacketToInputStream("in", input_packet));
    MP_RETURN_IF_ERROR(graph.CloseInputStream("in"));
    // Получение пакета на выходе
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