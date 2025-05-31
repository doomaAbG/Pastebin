#include "crow.h"
#include <string>
#include <fstream>
#include <random>
#include <ctime> // Для получения текущей даты и времени

// Функция для генерации уникального ID
std::string generate_id() {
    std::random_device rd;              // Источник случайных чисел
    std::mt19937 gen(rd());             // Генератор случайных чисел
    std::uniform_int_distribution<> dis(10000, 99999); // Диапазон от 10000 до 99999
    return std::to_string(dis(gen));    // Преобразуем число в строку
}

// Функция для получения текущей даты и времени
std::string get_current_time() {
    std::time_t now = std::time(nullptr); // Текущее время в секундах
    std::string time_str = std::ctime(&now); // Преобразуем в строку (например, "Sat May 31 15:05:00 2025\n")
    time_str.pop_back(); // Убираем \n в конце
    return time_str;
}

int main() {
    crow::SimpleApp app;    // Создаём объект приложения Crow

    // Настраиваем логирование (чтобы видеть INFO-сообщения)
    app.loglevel(crow::LogLevel::Info);

    // Маршрут для главной страницы с HTML-формой
    CROW_ROUTE(app, "/")([]() {
        return R"(
            <html>
                <body>
                    <h1>Welcome to my Pastebin!</h1>
                    <form method="POST" action="/paste">
                        <textarea name="content" rows="10" cols="50"></textarea><br>
                        <input type="submit" value="Create Paste">
                    </form>
                </body>
            </html>
        )";
    });

    // Маршрут для создания пасты через POST-запрос
    CROW_ROUTE(app, "/paste").methods(crow::HTTPMethod::POST)([](const crow::request& req) {
        // Логируем сырой запрос (до декодирования)
        CROW_LOG_INFO << "Raw request body: " << req.body;

        // Получаем текст из запроса
        std::string paste_content = req.body;

        // Логируем декодированный текст
        CROW_LOG_INFO << "Decoded content: " << paste_content;

        // Проверяем, пустой ли текст
        if (paste_content.empty()) {
            CROW_LOG_INFO << "Content is empty, returning 400";
            return crow::response(400, "Paste content cannot be empty");
        }

        // Генерируем уникальный ID
        std::string paste_id = generate_id();
        CROW_LOG_INFO << "Generated paste ID: " << paste_id;

        // Получаем текущую дату и время
        std::string timestamp = get_current_time();

        // Формируем содержимое файла с дополнительной информацией
        std::string file_content = "Paste ID: " + paste_id + "\n"
                                + "Created at: " + timestamp + "\n"
                                + "Content:\n" + paste_content;

        // Открываем файл для записи
        std::ofstream file("C:/Pastebin/data/" + paste_id + ".txt");
        if (!file.is_open()) {
            CROW_LOG_INFO << "Failed to open file for paste ID: " << paste_id;
            return crow::response(500, "Failed to save paste");
        }

        // Сохраняем текст в файл
        file << file_content;
        file.close();

        // Логируем, что сохранили
        CROW_LOG_INFO << "Saved paste with ID: " << paste_id;

        // Возвращаем ответ с ссылкой на пасту
        std::string response = "Paste created! Access it at <a href=\"/paste/" + paste_id + "\">/paste/" + paste_id + "</a>";
        return crow::response(200, response);
    });

    // Маршрут для получения пасты по ID через GET-запрос
    CROW_ROUTE(app, "/paste/<string>")([](std::string paste_id) {
        // Открываем файл для чтения
        std::ifstream file("C:/Pastebin/data/" + paste_id + ".txt");
        if (!file.is_open()) {
            CROW_LOG_INFO << "Paste not found for ID: " << paste_id;
            return crow::response(404, "Paste not found");
        }

        // Читаем содержимое файла
        std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
        file.close();

        // Возвращаем пасту с форматированием
        return crow::response(200, "<h1>Paste #" + paste_id + "</h1><pre>" + content + "</pre>");
    });

    app.port(8080).multithreaded().run();    // Запускаем сервер на порту 8080 с многопоточностью
}