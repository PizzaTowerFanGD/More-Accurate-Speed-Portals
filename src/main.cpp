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

double portalSpeeds(Speed speed, bool isPlayer1) {
    bool enabled     = Mod::get()->getSettingValue<bool>("enable-mod");
    bool randomSpeed = Mod::get()->getSettingValue<bool>("random-speed");
    bool enabled2p   = Mod::get()->getSettingValue<bool>("enable-2p-mod");

    // Player 1 settings
    float minSpeedSlowP1 = static_cast<float>(Mod::get()->getSettingValue<double>("min-speed-slow"));
    float maxSpeedSlowP1 = static_cast<float>(Mod::get()->getSettingValue<double>("max-speed-slow"));
    float minSpeedNormalP1 = static_cast<float>(Mod::get()->getSettingValue<double>("min-speed-normal"));
    float maxSpeedNormalP1 = static_cast<float>(Mod::get()->getSettingValue<double>("max-speed-normal"));
    float minSpeedFastP1 = static_cast<float>(Mod::get()->getSettingValue<double>("min-speed-fast"));
    float maxSpeedFastP1 = static_cast<float>(Mod::get()->getSettingValue<double>("max-speed-fast"));
    float minSpeedFasterP1 = static_cast<float>(Mod::get()->getSettingValue<double>("min-speed-faster"));
    float maxSpeedFasterP1 = static_cast<float>(Mod::get()->getSettingValue<double>("max-speed-faster"));
    float minSpeedFastestP1 = static_cast<float>(Mod::get()->getSettingValue<double>("min-speed-fastest"));
    float maxSpeedFastestP1 = static_cast<float>(Mod::get()->getSettingValue<double>("max-speed-fastest"));
    float halfSpeedP1 = static_cast<float>(Mod::get()->getSettingValue<double>("half-speed"));
    float fullSpeedP1 = static_cast<float>(Mod::get()->getSettingValue<double>("full-speed"));
    float doubleSpeedP1 = static_cast<float>(Mod::get()->getSettingValue<double>("double-speed"));
    float tripleSpeedP1 = static_cast<float>(Mod::get()->getSettingValue<double>("triple-speed"));
    float quadrupleSpeedP1 = static_cast<float>(Mod::get()->getSettingValue<double>("quadruple-speed"));

    // Player 2 settings
    float minSpeedSlowP2 = static_cast<float>(Mod::get()->getSettingValue<double>("min-speed-slow-p2"));
    float maxSpeedSlowP2 = static_cast<float>(Mod::get()->getSettingValue<double>("max-speed-slow-p2"));
    float minSpeedNormalP2 = static_cast<float>(Mod::get()->getSettingValue<double>("min-speed-normal-p2"));
    float maxSpeedNormalP2 = static_cast<float>(Mod::get()->getSettingValue<double>("max-speed-normal-p2"));
    float minSpeedFastP2 = static_cast<float>(Mod::get()->getSettingValue<double>("min-speed-fast-p2"));
    float maxSpeedFastP2 = static_cast<float>(Mod::get()->getSettingValue<double>("max-speed-fast-p2"));
    float minSpeedFasterP2 = static_cast<float>(Mod::get()->getSettingValue<double>("min-speed-faster-p2"));
    float maxSpeedFasterP2 = static_cast<float>(Mod::get()->getSettingValue<double>("max-speed-faster-p2"));
    float minSpeedFastestP2 = static_cast<float>(Mod::get()->getSettingValue<double>("min-speed-fastest-p2"));
    float maxSpeedFastestP2 = static_cast<float>(Mod::get()->getSettingValue<double>("max-speed-fastest-p2"));
    float halfSpeedP2 = static_cast<float>(Mod::get()->getSettingValue<double>("half-speed-p2"));
    float fullSpeedP2 = static_cast<float>(Mod::get()->getSettingValue<double>("full-speed-p2"));
    float doubleSpeedP2 = static_cast<float>(Mod::get()->getSettingValue<double>("double-speed-p2"));
    float tripleSpeedP2 = static_cast<float>(Mod::get()->getSettingValue<double>("triple-speed-p2"));
    float quadrupleSpeedP2 = static_cast<float>(Mod::get()->getSettingValue<double>("quadruple-speed-p2"));

    
    if (isPlayer1) {
        switch (speed) {
            case Speed::Slow: return randomSpeed ? speedRandomiser(minSpeedSlowP1, maxSpeedSlowP1) : halfSpeedP1;
            case Speed::Normal: return randomSpeed ? speedRandomiser(minSpeedNormalP1, maxSpeedNormalP1) : fullSpeedP1;
            case Speed::Fast: return randomSpeed ? speedRandomiser(minSpeedFastP1, maxSpeedFastP1) : doubleSpeedP1;
            case Speed::Faster: return randomSpeed ? speedRandomiser(minSpeedFasterP1, maxSpeedFasterP1) : tripleSpeedP1;
            case Speed::Fastest: return randomSpeed ? speedRandomiser(minSpeedFastestP1, maxSpeedFastestP1) : quadrupleSpeedP1;
        }
    } else {
        switch (speed) {
            case Speed::Slow: return randomSpeed ? speedRandomiser(minSpeedSlowP2, maxSpeedSlowP2) : halfSpeedP2;
            case Speed::Normal: return randomSpeed ? speedRandomiser(minSpeedNormalP2, maxSpeedNormalP2) : fullSpeedP2;
            case Speed::Fast: return randomSpeed ? speedRandomiser(minSpeedFastP2, maxSpeedFastP2) : doubleSpeedP2;
            case Speed::Faster: return randomSpeed ? speedRandomiser(minSpeedFasterP2, maxSpeedFasterP2) : tripleSpeedP2;
            case Speed::Fastest: return randomSpeed ? speedRandomiser(minSpeedFastestP2, maxSpeedFastestP2) : quadrupleSpeedP2;
        }
    }
    return 1.0; // Default speed if none match
}

