#pragma once

#include <savepoint/savepoint.hpp>

#include <cstdint>
#include <memory>
#include <string>
#include <utility>
#include <vector>

class MppEntity :
    public SavepointEntity,
    public SavepointBase,
    public std::enable_shared_from_this<MppEntity>
{
protected:
    MppEntity();

public:
    virtual ~MppEntity() = default;
    virtual void OnAddEntity() {}
    virtual void Visit(SavepointVisitor& visitor) override;
    virtual void Render() const;
    virtual void Update(uint64_t ticks) {}
    virtual void OnAction(MppEntity& instigator) {}
    virtual void OnCollision(MppEntity& instigator) {}
    void SetX(int x);
    void SetY(int y);
    int GetX() const;
    int GetY() const;
    int GetPhysicsX() const;
    int GetPhysicsY() const;
    virtual int GetPhysicsOffsetX() const = 0;
    virtual int GetPhysicsOffsetY() const = 0;
    virtual int GetPhysicsWidth() const = 0;
    virtual int GetPhysicsHeight() const = 0;
    virtual int GetSize() const = 0;
    void Kill();
    bool IsDead() const;
    std::string GetName() const;

protected:
    int GetDistance(const std::shared_ptr<MppEntity>& entity) const;
    void Move(int dx, int dy);
    std::vector<std::pair<int, int>> Raycast(int x2, int y2);

private:
    void MoveAxis(int dx, int dy);
    bool MoveAxisTest();

protected:
    int X;
    int Y;

private:
    bool Dead;
};
