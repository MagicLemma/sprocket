// 
// THE SPROCKET ENGINE
//

// CORE
#include "Core/Window.h"
#include "Core/Events.h"
#include "Core/GameLoop.h"

// UI
#include "UI/Font/FontAtlas.h"
#include "UI/Font/Font.h"
#include "UI/Font/Glyph.h"

#include "UI/UIEngine.h"
#include "UI/SimpleUI.h"
#include "UI/SinglePanelUI.h"
#include "UI/DevUI.h"
#include "UI/ImGuiXtra.h"

// SCENE
#include "Scene/ECS.h"
#include "Scene/Camera.h"
#include "Scene/Components.h"
#include "Scene/Scene.h"
#include "Scene/Loader.h"
#include "Scene/EntitySystem.h"

#include "Scene/Systems/AnimationSystem.h"
#include "Scene/Systems/CameraSystem.h"
#include "Scene/Systems/ParticleSystem.h"
#include "Scene/Systems/PhysicsEngine3D.h"
#include "Scene/Systems/GameGrid.h"
#include "Scene/Systems/PathFollower.h"
#include "Scene/Systems/ScriptRunner.h"

// UTILITY
#include "Utility/Log.h"
#include "Utility/Colour.h"
#include "Utility/FileBrowser.h"
#include "Utility/GUID.h"
#include "Utility/HashPair.h"
#include "Utility/InputProxy.h"
#include "Utility/Maths.h"
#include "Utility/KeyboardCodes.h"
#include "Utility/Random.h"
#include "Utility/SparseSet.h"
#include "Utility/MouseCodes.h"
#include "Utility/Stopwatch.h"
#include "Utility/Types.h"
#include "Utility/Yaml.h"

// GRAPHICS
#include "Graphics/AssetManager.h"
#include "Graphics/Animation.h"
#include "Graphics/Buffer.h"
#include "Graphics/BufferLayout.h"
#include "Graphics/RenderContext.h"
#include "Graphics/Shader.h"
#include "Graphics/DepthBuffer.h"
#include "Graphics/Material.h"
#include "Graphics/Mesh.h"
#include "Graphics/Texture.h"
#include "Graphics/CubeMap.h"
#include "Graphics/FrameBuffer.h"
#include "Graphics/ShadowMap.h"
#include "Graphics/ParticleManager.h"
#include "Graphics/StreamBuffer.h"
#include "Graphics/VertexArray.h"
#include "Graphics/Viewport.h"

#include "Graphics/PostProcessing/PostProcessor.h"
#include "Graphics/PostProcessing/Effect.h"
#include "Graphics/PostProcessing/GaussianBlur.h"
#include "Graphics/PostProcessing/Negative.h"

#include "Graphics/Rendering/ColliderRenderer.h"
#include "Graphics/Rendering/Scene3DRenderer.h"
#include "Graphics/Rendering/SkyboxRenderer.h"

// AUDIO
#include "Audio/Listener.h"
#include "Audio/Music.h"
#include "Audio/Sound.h"

// SCRIPTING
#include "Scripting/LuaEngine.h"
#include "Scripting/LuaEntity.h"
#include "Scripting/LuaComponents.h"
#include "Scripting/LuaGlobals.h"

// VENDOR: TODO - Remove this
#include "Vendor/ImGuizmo/ImGuizmo.h"