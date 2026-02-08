#pragma once

#include <savepoint/savepoint.hpp>

#include <minicraft++/entity/furniture/furniture.hpp>
#include <minicraft++/item.hpp>

class MppLanternEntity : public MppFurnitureEntity
{
    SAVEPOINT_POLY(MppLanternEntity)

public:
    void Render() const override;
    MppItemID GetItemID() const override;
    
private:
    int GetSpriteX() const override;
    int GetSpriteY() const override;
    int GetColor1() const override;
    int GetColor2() const override;
    int GetColor3() const override;
    int GetColor4() const override;
    int GetColor5() const override;
};
