#include <iostream>
#include <fstream>
#include <queue> // очередь
#include <unordered_map> // хеш таблица
#include <thread>
#include <cmath>
#include <functional>
#include <mutex>

template<typename T>
T fun_sin(T arg){
    return std::sin(arg);
}

template<typename T>
T fun_sqrt(T arg){
    return std::sqrt(arg);
}

template<typename T>
T fun_pow(T x, T y){
    return std::pow(x, y);
}


template<typename T>
class Task{
    public:
    size_t id;
    std::function<T()> func;
};


template<typename T>
class Server{
    public:

    std::queue<Task<T>> quence;
    std::unordered_map<size_t, T> h_map;

    std::thread server_th;
    std::mutex mtx;
    
    bool running = false;

    size_t id_next = 0;

    public:

    size_t add_task(std::function<T()> f){
        std::lock_guard<std::mutex> lock(mtx);

        Task<T> task;

        task.id = id_next;
        id_next++;

        task.func = f;

        quence.push(task);
        return task.id;
    }

    void start(){
        Task<T> task;

        
        while(running){
            
            bool has_task = false;
            {
                std::lock_guard<std::mutex> lock(mtx);

                if (!quence.empty()){
                    task = quence.front();
                    quence.pop();
                    has_task = true;
                }   
            }

            if (!has_task){
                std::this_thread::sleep_for(std::chrono::milliseconds(1));
                continue;
            }

            T result = task.func(); ////////////

            {
                std::lock_guard<std::mutex> lock(mtx);

                h_map[task.id] = result;
            }
            
        }
    }

    void stop(){
        std::lock_guard<std::mutex> lock(mtx);
        running = false;
    }   

    T request_result(size_t id){

        while(true){

            {
                std::lock_guard<std::mutex> lock(mtx);

                auto it = h_map.find(id);

                if (it != h_map.end()){
                    return it->second;
                }
            }

            std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }
    }
};

void client_sin(Server<double>& server, int N) {

    std::ofstream fout("sin.txt");
    fout << "x" << " " << "result" << "\n";

    for (int i = 0; i < N; i++) {

        // генерим данные
        double x = rand() % 100;

        // отправляем задачу
        size_t id = server.add_task([x]() {
            return std::sin(x);
        });

        // ждём результат
        double result = server.request_result(id);

        // пишем в файл
        fout << x << " " << result << "\n";
    }
}

void client_sqrt(Server<double>& server, int N) {

    std::ofstream fout("sqrt.txt");
    fout << "x" << " " << "result" << "\n";

    for (int i = 0; i < N; i++) {

        double x = rand() % 100;

        size_t id = server.add_task([x]() {
            return std::sqrt(x);
        });

        double result = server.request_result(id);

        fout << x << " " << result << "\n";
    }
}

void client_pow(Server<double>& server, int N) {

    std::ofstream fout("pow.txt");
    fout << "x" << " " << "y" << " " << "result" << "\n";

    for (int i = 0; i < N; i++) {

        double x = rand() % 30;
        double y = rand() % 5;

        size_t id = server.add_task([x, y]() {
            return std::pow(x, y);
        });

        double result = server.request_result(id);
        
        fout << x << " " << y << " " << result << "\n";
    }
}

int main() {

    Server<double> server;

    server.running = true;
    server.server_th = std::thread(&Server<double>::start, &server);

    int N = 10000;

    std::thread t1(client_sin, std::ref(server), N);
    std::thread t2(client_sqrt, std::ref(server), N);
    std::thread t3(client_pow, std::ref(server), N);

    t1.join();
    t2.join();
    t3.join();

    server.stop();
    server.server_th.join();

    return 0;
}
