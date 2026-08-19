

1. Python

```
# Виртуальное окружение (Python 3.10)
python3.10 -m venv .venv
source .venv/bin/activate
pip install -r requirements.txt

# Модель: microsoft/Phi-3-mini-4k-instruct, Q4_K_M (4-bit, ~2.2 GB)
# GGUF: microsoft/Phi-3-mini-4k-instruct-gguf
huggingface-cli download microsoft/Phi-3-mini-4k-instruct-gguf \
  Phi-3-mini-4k-instruct-q4.gguf \
  --local-dir ./models \
  --local-dir-use-symlinks False
```

```
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
        {"role": "user", "content": "Explain quantum computing in one sentence."}
    ],
    temperature=0.7,
    max_tokens=256,
)
print(response["choices"][0]["message"]["content"])
```