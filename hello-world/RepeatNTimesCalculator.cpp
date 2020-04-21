#include "mediapipe/framework/calculator_framework.h"
#include "hello-world/RepeatNTimesCalculator.pb.h"

class RepeatNTimesCalculator : public mediapipe::CalculatorBase {
public:
    static mediapipe::Status GetContract(mediapipe::CalculatorContract* cc) {
        cc->Inputs().Get("", 0).Set<std::string>();
        cc->Outputs().Get("OUTPUT_TAG", 0).Set<std::string>();
        return mediapipe::OkStatus();
    }

    mediapipe::Status Open(mediapipe::CalculatorContext* cc) final {
        const auto& options = cc->Options<mediapipe_demonstration::RepeatNTimesCalculatoOptions>();
        n_ = options.n();
        return mediapipe::OkStatus();
    }

    mediapipe::Status Process(mediapipe::CalculatorContext* cc) final {
        auto txt = cc->Inputs().Index(0).Value().Get<std::string>();

        for (int i = 0; i < n_; ++i) {
            auto packet = mediapipe::MakePacket<std::string>(txt).At(cc->InputTimestamp() + i);
            cc->Outputs().Get("OUTPUT_TAG", 0).AddPacket(packet);
        }
        
        return mediapipe::OkStatus();
    }
private:
    int n_;
};

REGISTER_CALCULATOR(RepeatNTimesCalculator);
