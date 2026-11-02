#include <Geode/Geode.hpp>
#include <Geode/modify/PlayLayer.hpp>
#include <Geode/modify/PlayerObject.hpp>

using namespace geode::prelude;

class $modify(NexusHelperPlayLayer, PlayLayer) {
    struct Fields {
        bool clickPending = false;
        float pendingTimer = 0.0f;
        const float maxToleranceSeconds = 3.0f; // Margen de hasta 3 segundos para corregir el clic
        PlayerButton targetButton = PlayerButton::Jump;
    };

    void postUpdate(float dt) {
        PlayLayer::postUpdate(dt);

        if (m_fields->clickPending) {
            m_fields->pendingTimer += dt;

            // Evalúa si el jugador entra en una posición segura en este tick
            if (this->isPlayerInSafeState()) {
                PlayLayer::pushButton(m_fields->targetButton);
                m_fields->clickPending = false;
                m_fields->pendingTimer = 0.0f;
            } 
            // Si el lazo supera los 3 segundos, se cancela para evitar bloqueos
            else if (m_fields->pendingTimer >= m_fields->maxToleranceSeconds) {
                m_fields->clickPending = false;
                m_fields->pendingTimer = 0.0f;
            }
        }
    }

    void pushButton(PlayerButton button) {
        if (button == PlayerButton::Jump) {
            // Si el salto provocaría fallo inmediato, se retiene en el lazo de corrección
            if (!this->isPlayerInSafeState()) {
                m_fields->clickPending = true;
                m_fields->pendingTimer = 0.0f;
                m_fields->targetButton = button;
                return;
            }
        }
        PlayLayer::pushButton(button);
    }

    void releaseButton(PlayerButton button) {
        PlayLayer::releaseButton(button);
    }

    bool isPlayerInSafeState() {
        if (!m_player1) return false;

        bool isOnGround = m_player1->m_isOnGround;
        bool isTouchingRing = m_player1->m_touchingRings && m_player1->m_touchingRings->count() > 0;
        bool isHolding = m_player1->m_isHolding;

        return (isOnGround || isTouchingRing || isHolding);
    }
};
