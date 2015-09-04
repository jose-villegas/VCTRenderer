#pragma once
class ExternalInitializer
{
    public:
        void InitializeContextDependant(bool printLibInfo = true);
        void Initialize(bool printLibInfo = true);
        ExternalInitializer();
        ~ExternalInitializer();
};

