#include <algorithm>
#include <memory>
#include <vector>

#include "../../inventory.hpp"
#include "../../world.hpp"
#include "../item.hpp"
#include "../mob.hpp"
#include "../player.hpp"
#include "player.hpp"

static constexpr int kActionOffset = 8;
static constexpr int kDropDistance = 16;

void MppPlayerController::OnAddEntity()
{
    GetInventory()->SetOnActionCallback([this](int index)
    {
        // TODO
    });
    GetInventory()->SetOnDropCallback([this](int index)
    {
        std::shared_ptr<MppPlayerEntity> player = GetPlayer();
        std::shared_ptr<MppEntity> item = std::make_shared<MppItemEntity>(GetInventory()->Remove(index));
        item->SetX(player->GetX() + player->GetFacingX() * kDropDistance);
        item->SetY(player->GetY() + player->GetFacingY() * kDropDistance);
        MppWorldAddEntity(item);
    });
    MppInputSetPlayer(std::dynamic_pointer_cast<MppInputHandler>(shared_from_this()));
}

void MppPlayerController::OnAction()
{
    std::shared_ptr<MppPlayerEntity> player = GetPlayer();
    std::vector<std::shared_ptr<MppEntity>> entities = MppWorldGetEntities(player->GetX(), player->GetY());
    std::erase_if(entities, [&](std::shared_ptr<MppEntity>& other)
    {
        // TODO: remove e.g. items
        return player == other;
    });
    player->PushNow(kActionOffset * player->GetFacingX(), kActionOffset * player->GetFacingY());
    std::sort(entities.begin(), entities.end(), [&player](std::shared_ptr<MppEntity>& lhs, std::shared_ptr<MppEntity>& rhs)
    {
        // TODO: sort mobs in front of furniture
        return player->GetDistance(lhs) < player->GetDistance(rhs);
    });
    if (!entities.empty())
    {
        std::shared_ptr<MppEntity>& entity = entities[0];
        if (player->GetDistance(entity) <= player->GetActionRange())
        {
            entity->OnAction(*player);
        }
    }
    else
    {
        // TODO: tiles
    }
    player->PushNow(-kActionOffset * player->GetFacingX(), -kActionOffset * player->GetFacingY());
}

void MppPlayerController::OnInventory()
{
    MppInputSetInteraction(GetPlayer()->GetInventory());
}

void MppPlayerController::OnHeldUp()
{
    GetPlayer()->Push(0, -1);
}

void MppPlayerController::OnHeldDown()
{
    GetPlayer()->Push(0, 1);
}

void MppPlayerController::OnHeldLeft()
{
    GetPlayer()->Push(-1, 0);
}

void MppPlayerController::OnHeldRight()
{
    GetPlayer()->Push(1, 0);
}

std::shared_ptr<MppPlayerEntity> MppPlayerController::GetPlayer()
{
    return std::dynamic_pointer_cast<MppPlayerEntity>(Entity.lock());
}
