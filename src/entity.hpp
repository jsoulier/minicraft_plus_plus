#pragma once

#include <savepoint/base.hpp>
#include <savepoint/id.hpp>

#include <memory>
#include <utility>

class MppEntity : public SavepointBase
{
public:
    virtual void Visit(SavepointVisitor& visitor) override;

    template<typename T, typename... Args>
    static std::shared_ptr<T> Create(Args&&... args)
    {
        return std::make_shared<T>(std::forward<Args>(args)...);
    }

    template<typename OutT, typename InT>
    std::shared_ptr<OutT> Cast(const std::shared_ptr<InT>& entity)
    {
        return std::dynamic_pointer_cast<OutT>(entity);
    }

public:
    SavepointID ID;

private:
    int X;
    int Y;
};
