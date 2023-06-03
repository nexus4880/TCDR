#include "Settings.hpp"
#include <fstream>
#include <filesystem>
#include <nlohmann/json.hpp>
#include <exception>

#define TRY_READ(type, jobj, path) \
    ([&jobj]() -> type { \
        try { \
            return jobj path; \
        } catch (const std::exception& ex) { \
            return type{}; \
        } \
    })()

#define TRY_READ_LIST(type, vec, jobj, path) \
    ([&jobj, &vec](){ \
        try { \
            for (const auto& element : jobj path) { \
                vec.push_back(element.get<type>()); \
            } \
        } catch (const std::exception& ex) { \
        } \
    })()

#define SET_JSON_VALUE(jobj, path, value) \
    jobj path = value

#define SET_LIST_VALUE(jobj, vec, path) \
    for (const auto& element : vec) { \
        jobj path.push_back(element); \
    }

Settings Settings::FromFile(std::filesystem::path file, bool* hasErrorOccurred) {
    if (hasErrorOccurred) {
        *hasErrorOccurred = false;
    }

    if (!file.has_extension() || file.extension() != ".json") {
        if (hasErrorOccurred) {
            *hasErrorOccurred = true;
        }

        return Settings{};
    }

    std::ifstream fs(file);
    if (!fs) {
        if (hasErrorOccurred) {
            *hasErrorOccurred = true;
        }

        return Settings{};
    }

    nlohmann::json json{};
    try {
        fs >> json;
    }
    catch (const std::exception& ex) {
        if (hasErrorOccurred) {
            *hasErrorOccurred = true;
        }

        return Settings{};
    }

    Settings settings{};

    settings.showFPS = TRY_READ(bool, json, ["showFPS"]);
    settings.visualizeImportantRadius = TRY_READ(bool, json, ["visualizeImportantRadius"]);
    settings.importantRadius = TRY_READ(float, json, ["importantRadius"]);
    settings.updateRate = TRY_READ(float, json, ["updateRate"]);

    settings.noRecoil.enabled = TRY_READ(bool, json, ["noRecoil"]["enabled"]);
    settings.noRecoil.shootinggIntensity = TRY_READ(float, json, ["noRecoil"]["shootinggIntensity"]);
    settings.noRecoil.breathIntensity = TRY_READ(float, json, ["noRecoil"]["breathIntensity"]);
    settings.noRecoil.motionIntensity = TRY_READ(float, json, ["noRecoil"]["motionIntensity"]);

    settings.snapLines.activeMode = TRY_READ(int, json, ["snapLines"]["activeMode"]);
    settings.snapLines.types[0] = TRY_READ(bool, json, ["snapLines"]["types"][0]);
    settings.snapLines.types[1] = TRY_READ(bool, json, ["snapLines"]["types"][1]);
    settings.snapLines.types[2] = TRY_READ(bool, json, ["snapLines"]["types"][2]);

    settings.boxESP.types[0] = TRY_READ(bool, json, ["boxESP"]["types"][0]);
    settings.boxESP.types[1] = TRY_READ(bool, json, ["boxESP"]["types"][1]);
    settings.boxESP.types[2] = TRY_READ(bool, json, ["boxESP"]["types"][2]);
    settings.boxESP.factor = TRY_READ(float, json, ["boxESP"]["factor"]);

    settings.skeletonESP.types[0] = TRY_READ(bool, json, ["skeletonESP"]["types"][0]);
    settings.skeletonESP.types[1] = TRY_READ(bool, json, ["skeletonESP"]["types"][1]);
    settings.skeletonESP.types[2] = TRY_READ(bool, json, ["skeletonESP"]["types"][2]);
    settings.skeletonESP.distance = TRY_READ(float, json, ["skeletonESP"]["distance"]);
    settings.skeletonESP.closeFOV = TRY_READ(float, json, ["skeletonESP"]["closeFOV"]);
    settings.skeletonESP.entities = TRY_READ(int, json, ["skeletonESP"]["entities"]);

    settings.lootESP.enabled = TRY_READ(bool, json, ["lootESP"]["enabled"]);
    settings.lootESP.distance = TRY_READ(float, json, ["lootESP"]["distance"]);
    std::vector<std::string> lootESPFilters{};
    TRY_READ_LIST(std::string, lootESPFilters, json, ["lootESP"]["filters"]);
    for (const auto& filter : lootESPFilters) {
        settings.lootESP.filters.push_back(std::wstring{filter.begin(), filter.end()});
    }

    settings.keybinds.toggleNoRecoil = TRY_READ(int, json, ["keybinds"]["toggleNoRecoil"]);

    settings.debug.enabled = TRY_READ(int, json, ["debug"]["enabled"]);

    return settings;
}

