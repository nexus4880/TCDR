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
    settings.importantRadius = TRY_READ(float, json, ["visualizeImportantRadius"]);
    settings.updateRate = TRY_READ(float, json, ["visualizeImportantRadius"]);

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

    settings.skeletonESP.distance = TRY_READ(float, json, ["skeletonESP"]["distance"]);
    settings.skeletonESP.closeFOV = TRY_READ(float, json, ["skeletonESP"]["closeFOV"]);
    settings.skeletonESP.entities = TRY_READ(int, json, ["skeletonESP"]["entities"]);

    settings.keybinds.toggleNoRecoil = TRY_READ(int, json, ["keybinds"]["toggleNoRecoil"]);

    settings.debug.enabled = TRY_READ(int, json, ["debug"]["enabled"]);

    return settings;
}

void Settings::Serialize() {
    nlohmann::json jsonSettings;

    jsonSettings["showFPS"] = this->showFPS;

    jsonSettings["visualizeImportantRadius"] = this->visualizeImportantRadius;
    jsonSettings["importantRadius"] = this->importantRadius;

    jsonSettings["updateRate"] = this->updateRate;

    jsonSettings["noRecoil"]["enabled"] = this->noRecoil.enabled;
    jsonSettings["noRecoil"]["shootinggIntensity"] = this->noRecoil.shootinggIntensity;
    jsonSettings["noRecoil"]["breathIntensity"] = this->noRecoil.breathIntensity;
    jsonSettings["noRecoil"]["motionIntensity"] = this->noRecoil.motionIntensity;

    jsonSettings["snapLines"]["activeMode"] = this->snapLines.activeMode;
    jsonSettings["snapLines"]["types"][0] = this->snapLines.types[0];
    jsonSettings["snapLines"]["types"][1] = this->snapLines.types[1];
    jsonSettings["snapLines"]["types"][2] = this->snapLines.types[2];

    jsonSettings["boxESP"]["types"][0] = this->boxESP.types[0];
    jsonSettings["boxESP"]["types"][1] = this->boxESP.types[1];
    jsonSettings["boxESP"]["types"][2] = this->boxESP.types[2];
    jsonSettings["boxESP"]["factor"] = this->boxESP.factor;

    jsonSettings["skeletonESP"]["distance"] = this->skeletonESP.distance;
    jsonSettings["skeletonESP"]["closeFOV"] = this->skeletonESP.closeFOV;
    jsonSettings["skeletonESP"]["entities"] = this->skeletonESP.entities;

    jsonSettings["keybinds"]["toggleNoRecoil"] = this->keybinds.toggleNoRecoil;

    jsonSettings["debug"]["enabled"] = this->debug.enabled;

    std::ofstream fs{"config.json"};
    if (fs.is_open()) {
        fs << jsonSettings.dump(4);
    }
}
