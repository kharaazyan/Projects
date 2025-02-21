#pragma once


#include <tgbot/tgbot.h>
#include <unordered_map>
#include <functional>
#include <iostream>
#include <string>
#include "ThreadPool.hpp"

/**
 * AsyncBot – обёртка над TgBot::Bot, которая:
 * - Регистрирует команды и callback-обработчики,
 * - Передаёт обработку в ThreadPool для асинхронного выполнения.
 */
class AsyncBot {
public:
    /// Создаёт бота с указанным токеном и размером пула (по умолчанию 6 потоков)
    AsyncBot(const std::string& token, size_t poolSize = 6);

    /// Запускает long poll (блокирующий вызов)
    void start();

    /// Регистрирует обработчик команды (без символа '/')
    void addCommand(const std::string& commandName,
                    std::function<void(TgBot::Bot&, TgBot::Message::Ptr)> handler);

    /// Регистрирует обработчик callback для inline-кнопки
    void addCallback(const std::string& callbackData,
                     std::function<void(TgBot::Bot&, TgBot::CallbackQuery::Ptr)> handler);

    /// Устанавливает обработчик для сообщений, не являющихся командами
    void setDefaultMessageHandler(
                    std::function<void(TgBot::Bot&, TgBot::Message::Ptr)> handler);

    /// Доступ к оригинальному TgBot::Bot (на свой страх и риск)
    TgBot::Bot& getBot() { return bot; }

private:
    TgBot::Bot bot;
    ThreadPool pool;

    std::unordered_map<std::string, std::function<void(TgBot::Bot&, TgBot::Message::Ptr)>> commandHandlers;
    std::unordered_map<std::string, std::function<void(TgBot::Bot&, TgBot::CallbackQuery::Ptr)>> callbackHandlers;

    std::function<void(TgBot::Bot&, TgBot::Message::Ptr)> defaultMessageHandler;
};


AsyncBot::AsyncBot(const std::string& token, size_t poolSize)
    : bot(token), pool(poolSize)
{
    // Глобальный обработчик callback для inline-кнопок
    bot.getEvents().onCallbackQuery([this](TgBot::CallbackQuery::Ptr query) {
        pool.enqueue([this, query]() {
            auto it = callbackHandlers.find(query->data);
            if (it != callbackHandlers.end()) {
                it->second(bot, query);
            } else {
                bot.getApi().answerCallbackQuery(query->id, "Неизвестный callback");
            }
        });
    });

    // Глобальный обработчик для всех входящих сообщений
    bot.getEvents().onAnyMessage([this](TgBot::Message::Ptr message) {
        if (StringTools::startsWith(message->text, "/")) {
            // Извлекаем имя команды (без '/')
            std::string commandName = message->text.substr(1);
            auto spacePos = commandName.find(' ');
            if (spacePos != std::string::npos) {
                commandName = commandName.substr(0, spacePos);
            }
            auto it = commandHandlers.find(commandName);
            if (it != commandHandlers.end()) {
                pool.enqueue([this, &message, &it]() {
                    it->second(bot, message);
                });
            }
        } else {
            if (defaultMessageHandler) {
                pool.enqueue([this, &message]() {
                    defaultMessageHandler(bot, message);
                });
            }
        }
    });
}

void AsyncBot::start() {
    TgBot::TgLongPoll longPoll(bot);
    std::cout << "Бот запущен. Ожидаю сообщения..." << std::endl;
    while (true) {
        longPoll.start();
    }
}

void AsyncBot::addCommand(const std::string& commandName,
    std::function<void(TgBot::Bot&, TgBot::Message::Ptr)> handler)
{
    commandHandlers[commandName] = handler;
}

void AsyncBot::addCallback(const std::string& callbackData,
    std::function<void(TgBot::Bot&, TgBot::CallbackQuery::Ptr)> handler)
{
    callbackHandlers[callbackData] = handler;
}

void AsyncBot::setDefaultMessageHandler(
    std::function<void(TgBot::Bot&, TgBot::Message::Ptr)> handler)
{
    defaultMessageHandler = handler;
}
