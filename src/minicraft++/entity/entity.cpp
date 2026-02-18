#include <savepoint/savepoint.hpp>

#include <algorithm>
#include <cctype>
#include <cmath>
#include <memory>
#include <string>
#include <string_view>
#include <vector>

#include <minicraft++/assert.hpp>
#include <minicraft++/color.hpp>
#include <minicraft++/console.hpp>
#include <minicraft++/entity/entity.hpp>
#include <minicraft++/entity/furniture/furniture.hpp>
#include <minicraft++/entity/mob/mob.hpp>
#include <minicraft++/entity/projectile/projectile.hpp>
#include <minicraft++/log.hpp>
#include <minicraft++/renderer.hpp>
#include <minicraft++/tile.hpp>
#include <minicraft++/util.hpp>
#include <minicraft++/world.hpp>

MppEntityReference::MppEntityReference()
    : Entity{}
    , EntityID{}
{
}

MppEntityReference::MppEntityReference(const std::shared_ptr<MppEntity>& entity)
    : Entity{entity}
    , EntityID{entity->GetID()}
{
}

void MppEntityReference::Visit(SavepointVisitor& visitor)
{
    if (visitor.IsReading())
    {
        MppAssert(Entity.expired());
    }
    visitor(EntityID);
}

std::shared_ptr<MppEntity> MppEntityReference::GetEntity()
{
    std::shared_ptr<MppEntity> entity = Entity.lock();
    if (entity)
    {
        return entity;
    }
    if (!EntityID.IsValid())
    {
        return nullptr;
    }
    entity = MppWorldGetEntity(EntityID);
    Entity = entity;
    if (!entity)
    {
        MppLog("Failed to find reference on entity");
        EntityID = SavepointID{};
    }
    return entity;
}

bool MppEntityReference::IsValid() const
{
    return !Entity.expired();
}

MppEntity::MppEntity()
    : X{0}
    , Y{0}
    , Spawned{false}
{
}

void MppEntity::OnCreate()
{
    if (HasPhysics())
    {
        int size = GetSize();
        int x = GetPhysicsOffsetX();
        int y = GetPhysicsOffsetY();
        int w = GetPhysicsWidth();
        int h = GetPhysicsHeight();
        MppAssert(x * 2 + w == size);
        MppAssert(y * 2 + h == size);
    }
}

void MppEntity::OnAdd()
{
    MppAssert(!Spawned);
    Spawned = true;
}

void MppEntity::Visit(SavepointVisitor& visitor)
{
    visitor(X);
    visitor(Y);
}

void MppEntity::Render() const
{
    if (IsSpawned() && MppConsole::CVarPhysics.GetBool())
    {
        int color = 0;
        int x = GetPhysicsX();
        int y = GetPhysicsY();
        int w = GetPhysicsWidth();
        int h = GetPhysicsHeight();
        if (IsA<MppMobEntity>())
        {
            color = kMppColorDebugMobEntityPhysics;
        }
        else if (IsA<MppFurnitureEntity>())
        {
            color = kMppColorDebugFurnitureEntityPhysics;
        }
        else if (IsA<MppProjectileEntity>())
        {
            color = kMppColorDebugProjectileEntityPhysics;
        }
        if (color)
        {
            MppRendererDrawRect(color, x, y, w, h, MppRendererLayerDebugEntityPhysics);
        }
    }
}

bool MppEntity::OnAction(std::shared_ptr<MppEntity>& instigator)
{
    return false;
}

bool MppEntity::OnInteraction(std::shared_ptr<MppEntity>& instigator)
{
    return false;
}

MppEntityCollision MppEntity::OnCollision(std::shared_ptr<MppEntity>& instigator, int dx, int dy)
{
    return MppEntityCollisionAccept;
}

bool MppEntity::HasPhysics() const
{
    return true;
}

bool MppEntity::CanBeSaved() const
{
    return true;
}

MppEntityReference MppEntity::GetReference()
{
    return MppEntityReference(shared_from_this());
}

bool MppEntity::IsEmpty() const
{
    return true;
}

MppItemID MppEntity::GetItemID() const
{
    return MppItemIDInvalid;
}

bool MppEntity::Drop(const std::shared_ptr<MppEntity>& instigator)
{
    return false;
}

bool MppEntity::IsColliding()
{
    return MoveTest(0, 0) != MppEntityCollisionAccept;
}

void MppEntity::Unspawn()
{
    Spawned = false;
}

bool MppEntity::IsSpawned() const
{
    return Spawned;
}

void MppEntity::SetX(int x)
{
    X = x;
}

void MppEntity::SetY(int y)
{
    Y = y;
}

int MppEntity::GetX() const
{
    return X;
}

int MppEntity::GetY() const
{
    return Y;
}

int MppEntity::GetPhysicsX() const
{
    return X + GetPhysicsOffsetX();
}

int MppEntity::GetPhysicsY() const
{
    return Y + GetPhysicsOffsetY();
}

std::pair<int, int> MppEntity::GetCenter() const
{
    return std::make_pair(GetX() + GetSize() / 2, GetY() + GetSize() / 2);
}

