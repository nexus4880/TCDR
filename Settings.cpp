#include "Settings.hpp"
#include <fstream>
#include <filesystem>
#include <nlohmann/json.hpp>

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

    settings.showFPS = json["showFPS"];
    settings.visualizeImportantRadius = json["visualizeImportantRadius"];
    settings.importantRadius = json["importantRadius"];
    settings.updateRate = json["updateRate"];

    settings.noRecoil.enabled = json["noRecoil"]["enabled"];
    settings.noRecoil.shootinggIntensity = json["noRecoil"]["shootinggIntensity"];
    settings.noRecoil.breathIntensity = json["noRecoil"]["breathIntensity"];
    settings.noRecoil.motionIntensity = json["noRecoil"]["motionIntensity"];

    settings.snapLines.activeMode = json["snapLines"]["activeMode"];
    settings.snapLines.types[0] = json["snapLines"]["types"][0];
    settings.snapLines.types[1] = json["snapLines"]["types"][1];
    settings.snapLines.types[2] = json["snapLines"]["types"][2];

    settings.skeletonESP.distance = json["skeletonESP"]["distance"];
    settings.skeletonESP.closeFOV = json["skeletonESP"]["closeFOV"];
    settings.skeletonESP.entities = json["skeletonESP"]["entities"];

    settings.keybinds.toggleNoRecoil = json["keybinds"]["toggleNoRecoil"];

    settings.debug.enabled = json["debug"]["enabled"];

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
