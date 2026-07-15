#!/usr/bin/env python3
"""
Харнес как local-cli — перехватывает JSON и выполняет инструменты
"""

from ollama import chat
import json
import subprocess
import os
import glob as glob_module
import re

MODEL = "qwen2.5-coder:3b-instruct-q4_K_M-16k"

# ============ ИНСТРУМЕНТЫ ============
def tool_glob(pattern):
    try:
        files = glob_module.glob(pattern, recursive=True)
        return "\n".join(files) if files else "Файлы не найдены"
    except Exception as e:
        return f"Ошибка: {e}"

def tool_read(file_path):
    try:
        with open(file_path, 'r') as f:
            content = f.read()
            if len(content) > 500:
                return content[:500] + f"\n... (всего {len(content)} символов)"
            return content
    except Exception as e:
        return f"Ошибка: {e}"

def tool_write(file_path, content):
    try:
        with open(file_path, 'w') as f:
            f.write(content)
        return f"✅ Файл {file_path} записан ({len(content)} символов)"
    except Exception as e:
        return f"Ошибка: {e}"

def tool_edit(file_path, old_text, new_text):
    try:
        with open(file_path, 'r') as f:
            content = f.read()
        if old_text not in content:
            return f"❌ Текст не найден"
        new_content = content.replace(old_text, new_text)
        with open(file_path, 'w') as f:
            f.write(new_content)
        return f"✅ Файл {file_path} отредактирован"
    except Exception as e:
        return f"Ошибка: {e}"

def tool_bash(command):
    try:
        result = subprocess.run(command, shell=True, capture_output=True, text=True)
        output = result.stdout + result.stderr
        return output[:1000] if output else "✅ Команда выполнена (нет вывода)"
    except Exception as e:
        return f"Ошибка: {e}"

TOOL_IMPL = {
    "glob": tool_glob,
    "read": tool_read,
    "write": tool_write,
    "edit": tool_edit,
    "bash": tool_bash
}

def extract_json(text):
    """Извлекает JSON из текста"""
    match = re.search(r'\{[^{}]*\}', text, re.DOTALL)
    if match:
        try:
            return json.loads(match.group())
        except:
            return None
    return None

# ============ ОСНОВНОЙ ЦИКЛ ============
print("=" * 70)
print("🧪 ХАРНЕС КАК LOCAL-CLI")
print("=" * 70)
print(f"📦 Модель: {MODEL}")
print(f"🔧 Инструменты: {', '.join(TOOL_IMPL.keys())}")
print("=" * 70)

messages = []

while True:
    user_input = input("\n👤 Вы: ")
    if user_input.lower() in ['exit', 'quit', 'q']:
        break
    
    messages.append({'role': 'user', 'content': user_input})
    
    print("\n🤖 Модель думает...", end="", flush=True)
    
    # Получаем ответ от модели
    response = chat(
        model=MODEL,
        messages=messages
    )
    
    reply = response['message']['content']
    print("\n" + "=" * 70)
    
    # Пытаемся извлечь JSON
    tool_call = extract_json(reply)
    
    if tool_call and 'name' in tool_call:
        tool_name = tool_call['name']
        tool_args = tool_call.get('arguments', {})
        
        print(f"🔧 ВЫЗОВ: {tool_name}")
        print(f"📋 Аргументы: {json.dumps(tool_args, ensure_ascii=False)}")
        
        # Выполняем инструмент
        if tool_name in TOOL_IMPL:
            result = TOOL_IMPL[tool_name](**tool_args)
            print(f"📄 Результат:\n{result}")
            
            # Формируем сообщение с результатом
            tool_result_message = {
                'role': 'user',
                'content': f"Результат выполнения {tool_name}:\n{result}\n\nТеперь ответь пользователю на основе этого результата."
            }
            messages.append(tool_result_message)
            
            # Получаем финальный ответ
            print("\n🤖 Формирую финальный ответ...", end="", flush=True)
            final_response = chat(
                model=MODEL,
                messages=messages
            )
            
            print("\n" + "=" * 70)
            print("🤖 ОТВЕТ:")
            print("=" * 70)
            print(final_response['message']['content'])
            messages.append({'role': 'assistant', 'content': final_response['message']['content']})
        else:
            print(f"❌ Инструмент '{tool_name}' не найден")
            messages.append({'role': 'assistant', 'content': reply})
    else:
        # Обычный текстовый ответ
        print("🤖 ОТВЕТ:")
        print("=" * 70)
        print(reply)
        messages.append({'role': 'assistant', 'content': reply})

print("\n👋 Пока!")