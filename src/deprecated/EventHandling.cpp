#include "EventHandling.h"
#include "../util/MapUtils.h"

void handleEventEntityCreate(Event::Event* event, ECS::EntityComponentSystem* ecs) {
    event->success = false;
    UUID id = event->field1.field_UUID;
    if (id >= ECS::MAX_ENTITIES) return;
    ECS::Entity* entity = new ECS::Entity();
    entity->id = id;
    ecs->entities[id] = entity;
    event->success = true;
}

void handleEventEntityDestroy(Event::Event* event, ECS::EntityComponentSystem* ecs) {
    event->success = false;
    UUID entityId = event->field1.field_UUID;
    if (entityId >= ecs->nextEntityId) return;
    ECS::Entity* entity = ecs->entities[entityId];
    if (entity == nullptr) return;
    delete entity;
    ecs->entities[entityId] = nullptr;
    event->success = true;
}

void handleEventEntityAddComponent(Event::Event* event, ECS::EntityComponentSystem* ecs) {
    event->success = false;
    UUID entityId = event->field1.field_UUID;
    i32 componentType = event->field2.field_Integer;
    if (entityId >= ecs->nextEntityId) return;
    ECS::Entity* entity = ecs->entities[entityId];
    if (entity == nullptr) return;
    ECS::Component *component = new ECS::Component();
    component->type = componentType;
    entity->components[componentType] = component;
    event->success = true;
}

void handleEventEntityRemoveComponent(Event::Event* event, ECS::EntityComponentSystem* ecs) {
    event->success = false;
    UUID entityId = event->field1.field_UUID;
    i32 componentType = event->field2.field_Integer;
    if (entityId >= ecs->nextEntityId) return;
    ECS::Entity* entity = ecs->entities[entityId];
    if (entity == nullptr) return;
    ECS::Component* component = entity->components[componentType];
    if (component == nullptr) return;
    delete component;
    entity->components[componentType] = nullptr;
    event->success = true;
}

void handleEventSetComponentField(Event::Event* event, ECS::EntityComponentSystem* ecs) {
    event->success = false;
    UUID entityId = event->field1.field_UUID;
    i32 componentType = event->field2.field_Integer;
    i32 fieldIndex = event->field3.field_Integer;
    if (entityId >= ecs->nextEntityId) return;
    ECS::Entity* entity = ecs->entities[entityId];
    if (entity == nullptr) return;
    ECS::Component* component = entity->components[componentType];
    if (component == nullptr) return;
    if (componentType >= ecs->numComponentTypes) return;
    ECS::ComponentInfo* componentInfo = ecs->componentTypes[componentType];
    if (componentInfo == nullptr) return;
    ECS::setField_f32(component, componentInfo, fieldIndex, event->field4.field_Float);
    event->success = true;
}

void handleEventGetComponentField(Event::Event* event, ECS::EntityComponentSystem* ecs) {
    event->success = false;
    UUID entityId = event->field1.field_UUID;
    i32 componentType = event->field2.field_Integer;
    i32 fieldIndex = event->field3.field_Integer;
    if (entityId >= ecs->nextEntityId) return;
    ECS::Entity* entity = ecs->entities[entityId];
    if (entity == nullptr) return;
    ECS::Component* component = entity->components[componentType];
    if (component == nullptr) return;
    if (componentType >= ecs->numComponentTypes) return;
    ECS::ComponentInfo* componentInfo = ecs->componentTypes[componentType];
    if (componentInfo == nullptr) return;
    event->field4.field_Float = ECS::getField_f32(component, componentInfo, fieldIndex);
    event->success = true;
}

bool EventHandling::handleNextEvent(Event::EventState* eventState, ECS::EntityComponentSystem* ecs) {
    Event::Event *event = eventState->eventQueue.pop();
    switch (event->eventType) {
        case Event::EventType::NONE:
            break;
        case Event::EventType::ENTITY_CREATE:
            handleEventEntityCreate(event, ecs);
            break;
        case Event::EventType::ENTITY_DESTROY:
            handleEventEntityDestroy(event, ecs);
            break;
        case Event::EventType::ENTITY_ADD_COMPONENT:
            handleEventEntityAddComponent(event, ecs);
            break;
        case Event::EventType::ENTITY_REMOVE_COMPONENT:
            handleEventEntityRemoveComponent(event, ecs);
            break;
        case Event::EventType::COMPONENT_SET_FIELD:
            handleEventSetComponentField(event, ecs);
            break;
        case Event::EventType::COMPONENT_GET_FIELD:
            handleEventGetComponentField(event, ecs);
            break;
        default:
            event->success = false;
            break;
    }

    event->isProcessed = true;
    event->success = true;
    bool success = event->success;

    if (event->success) {
        eventState->eventQueueProcessed.push(event);
    } else {
        eventState->eventPool.returnObject(event);
    }
    return success;
}