// speed logic:
// player
class $modify(SpeedPlayer, PlayerObject) {
    void updateTimeMod(Speed speed, bool p1, bool isPlayer1) {
        float playerSpeed = portalSpeeds(speed, isPlayer1);
        PlayerObject::updateTimeMod(playerSpeed, p1);
        this->m_playerSpeed = playerSpeed;
    }
};

// assigning speeds to portals when starting level (p0->m_startSpeed)
class $modify(SpeedGJBGL, GJBaseGameLayer) {
    void updateTimeMod(Speed speed, bool p1, bool isPlayer1) {
        this->m_gameState.m_timeModRelated  = 0;
        this->m_gameState.m_timeModRelated2 = 0;
        as<SpeedPlayer *>(this->m_player1)->updateTimeMod(speed, p1, isPlayer1); 
        if (this->m_gameState.m_isDualMode)
            as<SpeedPlayer *>(this->m_player2)->updateTimeMod(speed, p1, !isPlayer1);
    }

    void setupLevelStart(LevelSettingsObject* p0) {
        GJBaseGameLayer::setupLevelStart(p0);

        bool isPlayer1 = !Mod::get()->getSettingValue<bool>("enable-mod-2p") ? true : (this->m_gameState.m_isDualMode ? false : true);
        SpeedPlayer *player = isPlayer1 ? as<SpeedPlayer *>(this->m_player1) : as<SpeedPlayer *>(this->m_player2);
        
        switch (p0->m_startSpeed) {
            case Speed::Slow:    player->updateTimeMod(Speed::Slow, 0, isPlayer1);    break;
            case Speed::Normal:  player->updateTimeMod(Speed::Normal, 0, isPlayer1);  break;
            case Speed::Fast:    player->updateTimeMod(Speed::Fast, 0, isPlayer1);    break;
            case Speed::Faster:  player->updateTimeMod(Speed::Faster, 0, isPlayer1);  break;
            case Speed::Fastest: player->updateTimeMod(Speed::Fastest, 0, isPlayer1); break;
        }
    }
};

class $modify(SpeedEffectGameObject, EffectGameObject) {
    void triggerObject(GJBaseGameLayer* p0, int p1, gd::vector<int> const* p2) {
        EffectGameObject::triggerObject(p0, p1, p2);

        bool isPlayer1 = !Mod::get()->getSettingValue<bool>("enable-mod-2p") ? true : (this->m_activatedByPlayer1 ? true : ((p0->m_gameState.m_isDualMode && this->m_activatedByPlayer2) ? false : true));
        SpeedGJBGL *gameLayer = as<SpeedGJBGL *>(p0);

        if (this->m_objectID == 200) // 0.5x portal
            gameLayer->updateTimeMod(Speed::Slow, this->m_hasNoEffects, isPlayer1);
        else if (this->m_objectID == 201) // 1x portal
            gameLayer->updateTimeMod(Speed::Normal, this->m_hasNoEffects, isPlayer1);
        else if (this->m_objectID == 202) // 2x portal
            gameLayer->updateTimeMod(Speed::Fast, this->m_hasNoEffects, isPlayer1);
        else if (this->m_objectID == 203) // 3x portal
            gameLayer->updateTimeMod(Speed::Faster, this->m_hasNoEffects, isPlayer1);
        else if (this->m_objectID == 1334) // 4x portal
            gameLayer->updateTimeMod(Speed::Fastest, this->m_hasNoEffects, isPlayer1);
    }
};
