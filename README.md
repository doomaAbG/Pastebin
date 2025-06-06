📝 Pastebin на C++
Это простой Pastebin-клон, написанный на C++ с использованием современных библиотек. 
Приложение позволяет сохранять текстовые сообщения локально через веб-интерфейс. В первом релизе сообщения сохранялись в .txt файлах, 
а во втором релизе реализована полноценная работа с базой данных PostgreSQL.

🚀 Возможности
Веб-интерфейс на базе Crow (легковесный C++ веб-фреймворк)

Поддержка Boost, Asio и libpqxx для работы с сетью и PostgreSQL

Два режима хранения сообщений:

📄 Первый релиз: сообщения сохраняются в .txt файлы на локальной машине

🗄️ Второй релиз: сообщения сохраняются в базу данных PostgreSQL с полями:

id (автоинкремент)

timestamp (дата и время)

message (текст сообщения)

📦 Используемые библиотеки
Crow — HTTP сервер

Boost — работа со строками и временем

Asio — асинхронная работа с сетью

libpqxx — клиент для PostgreSQL

vcpkg — управление зависимостями

🛠️ Установка и запуск
Установите зависимости:

Убедитесь, что установлен vcpkg и PostgreSQL.
Это как локальный сайт который вы запустите на компе вот ссылка - http://localhost:18080

📚 Структура проекта
pastebin-cpp/
├── src/                 # Исходники
├── include/             # Заголовочные файлы
├── db/                  # SQL скрипты
├── storage/             # Для хранения txt файлов (1-й релиз)
├── CMakeLists.txt       # Сборка проекта
└── README.md            # Этот файл

🧾 История релизов
🔹 Первый релиз (v1.0)
Сохранение сообщений в .txt файл

Простой веб-интерфейс на Crow

🔸 Второй релиз (v2.0)
Подключена база данных PostgreSQL

Все сообщения сохраняются с id, timestamp и message

Обновлён backend
