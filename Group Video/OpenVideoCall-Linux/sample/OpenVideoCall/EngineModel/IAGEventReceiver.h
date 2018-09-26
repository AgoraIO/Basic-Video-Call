#pragma once

class IAGEventReceiver {
    public:
        virtual void onEvent(int id, void* pData) = 0;
};
