#pragma once

#include "PCH.hpp"

namespace AE
{
    struct ApplicationInfo {
        std::string name, version, author, description;
    };

    class Engine;
    class Application
    {
    public:

        Application(const ApplicationInfo& info);
        virtual ~Application();

        const ApplicationInfo& GetInfo() const;
        const std::string& GetName() const;
        const std::string& GetVersion() const;
        const std::string& GetAuthor() const;
        const std::string& GetDescription() const;

        bool IsInitialized() const;
    
    protected:

        Engine* engine = nullptr;
        
        virtual bool OnInitialize() { return true; }
        virtual void OnShutdown() {}
        virtual void OnUpdate() {}
        virtual void OnRender() {}

    private:
        
        ApplicationInfo _info;

        struct ApplicationState {
            bool initialized = false;
        } _state;

        bool _Initialize();
        void _Shutdown();
        void _Update();
        void _Render();

        void _SetEngine(Engine* engine);

        friend class Engine;

    };
}