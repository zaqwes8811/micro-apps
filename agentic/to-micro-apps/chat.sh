#!/bin/bash

# URL сервера
URL="http://localhost:8000/v1/chat/completions"
MODEL="TheBloke/deepseek-coder-1.3b-instruct-AWQ"

# Файл для хранения истории
HISTORY_FILE="/tmp/chat_history.json"

# Инициализация истории
echo '[]' > "$HISTORY_FILE"

echo "🤖 Чат-бот с DeepSeek-Coder (localhost:8000)"
echo "Введите 'exit' или 'quit' для выхода"
echo "Введите 'clear' для очистки истории"
echo "----------------------------------------"

while true; do
    echo -n "👤 Вы: "
    read question
    
    case "$question" in
        exit|quit)
            echo "👋 До свидания!"
            break
            ;;
        clear)
            echo '[]' > "$HISTORY_FILE"
            echo "🧹 История очищена!"
            continue
            ;;
        "")
            continue
            ;;
    esac
    
    # Читаем историю
    HISTORY=$(cat "$HISTORY_FILE")
    
    # Создаём временный файл с JSON запросом
    cat > /tmp/chat_request.json <<EOF
{
    "model": "$MODEL",
    "messages": $HISTORY,
    "max_tokens": 500,
    "temperature": 0.1
}
EOF
    
    # Добавляем вопрос к запросу
    cat > /tmp/chat_request_with_user.json <<EOF
{
    "model": "$MODEL",
    "messages": $(echo "$HISTORY" | jq --arg user "$question" '. + [{"role": "user", "content": $user}]'),
    "max_tokens": 500,
    "temperature": 0.1
}
EOF
    
    # Отправляем запрос
    RESPONSE=$(curl -s -X POST "$URL" \
        -H "Content-Type: application/json" \
        -d @/tmp/chat_request_with_user.json)
    
    # Извлекаем ответ
    ANSWER=$(echo "$RESPONSE" | jq -r '.choices[0].message.content // "null"')
    
    if [ -z "$ANSWER" ] || [ "$ANSWER" = "null" ]; then
        echo "❌ Ошибка: Не удалось получить ответ"
        echo "Ответ сервера:"
        echo "$RESPONSE" | jq .
        continue
    fi
    
    echo "🤖 Бот: $ANSWER"
    echo ""
    
    # Обновляем историю
    echo "$HISTORY" | jq --arg user "$question" --arg bot "$ANSWER" '. + [{"role": "user", "content": $user}, {"role": "assistant", "content": $bot}]' > "$HISTORY_FILE"
done

# Удаляем временные файлы
rm -f /tmp/chat_request.json /tmp/chat_request_with_user.json