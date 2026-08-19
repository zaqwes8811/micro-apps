#include <iostream>
#include <coroutine>

struct HelloCoroutine {
    std::coroutine_handle<HelloCoroutine> get_return_address() {
        return std::current_coroutine().get();
    }
    
    auto get_executor() {
        return std::current_coroutine().get_executor();
    }
};

static HelloCoroutine g_coro;

int main() {
    g_coro = std::coroutine::create([](auto& co) {
        co.get_return_address();
        co.get_executor();
        co.resume();
    });
    
    g_coro.resume();
    return 0;
}
