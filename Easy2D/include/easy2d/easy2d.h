#pragma once

// Easy2D v3.0 - 统一入口头文件
// 包含所有公共 API

// Core
#include <easy2d/core/types.h>
#include <easy2d/core/string.h>
#include <easy2d/core/color.h>
#include <easy2d/core/math_types.h>

// Platform
#include <easy2d/platform/window.h>
#include <easy2d/platform/input.h>

// Graphics
#include <easy2d/graphics/render_backend.h>
#include <easy2d/graphics/texture.h>
#include <easy2d/graphics/font.h>
#include <easy2d/graphics/camera.h>

// Scene
#include <easy2d/scene/node.h>
#include <easy2d/scene/scene.h>
#include <easy2d/scene/sprite.h>
#include <easy2d/scene/text.h>
#include <easy2d/scene/shape_node.h>
#include <easy2d/scene/scene_manager.h>
#include <easy2d/scene/transition.h>

// UI
#include <easy2d/ui/widget.h>
#include <easy2d/ui/button.h>

// Action
#include <easy2d/action/action.h>
#include <easy2d/action/actions.h>
#include <easy2d/action/ease.h>

// Event
#include <easy2d/event/event.h>
#include <easy2d/event/event_queue.h>
#include <easy2d/event/event_dispatcher.h>
#include <easy2d/event/input_codes.h>

// Audio
#include <easy2d/audio/audio_engine.h>
#include <easy2d/audio/sound.h>

// Resource
#include <easy2d/resource/resource_manager.h>

// Utils
#include <easy2d/utils/logger.h>
#include <easy2d/utils/timer.h>
#include <easy2d/utils/data.h>
#include <easy2d/utils/random.h>

// Spatial
#include <easy2d/spatial/spatial_index.h>
#include <easy2d/spatial/quadtree.h>
#include <easy2d/spatial/spatial_hash.h>
#include <easy2d/spatial/spatial_manager.h>

// Application
#include <easy2d/app/application.h>
