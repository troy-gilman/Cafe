#pragma once

#include "Event.hpp"
#include "State.hpp"
#include "ObjectPool.hpp"
#include <vector>
#include <queue>


class EventHandler {
private:
    std::queue<Event*> processedEvents;
    ObjectPool<Event>* eventPool;

public:
    EventHandler(ObjectPool<Event>* eventPool);
    ~EventHandler();

    bool hasProcessedEvent() const;
    size_t getNumProcessedEvents() const;
    Event* popProcessedEvent();
    void handleEvent(State* state, Event* event);

private:
    void static handleEventEntityCreate(State* state, Event* event);
    void static handleEventEntityDestroy(State* state, Event* event);
    void static handleEventEntityUpdateCameraDistanceFromTarget(State* state, Event* event);
    void static handleEventEntityGetCameraDistanceFromTarget(State* state, Event* event);
    void static handleEventEntityUpdateCameraVerticalAngle(State* state, Event* event);
    void static handleEventEntityGetCameraVerticalAngle(State* state, Event* event);
    void static handleEventEntityUpdateLightColor(State* state, Event* event);
    void static handleEventEntityGetLightColor(State* state, Event* event);
    void static handleEventEntityUpdateLightAttenuation(State* state, Event* event);
    void static handleEventEntityGetLightAttenuation(State* state, Event* event);
    void static handleEventEntityUpdateRenderable3DMeshAssetID(State* state, Event* event);
    void static handleEventEntityGetRenderable3DMeshAssetID(State* state, Event* event);
    void static handleEventEntityUpdateRenderable3DMaterialAssetID(State* state, Event* event);
    void static handleEventEntityGetRenderable3DMaterialAssetID(State* state, Event* event);
    void static handleEventEntityUpdateRenderable3DTextureAtlasIndex(State* state, Event* event);
    void static handleEventEntityGetRenderable3DTextureAtlasIndex(State* state, Event* event);
    void static handleEventEntityUpdateSpatial3DPosition(State* state, Event* event);
    void static handleEventEntityGetSpatial3DPosition(State* state, Event* event);
    void static handleEventEntityUpdateSpatial3DRotation(State* state, Event* event);
    void static handleEventEntityGetSpatial3DRotation(State* state, Event* event);
    void static handleEventEntityUpdateSpatial3DScale(State* state, Event* event);
    void static handleEventEntityGetSpatial3DScale(State* state, Event* event);


    


};