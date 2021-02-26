#include <MyGL/Animation.hpp>

using namespace my;

Animation::Animation() : m_duration(1.0f), m_originalPos(0.0f, 0.0f), m_targetPos(0.0f, 0.0f),
    m_positionStep(0.0f, 0.0f), m_frametime(0.0f), p_shape(nullptr), m_running(false)
{}

Animation::Animation(AbstractShape& shape) : Animation(shape, 1.0f, 0.0f, 0.0f) {}

Animation::Animation(AbstractShape& shape, float duration, float destX, float destY) :
    m_duration(glm::abs(duration)), m_originalPos(shape.getPosition()),
    m_targetPos(destX, destY), m_positionStep((m_targetPos - m_originalPos) / m_duration),
    m_frametime(0.0f), p_shape(&shape), m_running(false)
{}

void Animation::start() {
    if (!m_running) {
        m_running = true;
        m_frametime = SDL_GetTicks() / 1000.0f;
    }
}

void Animation::attach(AbstractShape& shape) {
    p_shape = &shape;
    m_originalPos = p_shape->getPosition();
}

void Animation::reset() {
    p_shape->setPosition(m_originalPos, true);
    m_running = false;
}

void Animation::setDuration(float duration) {
    m_duration = glm::abs(duration);
    m_positionStep = (m_targetPos - p_shape->getPosition()) / m_duration;
}

void Animation::setTargetPosition(float x, float y) {
    setTargetPosition(glm::vec2(x, y));
}

void Animation::setTargetPosition(const glm::vec2& pos) {
    m_targetPos = pos;
    m_positionStep = (m_targetPos - p_shape->getPosition()) / m_duration;
}

void Animation::setSpeed(float speed) {

}

bool Animation::isOver() const {
    return !m_running;
}

void Animation::draw(const glm::mat4& lookAt, const glm::mat4& projection) {
    if (m_running) {
        float nbOfTicks = SDL_GetTicks() / 1000.0f;
        m_frametime = nbOfTicks - m_frametime;
        p_shape->move(m_positionStep.x * m_frametime, m_positionStep.y * m_frametime);
        m_frametime = nbOfTicks;
        if (glm::distance(m_targetPos, p_shape->getPosition()) < 1.0f) m_running = false;
    }
    p_shape->draw(lookAt, projection);
}