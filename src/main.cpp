#include <Geode/Geode.hpp>

using namespace geode::prelude;

#include <Geode/modify/PlayerObject.hpp>
#include <Geode/modify/GJBaseGameLayer.hpp>
#include <Geode/modify/EffectGameObject.hpp>

// Load the settings outside of a function
bool enabled = Mod::get()->getSettingValue<bool>("enable-mod");
float speed_0_5x = static_cast<float>(Mod::get()->getSettingValue<double>("speed-adjustment-0.5x"));
float speed_1x = static_cast<float>(Mod::get()->getSettingValue<double>("speed-adjustment-1x"));
float speed_2x = static_cast<float>(Mod::get()->getSettingValue<double>("speed-adjustment-2x"));
float speed_3x = static_cast<float>(Mod::get()->getSettingValue<double>("speed-adjustment-3x"));
float speed_4x = static_cast<float>(Mod::get()->getSettingValue<double>("speed-adjustment-4x"));

double speedSpeed(Speed speed) {
    switch (speed) {
        case Speed::Slow: {
            if (enabled)
                return speed_0_5x;
            else
                return 0.7f;
        }
        case Speed::Normal: {
            if (enabled)
                return speed_1x;
            else
                return 0.9f;
        }
        case Speed::Fast: {
            if (enabled)
                return speed_2x;
            else
                return 1.1f;
        }
        case Speed::Faster: {
            if (enabled)
                return speed_3x;
            else
                return 1.3f;
        }
        case Speed::Fastest: {
            if (enabled)
                return speed_4x;
            else
                return 1.6f;
        }
    }
}

class $modify(MyPlayer, PlayerObject) {
    void updateTimeMod(Speed speed, bool p1) {
        double playerSpeed = speedSpeed(speed);
        PlayerObject::updateTimeMod(playerSpeed, p1);
        this->m_playerSpeed = playerSpeed;
    }
};

class $modify(MyGJBGL, GJBaseGameLayer) {
    void updateTimeMod(Speed speed, bool p1, bool p2) {
        this->m_gameState.m_timeModRelated  = 0;
        this->m_gameState.m_timeModRelated2 = 0;
        as<MyPlayer *>(this->m_player1)->updateTimeMod(speed, p2);
        if (this->m_gameState.m_isDualMode)
            as<MyPlayer *>(this->m_player2)->updateTimeMod(speed, p2);
    }

    void setupLevelStart(LevelSettingsObject* p0) {
        GJBaseGameLayer::setupLevelStart(p0);

        switch (p0->m_startSpeed) {
            case Speed::Slow: {
                as<MyPlayer *>(this->m_player1)->updateTimeMod(Speed::Slow, 0);
                if (this->m_gameState.m_isDualMode)
                    as<MyPlayer *>(this->m_player2)->updateTimeMod(Speed::Slow, 0);
                break;
            }
            case Speed::Normal: {
                as<MyPlayer *>(this->m_player1)->updateTimeMod(Speed::Normal, 0);
                if (this->m_gameState.m_isDualMode)
                    as<MyPlayer *>(this->m_player2)->updateTimeMod(Speed::Normal, 0);
                break;
            }
            case Speed::Fast: {
                as<MyPlayer *>(this->m_player1)->updateTimeMod(Speed::Fast, 0);
                if (this->m_gameState.m_isDualMode)
                    as<MyPlayer *>(this->m_player2)->updateTimeMod(Speed::Fast, 0);
                break;
            }
            case Speed::Faster: {
                as<MyPlayer *>(this->m_player1)->updateTimeMod(Speed::Faster, 0);
                if (this->m_gameState.m_isDualMode)
                    as<MyPlayer *>(this->m_player2)->updateTimeMod(Speed::Faster, 0);
                break;
            }
            case Speed::Fastest: {
                as<MyPlayer *>(this->m_player1)->updateTimeMod(Speed::Fastest, 0);
                if (this->m_gameState.m_isDualMode)
                    as<MyPlayer *>(this->m_player2)->updateTimeMod(Speed::Fastest, 0);
                break;
            }
        }
    }
};

class $modify(EffectGameObject) {
    void triggerObject(GJBaseGameLayer* p0, int p1, gd::vector<int> const* p2) {
        EffectGameObject::triggerObject(p0, p1, p2);

        if (this->m_objectID == 200) // 0.5x portal
            as<MyGJBGL *>(p0)->updateTimeMod(Speed::Slow, 0, this->m_hasNoEffects);
        else if (this->m_objectID == 201) // 1x portal
            as<MyGJBGL *>(p0)->updateTimeMod(Speed::Normal, 0, this->m_hasNoEffects);
        else if (this->m_objectID == 202) // 2x portal
            as<MyGJBGL *>(p0)->updateTimeMod(Speed::Fast, 0, this->m_hasNoEffects);
        else if (this->m_objectID == 203) // 3x portal
            as<MyGJBGL *>(p0)->updateTimeMod(Speed::Faster, 0, this->m_hasNoEffects);
        else if (this->m_objectID == 1334) // 4x portal
            as<MyGJBGL *>(p0)->updateTimeMod(Speed::Fastest, 0, this->m_hasNoEffects);
    }
};
