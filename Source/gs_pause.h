#ifndef GS_PAUSE_H
#define GS_PAUSE_H

#include "game_state.h"

#include <Urho3D/Graphics/Light.h>
#include <Urho3D/Graphics/Model.h>
#include <Urho3D/Graphics/StaticModel.h>
#include <Urho3D/Graphics/Material.h>
#include <Urho3D/Graphics/Skybox.h>
#include <Urho3D/Engine/Application.h>
#include <Urho3D/Core/CoreEvents.h>
#include <Urho3D/Input/Input.h>
#include <Urho3D/Input/InputEvents.h>

/// Pauses Urho and is stacked on top of the playing game state and called by that when pressing the escape key.
class gs_pause : public game_state
{
public:
    gs_pause();
    ~gs_pause();
    void HandleKeyDown(Urho3D::StringHash eventType,Urho3D::VariantMap& eventData);

    URHO3D_OBJECT(gs_pause,game_state);
};

#endif // GS_PAUSE_H
