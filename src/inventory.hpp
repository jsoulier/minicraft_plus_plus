#pragma once

#include <savepoint_fwd.hpp>

#include <string_view>
#include <vector>

#include "item.hpp"
#include "screen.hpp"

class MppRenderer;

class MppInventory : public MppScreen
{
public:
    void Draw(MppRenderer& renderer);
    void Visit(SavepointVisitor& visitor);

protected:
    std::string_view GetName() const override;
    int GetX() const override;
    int GetY() const override;
    int GetWidth() const override;
    int GetHeight() const override;

private:
    std::vector<MppItem> Items;
};