std::string MppEntity::GetName() const
{
    std::string_view name = GetClassName();
    static constexpr const char kPrefix[] = "Mpp";
    static constexpr const char kSuffix[] = "Entity";
    MppAssert(name.starts_with(kPrefix));
    MppAssert(name.ends_with(kSuffix));
    std::string string;
    string.reserve(name.size());
    for (int i = sizeof(kPrefix) - 1; i < name.size() - sizeof(kSuffix) + 1; i++)
    {
        if (std::islower(name[i]))
        {
            string.push_back(name[i]);
        }
        else
        {
            if (i > 3)
            {
                string.push_back('_');
            }
            string.push_back(std::tolower(name[i]));
        }
    }
    return string;
}

int MppEntity::GetDistance(const std::shared_ptr<MppEntity>& entity) const
{
    auto [x1, y1] = GetCenter();
    auto [x2, y2] = entity->GetCenter();
    float dx = x2 - x1;
    float dy = y2 - y1;
    return std::sqrt(dx * dx + dy * dy);
}

int MppEntity::IsColliding(const std::shared_ptr<MppEntity>& entity)
{
    int x = entity->GetPhysicsX();
    int y = entity->GetPhysicsY();
    int w = entity->GetPhysicsWidth();
    int h = entity->GetPhysicsHeight();
    return IsColliding(x, y, w, h);
}

int MppEntity::IsColliding(int inX, int inY, int inW, int inH)
{
    int x = GetPhysicsX();
    int y = GetPhysicsY();
    int w = GetPhysicsWidth();
    int h = GetPhysicsHeight();
    return (x + w > inX) && (x < inX + inW) && (y + h > inY) && (y < inY + inH);
}

MppEntityCollision MppEntity::Move(int dx, int dy)
{
    MppEntityCollision collision = MppEntityCollisionAccept;
    auto function = [&collision, this](int& delta, int signX, int signY)
    {
        if (!delta)
        {
            return false;
        }
        int step = MppNormalize(delta);
        switch (MoveTest(delta * signX, delta * signY))
        {
        case MppEntityCollisionReject:
            collision = MppEntityCollisionReject;
            break;
        case MppEntityCollisionTeleport:
            collision = MppEntityCollisionTeleport;
            break;
        }
        delta -= step;
        return collision == MppEntityCollisionTeleport;
    };
    while (dx || dy)
    {
        if (function(dx, 1, 0))
        {
            return collision;
        }
        if (function(dy, 0, 1))
        {
            return collision;
        }
    }
    return collision;
}

MppEntityCollision MppEntity::MoveTest(int dx, int dy)
{
    // Collisions can be infinitely recursive so we do some detection
    static std::vector<SavepointID> recursiveIDs;
    if (std::find(recursiveIDs.begin(), recursiveIDs.end(), GetID()) != recursiveIDs.end())
    {
        return MppEntityCollisionAccept;
    }
    recursiveIDs.push_back(GetID());
    MppEntityCollision collision = MoveTestImpl(dx, dy);
    int count = std::erase_if(recursiveIDs, [this](SavepointID id)
    {
        return id == GetID();
    });
    MppAssert(count == 1);
    return collision;
}

MppEntityCollision MppEntity::MoveTestImpl(int dx, int dy)
{
    std::shared_ptr<MppEntity> self = Cast<MppEntity>();
    int entityX = X;
    int entityY = Y;
    X += dx;
    Y += dy;
    int size = GetSize();
    int x = GetPhysicsX();
    int y = GetPhysicsY();
    int w = GetPhysicsWidth();
    int h = GetPhysicsHeight();
    int tileX1 = x / MppTile::kSize;
    int tileY1 = y / MppTile::kSize;
    int tileX2 = (x + w) / MppTile::kSize;
    int tileY2 = (y + h) / MppTile::kSize;
    MppEntityCollision collision = MppEntityCollisionAccept;
    for (int tileX = tileX1; tileX <= tileX2; tileX++)
    for (int tileY = tileY1; tileY <= tileY2; tileY++)
    {
        MppTile& tile = MppWorldGetTile(tileX, tileY);
        int tx = tile.GetPhysicsX(tileX);
        int ty = tile.GetPhysicsY(tileY);
        int tw = tile.GetPhysicsWidth();
        int th = tile.GetPhysicsHeight();
        if (!IsColliding(tx, ty, tw, th))
        {
            continue;
        }
        switch (tile.OnCollision(self, tileX, tileY))
        {
        case MppEntityCollisionReject:
            collision = MppEntityCollisionReject;
            break;
        case MppEntityCollisionTeleport:
            return MppEntityCollisionTeleport;
        }
    }
    for (std::shared_ptr<MppEntity>& entity : MppWorldGetEntities(X, Y))
    {
        if (this == entity.get())
        {
            continue;
        }
        int ex = entity->GetPhysicsX();
        int ey = entity->GetPhysicsY();
        int ew = entity->GetPhysicsWidth();
        int eh = entity->GetPhysicsHeight();
        if (!IsColliding(ex, ey, ew, eh))
        {
            continue;
        }
        switch (entity->OnCollision(self, dx, dy))
        {
        case MppEntityCollisionReject:
            collision = MppEntityCollisionReject;
            break;
        case MppEntityCollisionTeleport:
            return MppEntityCollisionTeleport;
        }
    }
    MppAssert(collision != MppEntityCollisionTeleport);
    if (collision == MppEntityCollisionReject)
    {
        X = entityX;
        Y = entityY;
    }
    return collision;
}
