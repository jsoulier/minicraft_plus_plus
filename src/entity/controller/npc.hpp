#pragma once

#include <vector>
#include <utility>

#include "../controller.hpp"

class MppNPCController : public MppController
{
public:
    virtual void Visit(SavepointVisitor& visitor) override;
    virtual void Update(uint64_t ticks) override;

protected:
    bool TryNavigateRandom();
    bool TryNavigateLine(const std::shared_ptr<MppEntity>& target);
    void CancelNavigation();
    bool IsNavigationCompleted() const;
    virtual int GetMaxNavigateRandomDistance() const;

private:
    std::vector<std::pair<int, int>> Points;
    int PointIndex;
};
