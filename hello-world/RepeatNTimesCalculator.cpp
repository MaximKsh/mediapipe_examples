#include "mediapipe/framework/calculator_framework.h"
#include "hello-world/RepeatNTimesCalculator.pb.h"

class RepeatNTimesCalculator : public mediapipe::CalculatorBase {
public:
    static mediapipe::Status GetContract(mediapipe::CalculatorContract* cc) {
        // На вход ожидается поток без тега с пакетами, содержащими строки
        cc->Inputs().Get("", 0).Set<std::string>();
        // Из калькулятора выходит поток с тегом OUTPUT_TAG, в котором пакеты со строками
        cc->Outputs().Get("OUTPUT_TAG", 0).Set<std::string>();
        return mediapipe::OkStatus();
    }

    mediapipe::Status Open(mediapipe::CalculatorContext* cc) final {
        // Загрузка параметров калькулятора, указанных при описании графа
        const auto& options = cc->Options<mediapipe_demonstration::RepeatNTimesCalculatoOptions>();
        // n - количество повторений входного сигнала на выходе
        n_ = options.n();
        return mediapipe::OkStatus();
    }

    mediapipe::Status Process(mediapipe::CalculatorContext* cc) final {
        // Получение текста из входного пакета
        // Из массива входных потоков берется поток без тега с нулевым индексом
        // И из него достается содержимое типа std::string
        auto txt = cc->Inputs().Index(0).Value().Get<std::string>();

        for (int i = 0; i < n_; ++i) {
            // Создание пакета с содержимым из входного
            auto packet = mediapipe::MakePacket<std::string>(txt).At(cc->InputTimestamp() + i);
            // Отправка пакета по потоку с тегом OUTPUT_TAG и индексом 0
            cc->Outputs().Get("OUTPUT_TAG", 0).AddPacket(packet);
        }
        
        return mediapipe::OkStatus();
    }
private:
    int n_;
};
// Макрос для регистрации калькулятора
REGISTER_CALCULATOR(RepeatNTimesCalculator);
