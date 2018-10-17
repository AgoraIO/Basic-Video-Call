#pragma once

class CommandLineView;
class EngineController;
struct AppConfig;

class OpenVideoCallApp {
    public:
        OpenVideoCallApp();
        ~OpenVideoCallApp();

        void loadConfig(const AppConfig& cfg);

        void run(bool open);

    private:
        CommandLineView* m_view;
        EngineController* m_controller;
};
