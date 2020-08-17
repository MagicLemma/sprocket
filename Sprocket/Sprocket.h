// 
// THE SPROCKET ENGINE
//

// CORE
#include "Core/Window.h"
#include "Core/SceneManager.h"
#include "Core/ModelManager.h"
#include "Core/TextureManager.h"
#include "Core/Layer.h"
#include "Core/Scene.h"

// UI
#include "UI/Font/FontAtlas.h"
#include "UI/Font/Font.h"
#include "UI/Font/Glyph.h"

#include "UI/UIEngine.h"
#include "UI/SimpleUI.h"
#include "UI/SinglePanelUI.h"
#include "UI/DevUI.h"

// ENTITIES
#include "Entities/Entity.h"
#include "Entities/Components.h"
#include "Entities/EntityManager.h"
#include "Entities/EntitySystem.h"
#include "Entities/Serialiser.h"

#include "Entities/Systems/BasicSelector.h"
#include "Entities/Systems/Selector.h"
#include "Entities/Systems/PhysicsEngine.h"
#include "Entities/Systems/PlayerMovement.h"
#include "Entities/Systems/ScriptRunner.h"
#include "Entities/Systems/GameGrid.h"
#include "Entities/Systems/PathFollower.h"

// EVENTS
#include "Events/Event.h"
#include "Events/KeyboardEvent.h"
#include "Events/MouseEvent.h"
#include "Events/WindowEvent.h"

// UTILITY
#include "Utility/Adaptors.h"
#include "Utility/Log.h"
#include "Utility/Colour.h"
#include "Utility/FileBrowser.h"
#include "Utility/HashPair.h"
#include "Utility/Maths.h"
#include "Utility/KeyboardProxy.h"
#include "Utility/KeyboardCodes.h"
#include "Utility/MouseProxy.h"
#include "Utility/Random.h"
#include "Utility/MouseCodes.h"
#include "Utility/Stopwatch.h"
#include "Utility/Tokenize.h"

// GRAPHICS
#include "Graphics/RenderContext.h"
#include "Graphics/Shader.h"

#include "Graphics/PostProcessing/PostProcessor.h"
#include "Graphics/PostProcessing/Effect.h"
#include "Graphics/PostProcessing/GaussianBlur.h"
#include "Graphics/PostProcessing/Negative.h"

#include "Graphics/Shadows/ShadowMapRenderer.h"

#include "Graphics/Primitives/BufferLayout.h"
#include "Graphics/Primitives/DepthBuffer.h"
#include "Graphics/Primitives/Resources.h"
#include "Graphics/Primitives/Model2D.h"
#include "Graphics/Primitives/Model3D.h"
#include "Graphics/Primitives/Texture.h"
#include "Graphics/Primitives/CubeMap.h"
#include "Graphics/Primitives/FrameBuffer.h"
#include "Graphics/Primitives/StreamBuffer.h"

#include "Graphics/Rendering/EntityRenderer.h"
#include "Graphics/Rendering/SkyboxRenderer.h"

// AUDIO
#include "Audio/Listener.h"
#include "Audio/Music.h"
#include "Audio/Sound.h"

// OBJECTS
#include "Objects/CameraUtils.h"
#include "Objects/Light.h"
#include "Objects/Terrain.h"
#include "Objects/Skybox.h"

// SCRIPTING
#include "Scripting/LuaEngine.h"
#include "Scripting/LuaGlobals.h"
#include "Scripting/LuaTransform.h"
#include "Scripting/LuaInput.h"
#include "Scripting/LuaCamera.h"

// VENDOR: TODO - Remove this
#include "Vendor/ImGuizmo/ImGuizmo.h"