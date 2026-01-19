#include "entity.hpp"

void MppEntity::Visit(SavepointVisitor& visitor)
{
    visitor(X);
    visitor(Y);
}
