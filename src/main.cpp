#include "crow.h"
#include <pqxx/pqxx>
#include <string>
#include <random>
#include <ctime>

// Функция для декодирования текста (заменяем + на пробелы)
std::string urldecode(const std::string& str) {
    std::string decoded = str;
    // Заменяем + на пробелы
    size_t pos = 0;
    while ((pos = decoded.find('+', pos)) != std::string::npos) {
        decoded.replace(pos, 1, " ");
        pos++;
    }
    return decoded;
}

// Функция для генерации уникального ID
std::string generate_id() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(10000, 99999);
    return std::to_string(dis(gen));
}

// Функция для получения текущей даты и времени
std::string get_current_time() {
    std::time_t now = std::time(nullptr);
    std::string time_str = std::ctime(&now);
    time_str.pop_back();
    return time_str;
}

int main() {
    crow::SimpleApp app;
    app.loglevel(crow::LogLevel::Info);

    // Подключение к PostgreSQL
    pqxx::connection conn("dbname=pastebin_db user=postgres password=0000 host=localhost port=5432");
    if (!conn.is_open()) {
        CROW_LOG_ERROR << "Cannot connect to PostgreSQL";
        return 1;
    }

    // Создаём таблицу, если её нет
    pqxx::work txn(conn);
    txn.exec("CREATE TABLE IF NOT EXISTS pastes (id VARCHAR(10) PRIMARY KEY, created_at TIMESTAMP, content TEXT)");
    txn.commit();

    // Маршрут для главной страницы
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

    // Маршрут для создания пасты
    CROW_ROUTE(app, "/paste").methods(crow::HTTPMethod::POST)([&conn](const crow::request& req) {
        CROW_LOG_INFO << "Raw request body: " << req.body;

        // Декодируем текст из формы (например, content=normalno+poshlo → normalno poshlo)
        std::string raw_body = req.body;
        // Убираем префикс "content=" и декодируем
        if (raw_body.substr(0, 8) == "content=") {
            raw_body = raw_body.substr(8);
        }
        std::string paste_content = urldecode(raw_body);
        CROW_LOG_INFO << "Decoded content: " << paste_content;

        if (paste_content.empty()) {
            CROW_LOG_INFO << "Content is empty, returning 400";
            return crow::response(400, "Paste content cannot be empty");
        }

        std::string paste_id = generate_id();
        CROW_LOG_INFO << "Generated paste ID: " << paste_id;

        std::string timestamp = get_current_time();

        // Сохраняем в PostgreSQL
        try {
            pqxx::work txn(conn);
            txn.exec_params("INSERT INTO pastes (id, created_at, content) VALUES ($1, $2, $3)",
                            paste_id, timestamp, paste_content);
            txn.commit();
            CROW_LOG_INFO << "Saved paste with ID: " << paste_id;
        } catch (const std::exception& e) {
            CROW_LOG_ERROR << "Failed to save paste: " << e.what();
            return crow::response(500, "Failed to save paste");
        }

        std::string response = "Paste created! Access it at <a href=\"/paste/" + paste_id + "\">/paste/" + paste_id + "</a>";
        return crow::response(200, response);
    });

    // Маршрут для получения пасты
    CROW_ROUTE(app, "/paste/<string>")([&conn](std::string paste_id) {
        try {
            pqxx::work txn(conn);
            pqxx::result res = txn.exec_params("SELECT content FROM pastes WHERE id = $1", paste_id);
            if (res.empty()) {
                CROW_LOG_INFO << "Paste not found for ID: " << paste_id;
                return crow::response(404, "Paste not found");
            }
            std::string paste_content = res[0][0].as<std::string>();
            return crow::response(200, "<h1>Paste #" + paste_id + "</h1><pre>" + paste_content + "</pre>");
        } catch (const std::exception& e) {
            CROW_LOG_ERROR << "Failed to retrieve paste: " << e.what();
            return crow::response(500, "Failed to retrieve paste");
        }
    });

    app.port(8080).multithreaded().run();
}