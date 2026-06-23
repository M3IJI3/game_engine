#include "Scene.h"
#include <glm/gtc/matrix_transform.hpp>

Scene::Scene(){}

void Scene::AddPlayer(const glm::vec2& pos){
    GameObject obj;
    obj.position = pos;
    obj.isPlayer = true;
    m_objects.push_back(obj);
    m_playerIndex = m_objects.size() - 1;
}

void Scene::AddEnemy(const glm::vec2& pos){
    GameObject obj;
    obj.position = pos;
    obj.isPlayer = false;
    m_objects.push_back(obj);
}

void Scene::Update(){
    m_time += 0.016f;       // 约 60FPS

    if(m_objects.empty() || m_playerIndex == -1) return;

    // 找玩家
    glm::vec2 playerPos = m_objects[m_playerIndex].position;

    // 更新每个物体
    for(auto& obj : m_objects){
        if(obj.isPlayer){
            // 玩家不动, 由键盘控制
            // 位置由外部通过 SetPlayerPosition 修改
        } else {
            // 敌人: 向玩家靠近
            glm::vec2 dir = playerPos - obj.position;
            float len = glm::length(dir);
            if(len > 0.01f){
                glm::vec2 normalized = dir / len;
                float speed = 0.015f;
                obj.position += normalized * speed;
            }
        }
    }

    // 碰撞检测: 敌人碰到玩家就重置
    for(auto& obj : m_objects){
        if(!obj.isPlayer){
            float dist = glm::length(obj.position - playerPos);
            if(dist < 0.15f){
                Reset();
                break;
            }
        }
    }
}

void Scene::Render(){
    // 渲染由 Engine 负责, Scene 只提供数据
}

void Scene::Reset(){
    if(m_objects.empty()) return;

    for(auto& obj : m_objects){
        if(obj.isPlayer){
            obj.position = glm::vec2(0.0f, 0.0f);
        } else {
            obj.position = glm::vec2(0.5f, 0.5f);
        }
    }
}

void Scene::SetPlayerPosition(const glm::vec2& pos){
    if(m_playerIndex >= 0 && m_playerIndex < (int)m_objects.size()){
        m_objects[m_playerIndex].position = pos;
    }
}

glm::vec2 Scene::GetPlayerPosition() const{
    if(m_playerIndex >= 0 && m_playerIndex < (int)m_objects.size()){
        return m_objects[m_playerIndex].position;
    }
    return glm::vec2(0.0f, 0.0f);
}