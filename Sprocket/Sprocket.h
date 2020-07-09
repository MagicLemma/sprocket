// 
// THE SPROCKET ENGINE
//

// CORE
#include "Core/Window.h"
#include "Core/SceneManager.h"
#include "Core/ModelManager.h"
#include "Core/Layer.h"
#include "Core/Scene.h"

// UI
#include "UI/Font/FontAtlas.h"
#include "UI/Font/Font.h"

#include "UI/DevUI.h"
#include "UI/SimpleUI.h"

// EVENTS
#include "Events/Event.h"
#include "Events/KeyboardEvent.h"
#include "Events/MouseEvent.h"
#include "Events/WindowEvent.h"

// UTILITY
#include "Utility/Log.h"
#include "Utility/Colour.h"
#include "Utility/Maths.h"
#include "Utility/KeyboardProxy.h"
#include "Utility/KeyboardCodes.h"
#include "Utility/MouseProxy.h"
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

#include "Graphics/Rendering/DisplayRenderer.h"
#include "Graphics/Rendering/EntityRenderer.h"
#include "Graphics/Rendering/SkyboxRenderer.h"

// AUDIO
#include "Audio/Listener.h"
#include "Audio/Music.h"
#include "Audio/Sound.h"

// OBJECTS
#include "Objects/CameraUtils.h"
#include "Objects/Material.h"
#include "Objects/Quad.h"
#include "Objects/Light.h"
#include "Objects/Terrain.h"
#include "Objects/Skybox.h"

#include "Objects/Entities/Entity.h"
#include "Objects/Entities/Components.h"
#include "Objects/Entities/EntityManager.h"

// SYSTEMS
#include "Systems/EntitySystem.h"
#include "Systems/PhysicsEngine.h"
#include "Systems/Colliders.h"
#include "Systems/PlayerMovement.h"
#include "Systems/BasicSelector.h"
#include "Systems/ScriptRunner.h"
#include "Systems/Selector.h"

// DISPLAY
#include "Display/Widget.h"
#include "Display/Container.h"
#include "Display/Button.h"
#include "Display/ColourPalette.h"
#include "Display/Slider.h"
#include "Display/Image.h"

#include "Display/WidgetProperties/WidgetProperty.h"
#include "Display/WidgetProperties/Draggable.h"
#include "Display/WidgetProperties/HorizontalConstraint.h"
#include "Display/WidgetProperties/VerticalConstraint.h"

#include "Scripting/LuaEngine.h"
#include "Scripting/LuaGlobals.h"
#include "Scripting/LuaTransform.h"
#include "Scripting/LuaInput.h"
#include "Scripting/LuaCamera.h"

// VENDOR: TODO - Remove this
#include "Vendor/ImGuizmo/ImGuizmo.h"