//
// Created by David Burchill on 2023-09-28.
//

#include "EntityManager.h"
#include "Entity.h"

EntityManager::EntityManager() : m_totalEntities(0)  {}


std::shared_ptr<Entity> EntityManager::addEntity(const std::string &tag) {
    auto e = std::shared_ptr<Entity>(new Entity(m_totalEntities++, tag));   // create a new Entity object
    m_EntitiesToAdd.push_back(e);       // store it in entities vector 
    return e;                           // return shared pointer to it
}


EntityVec &EntityManager::getEntities(const std::string &tag) {
    return m_entityMap[tag];
}


void EntityManager::update() {

    // Remove dead entities
    removeDeadEntities(m_entities);
    for (auto& [_, entityVec] : m_entityMap)
        removeDeadEntities(entityVec);
     

    // add new entities
    for (auto e : m_EntitiesToAdd)
    {
        m_entities.push_back(e);
        m_entityMap[e->getTag()].push_back(e);
    }
    m_EntitiesToAdd.clear();
}


EntityVec &EntityManager::getEntities() {
    return m_entities;
}


void EntityManager::removeDeadEntities(EntityVec &v) {
    v.erase(std::remove_if(v.begin(), v.end(), [](auto e) {return!(e->isActive()); }), v.end());
}