void Settings::Serialize() {
    nlohmann::json jsonSettings;

    SET_JSON_VALUE(jsonSettings, ["showFPS"], this->showFPS);

    SET_JSON_VALUE(jsonSettings, ["visualizeImportantRadius"], this->visualizeImportantRadius);
    SET_JSON_VALUE(jsonSettings, ["importantRadius"], this->importantRadius);

    SET_JSON_VALUE(jsonSettings, ["updateRate"], this->updateRate);

    SET_JSON_VALUE(jsonSettings, ["noRecoil"]["enabled"], this->noRecoil.enabled);
    SET_JSON_VALUE(jsonSettings, ["noRecoil"]["shootinggIntensity"], this->noRecoil.shootinggIntensity);
    SET_JSON_VALUE(jsonSettings, ["noRecoil"]["breathIntensity"], this->noRecoil.breathIntensity);
    SET_JSON_VALUE(jsonSettings, ["noRecoil"]["motionIntensity"], this->noRecoil.motionIntensity);

    SET_JSON_VALUE(jsonSettings, ["snapLines"]["activeMode"], this->snapLines.activeMode);
    SET_JSON_VALUE(jsonSettings, ["snapLines"]["types"][0], this->snapLines.types[0]);
    SET_JSON_VALUE(jsonSettings, ["snapLines"]["types"][1], this->snapLines.types[1]);
    SET_JSON_VALUE(jsonSettings, ["snapLines"]["types"][2], this->snapLines.types[2]);

    SET_JSON_VALUE(jsonSettings, ["boxESP"]["types"][0], this->boxESP.types[0]);
    SET_JSON_VALUE(jsonSettings, ["boxESP"]["types"][1], this->boxESP.types[1]);
    SET_JSON_VALUE(jsonSettings, ["boxESP"]["types"][2], this->boxESP.types[2]);
    SET_JSON_VALUE(jsonSettings, ["boxESP"]["factor"], this->boxESP.factor);

    SET_JSON_VALUE(jsonSettings, ["skeletonESP"]["types"][0], this->skeletonESP.types[0]);
    SET_JSON_VALUE(jsonSettings, ["skeletonESP"]["types"][1], this->skeletonESP.types[1]);
    SET_JSON_VALUE(jsonSettings, ["skeletonESP"]["types"][2], this->skeletonESP.types[2]);
    SET_JSON_VALUE(jsonSettings, ["skeletonESP"]["distance"], this->skeletonESP.distance);
    SET_JSON_VALUE(jsonSettings, ["skeletonESP"]["closeFOV"], this->skeletonESP.closeFOV);
    SET_JSON_VALUE(jsonSettings, ["skeletonESP"]["entities"], this->skeletonESP.entities);

    SET_JSON_VALUE(jsonSettings, ["lootESP"]["enabled"], this->lootESP.enabled);
    SET_JSON_VALUE(jsonSettings, ["lootESP"]["distance"], this->lootESP.distance);
    std::vector<std::string> lootESPFilters{};
    lootESPFilters.reserve(this->lootESP.filters.size());
    for (const auto& element : this->lootESP.filters) {
        lootESPFilters.push_back(std::string{element.begin(), element.end()});
    }

    SET_LIST_VALUE(jsonSettings, lootESPFilters, ["lootESP"]["filters"]);

    SET_JSON_VALUE(jsonSettings, ["keybinds"]["toggleNoRecoil"], this->keybinds.toggleNoRecoil);

    SET_JSON_VALUE(jsonSettings, ["debug"]["enabled"], this->debug.enabled);

    std::ofstream fs{"config.json"};
    if (fs.is_open()) {
        fs << jsonSettings.dump(4);
    }
}
