#include <SDL3/SDL.h>
#include <savepoint/savepoint.hpp>

#include <filesystem>
#include <string_view>

#include "log.hpp"
#include "save.hpp"
#include "version.hpp"

static const std::filesystem::path kPrefPath = SDL_GetPrefPath(nullptr, "minicraft++");
static const std::filesystem::path kSavePath = kPrefPath / "minicraft++.savepoint";

static Savepoint savepoint;

bool MppSaveInit()
{
    SavepointSetLogFunction([](const std::string_view& string)
    {
        MppLog("Savepoint: %s", string.data());
    });
    SavepointStatus status = savepoint.Open(SavepointDriver::SQLite3, kSavePath.string(), kMppVersion);
    return status != SavepointStatus::Failed;
}

void MppSaveQuit()
{
    savepoint.Close();
}

bool MppSaveIsOpen()
{
    return savepoint.IsOpen();
}
