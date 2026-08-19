from llama_cpp import Llama

# Q4_K_M GGUF (официальный репозиторий Microsoft)
llm = Llama.from_pretrained(
    repo_id="microsoft/Phi-3-mini-4k-instruct-gguf",
    filename="Phi-3-mini-4k-instruct-q4.gguf",
    n_ctx=4096,
    n_gpu_layers=-1,  # -1 = все слои на GPU; 0 = только CPU
    verbose=False,
)

response = llm.create_chat_completion(
    messages=[
        {"role": "user", "content": "Create a funny joke about chickens."}
    ],
    temperature=0.7,
    max_tokens=256,
)
print(response["choices"][0]["message"]["content"])