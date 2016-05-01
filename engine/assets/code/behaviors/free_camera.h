#include "../core/behavior.h"

class FreeCamera : public Behavior
{
    protected:
        void Update() override;
    public:
        FreeCamera() = default;
        ~FreeCamera() = default;
    private:
};
