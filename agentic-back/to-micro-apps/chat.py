#!/usr/bin/env python3
import requests
import json
import re

URL = "http://localhost:8000/v1/chat/completions"
MODEL = "TheBloke/deepseek-coder-1.3b-instruct-AWQ"
HISTORY = []

def clean_text(text):
    # Заменяем специальные символы DeepSeek
    text = text.replace('Ċ', '\n')
    text = text.replace('Ġ', ' ')
    
    # Убираем все управляющие токены
    text = re.sub(r'<\|EOT\|>', '', text)
    text = re.sub(r'<\|.*?\|>', '', text)
    
    # Убираем восклицательные знаки в конце
    text = re.sub(r'!:.*$', '', text)
    text = re.sub(r':!+.*$', '', text)
    text = re.sub(r'!+$', '', text)
    
    # Убираем повторяющиеся :! в конце строки
    text = re.sub(r'(:!\n?)+$', '', text)
    text = re.sub(r'(\n?:!\n?)+$', '', text)
    text = re.sub(r'(\n:!)+\n?$', '', text)
    
    # Убираем лишние пробелы и строки
    text = re.sub(r'\n{3,}', '\n\n', text)
    text = re.sub(r' {2,}', ' ', text)
    
    return text.strip()

print("🤖 Чат-бот с DeepSeek-Coder")
print("Введите 'exit' для выхода, 'clear' для очистки")
print("-" * 50)

while True:
    question = input("👤 Вы: ").strip()
    
    if question.lower() == 'exit':
        print("👋 До свидания!")
        break
    elif question.lower() == 'clear':
        HISTORY = []
        print("🧹 История очищена!")
        continue
    elif not question:
        continue
    
    messages = HISTORY + [{"role": "user", "content": question}]
    
    payload = {
        "model": MODEL,
        "messages": messages,
        "max_tokens": 500,
        "temperature": 0.1,
        "stream": False
    }
    
    try:
        response = requests.post(URL, json=payload)
        data = response.json()
        answer = data['choices'][0]['message']['content']
        
        clean_answer = clean_text(answer)
        
        print(f"🤖 Бот: {clean_answer}\n")
        
        HISTORY.append({"role": "user", "content": question})
        HISTORY.append({"role": "assistant", "content": answer})
        
    except requests.exceptions.ConnectionError:
        print("❌ Ошибка: Сервер vLLM не запущен!")
        break
    except Exception as e:
        print(f"❌ Ошибка: {e}")