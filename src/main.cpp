#include <Geode/Geode.hpp>

using namespace geode::prelude;

#include <Geode/modify/PlayerObject.hpp>
#include <Geode/modify/GJBaseGameLayer.hpp>
#include <Geode/modify/EffectGameObject.hpp>

// for the randomised numbers
#include <random>

// randomised speed logic
float speedRandomiser(float minSpeed, float maxSpeed) {
    static std::default_random_engine e;
    static std::uniform_real_distribution<float> dis(minSpeed, maxSpeed);
    return dis(e);
}

// assigning float values to Speed enum members
float portalSpeeds(Speed speed) {
    // Load the settings (originally we wanted to load the settings outside of the function but it was causing issues):
    // bool settings
    bool enabled     = Mod::get()->getSettingValue<bool>("enable-mod");
    bool randomSpeed = Mod::get()->getSettingValue<bool>("random-speed");

    // min and max speeds for randomisation
    float minSpeedSlow = static_cast<float>(Mod::get()->getSettingValue<double>("min-speed-slow"));
    float maxSpeedSlow = static_cast<float>(Mod::get()->getSettingValue<double>("max-speed-slow"));

    float minSpeedNormal = static_cast<float>(Mod::get()->getSettingValue<double>("min-speed-normal"));
    float maxSpeedNormal = static_cast<float>(Mod::get()->getSettingValue<double>("max-speed-normal"));

    float minSpeedFast = static_cast<float>(Mod::get()->getSettingValue<double>("min-speed-fast"));
    float maxSpeedFast = static_cast<float>(Mod::get()->getSettingValue<double>("max-speed-fast"));

    float minSpeedFaster = static_cast<float>(Mod::get()->getSettingValue<double>("min-speed-faster"));
    float maxSpeedFaster = static_cast<float>(Mod::get()->getSettingValue<double>("max-speed-faster"));

    float minSpeedFastest = static_cast<float>(Mod::get()->getSettingValue<double>("min-speed-fastest"));
    float maxSpeedFastest = static_cast<float>(Mod::get()->getSettingValue<double>("max-speed-fastest"));

    // singular speed values for when randomisation is disabled
    float halfSpeed      = static_cast<float>(Mod::get()->getSettingValue<double>("half-speed"));
    float fullSpeed      = static_cast<float>(Mod::get()->getSettingValue<double>("full-speed"));
    float doubleSpeed    = static_cast<float>(Mod::get()->getSettingValue<double>("double-speed"));
    float tripleSpeed    = static_cast<float>(Mod::get()->getSettingValue<double>("triple-speed"));
    float quadrupleSpeed = static_cast<float>(Mod::get()->getSettingValue<double>("quadruple-speed"));

    switch (speed) {
        case Speed::Slow: {
            if (enabled) {
                if(!randomSpeed)
                    return halfSpeed;
                else
                    return speedRandomiser(minSpeedSlow, maxSpeedSlow);
            }
            else
                return 0.7;
        }
        case Speed::Normal: {
            if (enabled) {
                if(!randomSpeed)
                    return fullSpeed;
                else
                    return speedRandomiser(minSpeedNormal, maxSpeedNormal);
            }
            else
                return 0.9;
        }
        case Speed::Fast: {
            if (enabled) {
                if(!randomSpeed)
                    return doubleSpeed;
                else
                    return speedRandomiser(minSpeedFast, maxSpeedFast);
            }
            else
                return 1.1;
        }
        case Speed::Faster: {
            if (enabled) {
                if(!randomSpeed)
                    return tripleSpeed;
                else
                    return speedRandomiser(minSpeedFaster, maxSpeedFaster);
            }
            else
                return 1.3;
        }
        case Speed::Fastest: {
            if (enabled) {
                if(!randomSpeed)
                    return quadrupleSpeed;
                else
                    return speedRandomiser(minSpeedFastest, maxSpeedFastest);
            }
            else
                return 1.6;
        }
    }
}
void doSettingsUpdate() {
    // Load the settings (again):
    // bool settings
    enabled     = Mod::get()->getSettingValue<bool>("enable-mod");
    randomSpeed = Mod::get()->getSettingValue<bool>("random-speed");

    // min and max speeds for randomisation
    minSpeedSlow = static_cast<float>(Mod::get()->getSettingValue<double>("min-speed-slow"));
    maxSpeedSlow = static_cast<float>(Mod::get()->getSettingValue<double>("max-speed-slow"));

    minSpeedNormal = static_cast<float>(Mod::get()->getSettingValue<double>("min-speed-normal"));
    maxSpeedNormal = static_cast<float>(Mod::get()->getSettingValue<double>("max-speed-normal"));

    minSpeedFast = static_cast<float>(Mod::get()->getSettingValue<double>("min-speed-fast"));
    maxSpeedFast = static_cast<float>(Mod::get()->getSettingValue<double>("max-speed-fast"));

    minSpeedFaster = static_cast<float>(Mod::get()->getSettingValue<double>("min-speed-faster"));
    maxSpeedFaster = static_cast<float>(Mod::get()->getSettingValue<double>("max-speed-faster"));

    minSpeedFastest = static_cast<float>(Mod::get()->getSettingValue<double>("min-speed-fastest"));
    maxSpeedFastest = static_cast<float>(Mod::get()->getSettingValue<double>("max-speed-fastest"));

    // singular speed values for when randomisation is disabled
    halfSpeed      = static_cast<float>(Mod::get()->getSettingValue<double>("half-speed"));
    fullSpeed      = static_cast<float>(Mod::get()->getSettingValue<double>("full-speed"));
    doubleSpeed    = static_cast<float>(Mod::get()->getSettingValue<double>("double-speed"));
    tripleSpeed    = static_cast<float>(Mod::get()->getSettingValue<double>("triple-speed"));
    quadrupleSpeed = static_cast<float>(Mod::get()->getSettingValue<double>("quadruple-speed"));
}
// speed logic:
// player
class $modify(SpeedPlayer, PlayerObject) {
    void updateTimeMod(Speed speed, bool p1) {
        float playerSpeed = portalSpeeds(speed);
        PlayerObject::updateTimeMod(playerSpeed, p1);
        this->m_playerSpeed = playerSpeed;
    }
};

