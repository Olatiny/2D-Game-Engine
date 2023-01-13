#include "../server_src/includes.hpp"
#include "../server_src/character.hpp"
#include "../server_src/platform.hpp"

int main(void) {
    zmq::context_t context(1);
    zmq::socket_t sock(context, zmq::socket_type::req);
    sock.connect("tcp://localhost:1337");

    Character c(Vector2f(15, 15), Color(255, 255, 255), 100, 0, 0.08, false, false, false);

    nlohmann::json j = c.toJson();

    sock.send(zmq::buffer(j.dump()));

    printf("sent\n");

    zmq::message_t m;
    sock.recv(m);

    int id = std::stoi(m.to_string());

    printf("given id: %d\n", id);

    std::stringstream ss;
    ss << "tcp://*:" << std::setw(4) << std::setfill('1') << id;
    std::string connection = ss.str();
    std::cout << connection << std::endl;
    zmq::socket_t pub(context, zmq::socket_type::pub);
    pub.bind(connection);

    c.setPosition(Vector2f(100, 20 + id * 10));

    zmq::socket_t subscriber(context, zmq::socket_type::sub);
    const char * val = "";
    subscriber.connect("tcp://localhost:6969");
    subscriber.setsockopt(ZMQ_SUBSCRIBE, val, strlen(val));

    while (true) {
        zmq::message_t m2;
        subscriber.recv(m2);

        nlohmann::json j = nlohmann::json::parse(m2.to_string());

        std::cout << j["players"][0] << std::endl;
        // std::cout << nlohmann::json::parse(m2.to_string()).dump() << std::endl;

        std::this_thread::sleep_for(std::chrono::seconds(1));
        c.move(Vector2f(15, 0));

        nlohmann::json sData;
        sData["id"] = id;
        sData["player"] = c.toJson();

        // std::cout << "sending: \n" << sData.dump() << std::endl;

        pub.send(zmq::buffer(sData.dump()));
        // sock.recv(m, zmq::recv_flags::none);

        printf("sent update\n");
    }
}