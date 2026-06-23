#pragma once

#include <glm/glm.hpp>
#include <vector>

// 每个游戏物体 (玩家, 敌人) 的数据
struct GameObject {
    glm::vec2 position;     
    glm::vec2 velocity;     // 速度
    bool isPlayer;          // true = 玩家, false = 敌人
};

class Scene{
public:
    Scene();

    void AddPlayer(const glm::vec2& pos);
    void AddEnemy(const glm::vec2& pos);

    void Update();      // 更新所有物体的位置, AI等
    void Render();      // 渲染所有物体 (需要外部传入着色器和VAO)

    // 获取数据, 供 Engine。绘制使用
    const std::vector<GameObject>& GetObjects() const { return m_objects; }

    // 重置场景
    void Reset();

    void SetPlayerPosition(const glm::vec2& pos);
    glm::vec2 GetPlayerPosition() const;

private:
    std::vector<GameObject> m_objects;
    float m_time = 0.0f;
    // 追踪到玩家索引 (方便AI找到目标)
    int m_playerIndex = -1;
};