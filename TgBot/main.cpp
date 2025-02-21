#include <tgbot/tgbot.h>
#include <string>
#include <iostream>


int main() {
    const std::string token("7443024001:AAELfqHayQSb9QJbvy0PYg5m49lyKFlO8PI");
    try {
        TgBot::Bot bot(token);

        bot.getEvents().onCommand("start", [&bot](TgBot::Message::Ptr message) {
            std::string startText =
            "Hi! I am demo version Hovo V1.0.\n"
            "Воспользуйтесь /help, чтобы посмотреть список команд.";

            bot.getApi().sendMessage(message->chat->id, startText);
        });

        bot.getEvents().onCommand("help", [&bot](TgBot::Message::Ptr message) {
            std::string helpText =
                "Список команд:\n"
                "/start - приветствие\n"
                "/help - помощь\n"
                "/inline - показать inline-кнопку\n"
                "/reply - показать обычную клавиатуру\n";

            bot.getApi().sendMessage(message->chat->id, helpText);
        });

        // ---- /inline (пример inline-кнопки + Markdown) ----
        bot.getEvents().onCommand("inline", [&bot](TgBot::Message::Ptr message) {
            // Создаём inline-клавиатуру
            TgBot::InlineKeyboardMarkup::Ptr inlineKb(new TgBot::InlineKeyboardMarkup);

            // Формируем один ряд кнопок
            std::vector<TgBot::InlineKeyboardButton::Ptr> row;
            {
                TgBot::InlineKeyboardButton::Ptr btn(new TgBot::InlineKeyboardButton);
                btn->text = "Нажми меня!";
                btn->callbackData = "INLINE_BTN_HELLO"; // то, что придёт в callback
                row.push_back(btn);
            }
            inlineKb->inlineKeyboard.push_back(row);

            // Пример отключения web-превью: создаём LinkPreviewOptions
            TgBot::LinkPreviewOptions::Ptr previewOptions(new TgBot::LinkPreviewOptions);
            previewOptions->isDisabled = true;

            // Отправляем сообщение с inline-кнопкой, Markdown и отключённым превью
            bot.getApi().sendMessage(
                message->chat->id,                         // chatId
                "*Демо inline-кнопки* в Markdown",          // text
                previewOptions,                             // linkPreviewOptions
                nullptr,                                    // replyParameters
                inlineKb,                                   // replyMarkup
                "Markdown",                                 // parseMode
                false                                       // disableNotification
                // Остальные параметры: entities = {},
                // messageThreadId = 0, protectContent = false,
                // businessConnectionId = "" — берем по умолчанию
            );
        });

        // ---- /reply (пример обычной клавиатуры) ----
        bot.getEvents().onCommand("reply", [&bot](TgBot::Message::Ptr message) {
            TgBot::ReplyKeyboardMarkup::Ptr replyKb(new TgBot::ReplyKeyboardMarkup);
            replyKb->resizeKeyboard = true;
            replyKb->oneTimeKeyboard = true;

            // 1-й ряд
            {
                std::vector<TgBot::KeyboardButton::Ptr> row;
                TgBot::KeyboardButton::Ptr btn1(new TgBot::KeyboardButton);
                btn1->text = "First";
                row.push_back(btn1);

                TgBot::KeyboardButton::Ptr btn2(new TgBot::KeyboardButton);
                btn2->text = "Second";
                row.push_back(btn2);

                replyKb->keyboard.push_back(row);
            }

            // 2-й ряд
            {
                std::vector<TgBot::KeyboardButton::Ptr> row;
                TgBot::KeyboardButton::Ptr btn3(new TgBot::KeyboardButton);
                btn3->text = "Third";
                row.push_back(btn3);

                replyKb->keyboard.push_back(row);
            }

            {
                std::vector<TgBot::KeyboardButton::Ptr> row;
                TgBot::KeyboardButton::Ptr btn4(new TgBot::KeyboardButton);
                btn4->text = "Fourth";
                row.push_back(btn4);

                replyKb->keyboard.push_back(row);
            }

            // Отправляем сообщение с reply-клавиатурой
            bot.getApi().sendMessage(
                message->chat->id,
                "Enter the button"
                // Всё остальное берем по умолчанию:
                // linkPreviewOptions = nullptr,
                // replyParameters = nullptr,
                // replyMarkup = replyKb (вставим здесь),
                // parseMode = "",
                // disableNotification = false,
                // ...
            ,
                nullptr,    // linkPreviewOptions (не трогаем превью)
                nullptr,    // replyParameters
                replyKb     // replyMarkup
            );
        });

        // ---- callback от inline-кнопки ----
        bot.getEvents().onCallbackQuery([&bot](TgBot::CallbackQuery::Ptr query) {
            if (query->data == "INLINE_BTN_HELLO") {
                // Ответим callback’ом, чтобы Telegram не «ругался» на неотвеченный коллбэк
                bot.getApi().answerCallbackQuery(query->id, "Спасибо, что нажали кнопку!");
                // И вышлем что-то в чат
                bot.getApi().sendMessage(query->message->chat->id, "Привет из inline-кнопки!");
            }
        });

        // ---- Обработка «любой другой» команды/сообщения ----
        bot.getEvents().onAnyMessage([&bot](TgBot::Message::Ptr message) {
            // Проверяем, не команда ли (если начинается на "/")
            // В некоторых версиях tgbot-cpp метод называется StringTools::starts_with.
            // У вас, судя по ошибкам, может быть StringTools::startsWith.
            if (StringTools::startsWith(message->text, "/")) {
                // Уже обработана какая-то команда — выходим
                return;
            }

            // Обычный текст — отвечаем эхом
            bot.getApi().sendMessage(message->chat->id, "Вы написали: " + message->text);
        });

        // ---- Запускаем long poll ----
        TgBot::TgLongPoll longPoll(bot);
        std::cout << "Бот запущен. Ожидаю сообщения..." << std::endl;

        while (true) {
            longPoll.start();
        }

    } catch (const std::exception& e) {
        std::cerr << "Ошибка: " << e.what() << std::endl;
    }

    exit(0);
}
