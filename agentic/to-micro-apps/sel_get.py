from selenium import webdriver
from selenium.webdriver.chrome.options import Options
import time

# Настройка драйвера
options = Options()
#options.add_argument('--headless')  # Фоновый режим
driver = webdriver.Chrome(options=options)

# Открываем страницу
driver.get("https://www.gate.com/docs/developers/apiv4/en/futures/")
time.sleep(5)  # Ждём загрузки JavaScript

# Получаем HTML
html = driver.page_source
driver.quit()

# Сохраняем
with open("gate_futures.html", "w", encoding="utf-8") as f:
    f.write(html)

print("✅ Страница сохранена как gate_futures.html")