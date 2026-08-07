cat > /home/zaqwes/My/micro-apps/agentic/small/cpp_coro/test_simple.cpp << 'EOF'
#include <coroutine>

struct HelloCoroutine {
    type get_return_address() {
        return std::current_coroutine().get();
    }

    auto get_executor() {
        return std::current_coroutine().get_executor();
    }
};

int main() {
    static HelloCoroutine g_coro;
    
    g_coro = std::coroutine::create([](auto& co) {
        co.get_return_address();
        co.get_executor();
        co.resume();
    });
    
    g_coro.resume();
    return 0;
}
EOF
