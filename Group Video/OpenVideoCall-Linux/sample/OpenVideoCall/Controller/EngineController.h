#pragma once

class MsgHandler;

class EngineController {
    public:
        EngineController();
        ~EngineController();

        void setModel(MsgHandler* model);
        void setView(MsgHandler* view);
        bool sendMsg(MsgHandler* sender, int msgUri, void* msg);

    private:
        MsgHandler* m_model;
        MsgHandler* m_view;
};

