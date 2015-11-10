#include "ssc/websocket/WebSocketServer.hpp"
#include <ssc/text_file_reader.hpp>
#include "SimServer.hpp"
#include <sstream>
#include <ssc/macros.hpp>
#include TR1INC(memory)
#include "InputData.hpp"
#include "utilities_for_InputData.hpp"



//#include <unistd.h> // usleep

using namespace ssc::websocket;

#define ADDRESS "127.0.0.1"
#define WEBSOCKET_ADDRESS "ws://" ADDRESS
#define WEBSOCKET_PORT    1234


struct SimulationMessage : public MessageHandler
{
    SimulationMessage(const TR1(shared_ptr)<SimServer>& simserver) : sim_server(simserver)
    {
    }
    void operator()(const Message& msg)
    {
        COUT(msg.get_payload());
        const std::string input_yaml = msg.get_payload();
        const std::string output_yaml = sim_server->play_one_step(input_yaml);
        msg.send_text(output_yaml);
        COUT("réponse serveur :"<<output_yaml)
        COUT("message !");
    }

    private: TR1(shared_ptr)<SimServer> sim_server;
};

void start_server(const InputData& input_data);
void start_server(const InputData& input_data)
{
    const ssc::text_file_reader::TextFileReader yaml_reader(input_data.yaml_filenames);
    const auto yaml = yaml_reader.get_contents();
    TR1(shared_ptr)<SimServer> sim_server (new SimServer(yaml, input_data.solver, input_data.initial_timestep));
    SimulationMessage handler(sim_server);

    new ssc::websocket::Server(handler, ADDRESS, WEBSOCKET_PORT);
    usleep(1000000);
}

int main(int argc, char** argv)
{
    InputData input_data;
    if (argc==1) return display_help(argv[0], input_data);
    const int error = get_input_data(argc, argv, input_data);
    if (error)
    {
        std::cerr <<"A problem occurred while parsing inputs"<<std::endl;
        return error;
    }
    if (input_data.empty()) return EXIT_SUCCESS;
    start_server(input_data);

    return error;
}
