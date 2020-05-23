// 
// THE SPROCKET ENGINE
//

// CORE
#include "Core/CoreSystems.h"
#include "Core/Window.h"
#include "Core/SceneManager.h"
#include "Core/ModelManager.h"
#include "Core/Layer.h"
#include "Core/Scene.h"

// DEV GUI
#include "DevGUI/DevGUI.h"

// EVENTS
#include "Events/Event.h"
#include "Events/KeyboardEvent.h"
#include "Events/MouseEvent.h"
#include "Events/WindowEvent.h"

// UTILITY
#include "Utility/Log.h"
#include "Utility/Maths.h"
#include "Utility/KeyboardProxy.h"
#include "Utility/FramerateTimer.h"
#include "Utility/Tokenize.h"

// GRAPHICS
#include "Graphics/MousePicker.h"
#include "Graphics/RenderContext.h"
#include "Graphics/Shader.h"

#include "Graphics/PostProcessing/PostProcessor.h"
#include "Graphics/PostProcessing/Effect.h"
#include "Graphics/PostProcessing/GaussianBlur.h"
#include "Graphics/PostProcessing/Negative.h"

#include "Graphics/Primitives/Resources.h"
#include "Graphics/Primitives/Model2D.h"
#include "Graphics/Primitives/Model3D.h"
#include "Graphics/Primitives/Texture.h"
#include "Graphics/Primitives/CubeMap.h"
#include "Graphics/Primitives/FrameBuffer.h"
#include "Graphics/Primitives/StreamBuffer.h"

#include "Graphics/Rendering/DevGUIRenderer.h"
#include "Graphics/Rendering/DisplayRenderer.h"
#include "Graphics/Rendering/EntityRenderer.h"
#include "Graphics/Rendering/SkyboxRenderer.h"
#include "Graphics/Rendering/TerrainRenderer.h"

// OBJECTS
#include "Objects/Material.h"
#include "Objects/Quad.h"
#include "Objects/Light.h"
#include "Objects/Terrain.h"
#include "Objects/Skybox.h"

#include "Objects/Entities/Entity.h"
#include "Objects/Entities/Components.h"
#include "Objects/Entities/EntityManager.h"

#include "Objects/Cameras/Camera.h"
#include "Objects/Cameras/FirstPersonCamera.h"
#include "Objects/Cameras/PlayerBasedCamera.h"
#include "Objects/Cameras/ThirdPersonCamera.h"

#include "Objects/Lenses/Lens.h"
#include "Objects/Lenses/PerspectiveLens.h"

// SYSTEMS
#include "Systems/EntitySystem.h"
#include "Systems/PhysicsEngine.h"
#include "Systems/Colliders.h"
#include "Systems/PlayerMovement.h"
#include "Systems/Selector.h"

// DISPLAY
#include "Display/Widget.h"
#include "Display/Container.h"
#include "Display/Button.h"
#include "Display/ColourPalette.h"
#include "Display/Slider.h"
#include "Display/Image.h"
#include "Display/TextBox.h"

#include "Display/Fonts/Character.h"
#include "Display/Fonts/Font.h"
#include "Display/Fonts/FontLoader.h"
#include "Display/Fonts/Text.h"

#include "Display/WidgetProperties/WidgetProperty.h"
#include "Display/WidgetProperties/Draggable.h"
#include "Display/WidgetProperties/HorizontalConstraint.h"
#include "Display/WidgetProperties/VerticalConstraint.h"

// VENDOR: TODO - Remove this
#include "Vendor/ImGuizmo/ImGuizmo.h"