// assigning speeds to portals when starting level (p0->m_startSpeed)
class $modify(SpeedGJBGL, GJBaseGameLayer) {
    void updateTimeMod(Speed speed, bool p1) {
        this->m_gameState.m_timeModRelated  = 0;
        this->m_gameState.m_timeModRelated2 = 0;
        as<SpeedPlayer *>(this->m_player1)->updateTimeMod(speed, p1); 
        if (this->m_gameState.m_isDualMode)
            as<SpeedPlayer *>(this->m_player2)->updateTimeMod(speed, p1);
    }

    void setupLevelStart(LevelSettingsObject* p0) {
        GJBaseGameLayer::setupLevelStart(p0);
        doSettingsUpdate(); // make changing settings NOT require a restart
        switch (p0->m_startSpeed) {
            case Speed::Slow: {
                as<SpeedPlayer *>(this->m_player1)->updateTimeMod(Speed::Slow, 0);
                if (this->m_gameState.m_isDualMode)
                    as<SpeedPlayer *>(this->m_player2)->updateTimeMod(Speed::Slow, 0);
                break;
            }
            case Speed::Normal: {
                as<SpeedPlayer *>(this->m_player1)->updateTimeMod(Speed::Normal, 0);
                if (this->m_gameState.m_isDualMode)
                    as<SpeedPlayer *>(this->m_player2)->updateTimeMod(Speed::Normal, 0);
                break;
            }
            case Speed::Fast: {
                as<SpeedPlayer *>(this->m_player1)->updateTimeMod(Speed::Fast, 0);
                if (this->m_gameState.m_isDualMode)
                    as<SpeedPlayer *>(this->m_player2)->updateTimeMod(Speed::Fast, 0);
                break;
            }
            case Speed::Faster: {
                as<SpeedPlayer *>(this->m_player1)->updateTimeMod(Speed::Faster, 0);
                if (this->m_gameState.m_isDualMode)
                    as<SpeedPlayer *>(this->m_player2)->updateTimeMod(Speed::Faster, 0);
                break;
            }
            case Speed::Fastest: {
                as<SpeedPlayer *>(this->m_player1)->updateTimeMod(Speed::Fastest, 0);
                if (this->m_gameState.m_isDualMode)
                    as<SpeedPlayer *>(this->m_player2)->updateTimeMod(Speed::Fastest, 0);
                break;
            }
        }
    }
};

class $modify(SpeedEffectGameObject, EffectGameObject) {
    void triggerObject(GJBaseGameLayer* p0, int p1, gd::vector<int> const* p2) {
        EffectGameObject::triggerObject(p0, p1, p2);

        if (this->m_objectID == 200) // 0.5x portal
            as<SpeedGJBGL *>(p0)->updateTimeMod(Speed::Slow, this->m_hasNoEffects);
        else if (this->m_objectID == 201) // 1x portal
            as<SpeedGJBGL *>(p0)->updateTimeMod(Speed::Normal, this->m_hasNoEffects);
        else if (this->m_objectID == 202) // 2x portal
            as<SpeedGJBGL *>(p0)->updateTimeMod(Speed::Fast, this->m_hasNoEffects);
        else if (this->m_objectID == 203) // 3x portal
            as<SpeedGJBGL *>(p0)->updateTimeMod(Speed::Faster, this->m_hasNoEffects);
        else if (this->m_objectID == 1334) // 4x portal
            as<SpeedGJBGL *>(p0)->updateTimeMod(Speed::Fastest, this->m_hasNoEffects);
    }
};
