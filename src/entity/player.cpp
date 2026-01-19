#include <memory>

#include "../input.hpp"
#include "../renderer.hpp"
#include "player.hpp"

class Handler : public MppInputHandler
{
public:
    Handler(MppPlayerEntity& player)
        : Player(player)
    {
    }

    void OnAction() override
    {
    }

    void OnHeldUp() override
    {
        Player.VelocityY--;
    }

    void OnHeldDown() override
    {
        Player.VelocityY++;
    }

    void OnHeldLeft() override
    {
        Player.VelocityX--;
    }

    void OnHeldRight() override
    {
        Player.VelocityX++;
    }

private:
    MppPlayerEntity& Player;
};

MppPlayerEntity::MppPlayerEntity()
{
    InputHandler = std::make_shared<Handler>(*this);
    MppInputPush(InputHandler);
}

void MppPlayerEntity::Update(uint64_t ticks)
{
    MppHumanoidEntity::Update(ticks);
    MppRendererMove(X, Y, GetSize());
}

void MppPlayerEntity::Render()
{
    MppHumanoidEntity::Render();
}
