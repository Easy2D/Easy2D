#include <easy2d/scene/scene_manager.h>
#include <easy2d/scene/transition.h>
#include <easy2d/graphics/render_backend.h>
#include <easy2d/graphics/render_command.h>
#include <easy2d/app/application.h>
#include <easy2d/platform/input.h>
#include <easy2d/utils/logger.h>
#include <algorithm>

namespace easy2d {

namespace {

Node* hitTestTopmost(const Ptr<Node>& node, const Vec2& worldPos) {
    if (!node || !node->isVisible()) {
        return nullptr;
    }

    std::vector<Ptr<Node>> children = node->getChildren();
    std::stable_sort(children.begin(), children.end(),
        [](const Ptr<Node>& a, const Ptr<Node>& b) {
            return a->getZOrder() < b->getZOrder();
        });

    for (auto it = children.rbegin(); it != children.rend(); ++it) {
        if (Node* hit = hitTestTopmost(*it, worldPos)) {
            return hit;
        }
    }

    if (node->getEventDispatcher().getTotalListenerCount() == 0) {
        return nullptr;
    }

    Rect bounds = node->getBoundingBox();
    if (!bounds.empty() && bounds.containsPoint(worldPos)) {
        return node.get();
    }

    return nullptr;
}

void dispatchToNode(Node* node, Event& event) {
    if (!node) {
        return;
    }
    node->getEventDispatcher().dispatch(event);
}

} // namespace

SceneManager& SceneManager::getInstance() {
    static SceneManager instance;
    return instance;
}

void SceneManager::runWithScene(Ptr<Scene> scene) {
    if (!scene) {
        return;
    }
    
    if (!sceneStack_.empty()) {
        E2D_LOG_WARN("SceneManager: runWithScene should only be called once");
        return;
    }
    
    scene->onEnter();
    scene->onAttachToScene(scene.get());
    sceneStack_.push(scene);
}

void SceneManager::replaceScene(Ptr<Scene> scene) {
    if (!scene || isTransitioning_) {
        return;
    }
    
    if (sceneStack_.empty()) {
        runWithScene(scene);
        return;
    }
    
    // Pop current scene
    auto oldScene = sceneStack_.top();
    oldScene->onExit();
    oldScene->onDetachFromScene();
    sceneStack_.pop();
    
    // Push new scene
    scene->onEnter();
    scene->onAttachToScene(scene.get());
    sceneStack_.push(scene);
}

void SceneManager::enterScene(Ptr<Scene> scene) {
    if (!scene || isTransitioning_) {
        return;
    }
    
    if (sceneStack_.empty()) {
        runWithScene(scene);
    } else {
        replaceScene(scene);
    }
}

void SceneManager::enterScene(Ptr<Scene> scene, Ptr<class Transition> transition) {
    if (!scene || isTransitioning_) {
        return;
    }

    if (!transition) {
        enterScene(scene);
        return;
    }

    auto current = getCurrentScene();

    // 在过渡开始前，发送 UIHoverExit 给当前悬停的节点，重置按钮状态
    if (hoverTarget_) {
        Event evt;
        evt.type = EventType::UIHoverExit;
        evt.data = CustomEvent{0, hoverTarget_};
        dispatchToNode(hoverTarget_, evt);
        hoverTarget_ = nullptr;
    }
    captureTarget_ = nullptr;
    hasLastPointerWorld_ = false;

    transition->start(current, scene);
    outgoingScene_ = current;
    incomingScene_ = scene;
    activeTransition_ = transition;
    currentTransition_ = TransitionType::None;
    isTransitioning_ = true;
    transitionStackAction_ = [this]() {
        // 退出旧场景
        auto outgoing = outgoingScene_;
        if (!sceneStack_.empty() && outgoing && sceneStack_.top() == outgoing) {
            outgoing->onExit();
            outgoing->onDetachFromScene();
            sceneStack_.pop();
        }
        // 推入新场景并调用 onEnter
        auto incoming = incomingScene_;
        if (incoming) {
            sceneStack_.push(incoming);
            if (!incoming->isRunning()) {
                incoming->onEnter();
                incoming->onAttachToScene(incoming.get());
            }
        }
    };
    // 注意：不在此处调用新场景的 onEnter，由 transitionStackAction_ 在过渡完成后调用
}

void SceneManager::replaceScene(Ptr<Scene> scene, TransitionType transition, float duration) {
    if (!scene || isTransitioning_) {
        return;
    }

    if (sceneStack_.empty()) {
        runWithScene(scene);
        return;
    }

    auto oldScene = sceneStack_.top();

    startTransition(oldScene, scene, transition, duration, [this]() {
        // 过渡完成后，退出旧场景并从堆栈中移除
        auto outgoing = outgoingScene_;
        auto incoming = incomingScene_;
        if (!sceneStack_.empty() && outgoing && sceneStack_.top() == outgoing) {
            outgoing->onExit();
            outgoing->onDetachFromScene();
            sceneStack_.pop();
        }
        // 将新场景推入堆栈并调用 onEnter
        if (incoming) {
            sceneStack_.push(incoming);
            if (!incoming->isRunning()) {
                incoming->onEnter();
                incoming->onAttachToScene(incoming.get());
            }
        }
    });
}

void SceneManager::pushScene(Ptr<Scene> scene) {
    if (!scene || isTransitioning_) {
        return;
    }
    
    // Pause current scene
    if (!sceneStack_.empty()) {
        sceneStack_.top()->pause();
    }
    
    // Push new scene
    scene->onEnter();
    scene->onAttachToScene(scene.get());
    sceneStack_.push(scene);
}

void SceneManager::pushScene(Ptr<Scene> scene, TransitionType transition, float duration) {
    if (!scene || isTransitioning_) {
        return;
    }

    if (sceneStack_.empty()) {
        runWithScene(scene);
        return;
    }

    // 暂停当前场景
    sceneStack_.top()->pause();

    auto currentScene = sceneStack_.top();

    startTransition(currentScene, scene, transition, duration, [this]() {
        // 过渡完成后，将新场景推入堆栈并调用 onEnter
        if (incomingScene_) {
            sceneStack_.push(incomingScene_);
            if (!incomingScene_->isRunning()) {
                incomingScene_->onEnter();
                incomingScene_->onAttachToScene(incomingScene_.get());
            }
        }
    });
}

void SceneManager::popScene() {
    if (sceneStack_.size() <= 1 || isTransitioning_) {
        return;
    }
    
    auto current = sceneStack_.top();
    current->onExit();
    current->onDetachFromScene();
    sceneStack_.pop();
    
    // Resume previous scene
    if (!sceneStack_.empty()) {
        sceneStack_.top()->resume();
    }
}

void SceneManager::popScene(TransitionType transition, float duration) {
    if (sceneStack_.size() <= 1 || isTransitioning_) {
        return;
    }

    auto current = sceneStack_.top();
    auto previous = getPreviousScene();

    startTransition(current, previous, transition, duration, [this]() {
        // 过渡完成后，退出当前场景并从堆栈中移除
        auto outgoing = outgoingScene_;
        if (!sceneStack_.empty() && outgoing && sceneStack_.top() == outgoing) {
            outgoing->onExit();
            outgoing->onDetachFromScene();
            sceneStack_.pop();
        }
        // 恢复前一个场景
        auto incoming = incomingScene_;
        if (!sceneStack_.empty() && incoming && sceneStack_.top() == incoming) {
            if (!incoming->isRunning()) {
                incoming->onEnter();
                incoming->onAttachToScene(incoming.get());
            }
            incoming->resume();
        }
    });
}

void SceneManager::popToRootScene() {
    if (sceneStack_.size() <= 1 || isTransitioning_) {
        return;
    }
    
    // Exit all scenes except root
    while (sceneStack_.size() > 1) {
        auto scene = sceneStack_.top();
        scene->onExit();
        scene->onDetachFromScene();
        sceneStack_.pop();
    }
    
    // Resume root
    sceneStack_.top()->resume();
}

void SceneManager::popToRootScene(TransitionType transition, float duration) {
    if (sceneStack_.size() <= 1 || isTransitioning_) {
        return;
    }

    auto root = getRootScene();
    auto current = sceneStack_.top();

    startTransition(current, root, transition, duration, [this, root]() {
        // 退出所有场景直到根场景
        while (!sceneStack_.empty() && sceneStack_.top() != root) {
            auto scene = sceneStack_.top();
            scene->onExit();
            scene->onDetachFromScene();
            sceneStack_.pop();
        }
        // 恢复根场景
        if (!sceneStack_.empty() && sceneStack_.top() == root) {
            if (!root->isRunning()) {
                root->onEnter();
                root->onAttachToScene(root.get());
            }
            root->resume();
        }
    });
}

void SceneManager::popToScene(const std::string& name) {
    if (isTransitioning_) {
        return;
    }
    
    // Find target scene in stack
    std::stack<Ptr<Scene>> tempStack;
    Ptr<Scene> target = nullptr;
    
    while (!sceneStack_.empty()) {
        auto scene = sceneStack_.top();
        if (scene->getName() == name) {
            target = scene;
            break;
        }
        scene->onExit();
        scene->onDetachFromScene();
        sceneStack_.pop();
    }
    
    if (target) {
        target->resume();
    }
}

void SceneManager::popToScene(const std::string& name, TransitionType transition, float duration) {
    if (isTransitioning_) {
        return;
    }

    auto target = getSceneByName(name);
    if (target && target != sceneStack_.top()) {
        auto current = sceneStack_.top();

        startTransition(current, target, transition, duration, [this, target]() {
            // 退出所有场景直到目标场景
            while (!sceneStack_.empty() && sceneStack_.top() != target) {
                auto scene = sceneStack_.top();
                scene->onExit();
                scene->onDetachFromScene();
                sceneStack_.pop();
            }
            // 恢复目标场景
            if (!sceneStack_.empty() && sceneStack_.top() == target) {
                if (!target->isRunning()) {
                    target->onEnter();
                    target->onAttachToScene(target.get());
                }
                target->resume();
            }
        });
    }
}

Ptr<Scene> SceneManager::getCurrentScene() const {
    if (sceneStack_.empty()) {
        return nullptr;
    }
    return sceneStack_.top();
}

Ptr<Scene> SceneManager::getPreviousScene() const {
    if (sceneStack_.size() < 2) {
        return nullptr;
    }
    
    // Copy stack to access second top
    auto tempStack = sceneStack_;
    tempStack.pop();
    return tempStack.top();
}

Ptr<Scene> SceneManager::getRootScene() const {
    if (sceneStack_.empty()) {
        return nullptr;
    }
    
    // Copy stack to access bottom
    auto tempStack = sceneStack_;
    Ptr<Scene> root;
    while (!tempStack.empty()) {
        root = tempStack.top();
        tempStack.pop();
    }
    return root;
}

Ptr<Scene> SceneManager::getSceneByName(const std::string& name) const {
    auto it = namedScenes_.find(name);
    if (it != namedScenes_.end()) {
        return it->second;
    }
    
    // Search in stack
    auto tempStack = sceneStack_;
    while (!tempStack.empty()) {
        auto scene = tempStack.top();
        if (scene->getName() == name) {
            return scene;
        }
        tempStack.pop();
    }
    
    return nullptr;
}

bool SceneManager::hasScene(const std::string& name) const {
    return getSceneByName(name) != nullptr;
}

void SceneManager::update(float dt) {
    if (isTransitioning_) {
        updateTransition(dt);
        hoverTarget_ = nullptr;
        captureTarget_ = nullptr;
        hasLastPointerWorld_ = false;
    }
    
    if (!sceneStack_.empty()) {
        auto& scene = *sceneStack_.top();
        scene.updateScene(dt);
        if (!isTransitioning_) {
            dispatchPointerEvents(scene);
        }
    }
}

void SceneManager::render(RenderBackend& renderer) {
    Color clearColor = Colors::Black;
    if (isTransitioning_) {
        if (outgoingScene_) {
            clearColor = outgoingScene_->getBackgroundColor();
        } else if (incomingScene_) {
            clearColor = incomingScene_->getBackgroundColor();
        }
    } else if (!sceneStack_.empty()) {
        clearColor = sceneStack_.top()->getBackgroundColor();
    }

    renderer.beginFrame(clearColor);
    if (isTransitioning_ && activeTransition_) {
        activeTransition_->render(renderer);
    } else if (!sceneStack_.empty()) {
        sceneStack_.top()->renderContent(renderer);
    }
    renderer.endFrame();
}

void SceneManager::collectRenderCommands(std::vector<RenderCommand>& commands) {
    if (isTransitioning_ && outgoingScene_) {
        // During transition, collect commands from both scenes
        outgoingScene_->collectRenderCommands(commands);
        if (incomingScene_) {
            incomingScene_->collectRenderCommands(commands);
        }
    } else if (!sceneStack_.empty()) {
        sceneStack_.top()->collectRenderCommands(commands);
    }
}

void SceneManager::end() {
    while (!sceneStack_.empty()) {
        auto scene = sceneStack_.top();
        scene->onExit();
        scene->onDetachFromScene();
        sceneStack_.pop();
    }
    namedScenes_.clear();
}

void SceneManager::purgeCachedScenes() {
    namedScenes_.clear();
}

void SceneManager::startTransition(Ptr<Scene> from, Ptr<Scene> to, TransitionType type, float duration, Function<void()> stackAction) {
    if (!from || !to) {
        return;
    }

    Ptr<Transition> transition;
    switch (type) {
        case TransitionType::Fade:
            transition = makePtr<FadeTransition>(duration);
            break;
        case TransitionType::SlideLeft:
            transition = makePtr<SlideTransition>(duration, TransitionDirection::Left);
            break;
        case TransitionType::SlideRight:
            transition = makePtr<SlideTransition>(duration, TransitionDirection::Right);
            break;
        case TransitionType::SlideUp:
            transition = makePtr<SlideTransition>(duration, TransitionDirection::Up);
            break;
        case TransitionType::SlideDown:
            transition = makePtr<SlideTransition>(duration, TransitionDirection::Down);
            break;
        case TransitionType::Scale:
            transition = makePtr<ScaleTransition>(duration);
            break;
        case TransitionType::Flip:
            transition = makePtr<FlipTransition>(duration);
            break;
        default:
            transition = makePtr<FadeTransition>(duration);
            break;
    }

    transition->start(from, to);

    // 在过渡开始前，发送 UIHoverExit 给当前悬停的节点，重置按钮状态
    if (hoverTarget_) {
        Event evt;
        evt.type = EventType::UIHoverExit;
        evt.data = CustomEvent{0, hoverTarget_};
        dispatchToNode(hoverTarget_, evt);
        hoverTarget_ = nullptr;
    }
    captureTarget_ = nullptr;
    hasLastPointerWorld_ = false;

    isTransitioning_ = true;
    currentTransition_ = type;
    transitionDuration_ = duration;
    transitionElapsed_ = 0.0f;
    outgoingScene_ = from;
    incomingScene_ = to;
    activeTransition_ = transition;
    transitionStackAction_ = std::move(stackAction);

    // 注意：不在此处调用新场景的 onEnter，由 transitionStackAction_ 在过渡完成后调用
}

void SceneManager::updateTransition(float dt) {
    transitionElapsed_ += dt;

    if (activeTransition_) {
        activeTransition_->update(dt);
        if (activeTransition_->isFinished()) {
            finishTransition();
        }
    } else {
        finishTransition();
    }
}

void SceneManager::finishTransition() {
    // 先保存当前悬停的节点，然后在 transitionStackAction_ 之后发送 UIHoverExit
    Node* lastHoverTarget = hoverTarget_;

    isTransitioning_ = false;
    hoverTarget_ = nullptr;
    captureTarget_ = nullptr;
    hasLastPointerWorld_ = false;

    if (transitionStackAction_) {
        transitionStackAction_();
    }

    // 在 transitionStackAction_ 之后发送 UIHoverExit，确保旧场景仍然有效
    if (lastHoverTarget) {
        Event evt;
        evt.type = EventType::UIHoverExit;
        evt.data = CustomEvent{0, lastHoverTarget};
        dispatchToNode(lastHoverTarget, evt);
    }

    outgoingScene_.reset();
    incomingScene_.reset();
    activeTransition_.reset();
    transitionStackAction_ = nullptr;
    
    if (transitionCallback_) {
        transitionCallback_();
        transitionCallback_ = nullptr;
    }
}

void SceneManager::dispatchPointerEvents(Scene& scene) {
    auto& input = Application::instance().input();
    Vec2 screenPos = input.getMousePosition();

    Vec2 worldPos = screenPos;
    if (auto* camera = scene.getActiveCamera()) {
        worldPos = camera->screenToWorld(screenPos);
    }

    Ptr<Node> root = scene.shared_from_this();
    Node* newHover = hitTestTopmost(root, worldPos);

    if (newHover != hoverTarget_) {
        if (hoverTarget_) {
            Event evt;
            evt.type = EventType::UIHoverExit;
            evt.data = CustomEvent{0, hoverTarget_};
            dispatchToNode(hoverTarget_, evt);
        }
        hoverTarget_ = newHover;
        if (hoverTarget_) {
            Event evt;
            evt.type = EventType::UIHoverEnter;
            evt.data = CustomEvent{0, hoverTarget_};
            dispatchToNode(hoverTarget_, evt);
        }
    }

    if (!hasLastPointerWorld_) {
        lastPointerWorld_ = worldPos;
        hasLastPointerWorld_ = true;
    }

    Vec2 delta = worldPos - lastPointerWorld_;
    if (hoverTarget_ && (delta.x != 0.0f || delta.y != 0.0f)) {
        Event evt = Event::createMouseMove(worldPos, delta);
        dispatchToNode(hoverTarget_, evt);
    }

    float scrollDelta = input.getMouseScrollDelta();
    if (hoverTarget_ && scrollDelta != 0.0f) {
        Event evt = Event::createMouseScroll(Vec2(0.0f, scrollDelta), worldPos);
        dispatchToNode(hoverTarget_, evt);
    }

    if (input.isMousePressed(MouseButton::Left)) {
        captureTarget_ = hoverTarget_;
        if (captureTarget_) {
            Event evt = Event::createMouseButtonPress(static_cast<int>(MouseButton::Left), 0, worldPos);
            dispatchToNode(captureTarget_, evt);

            Event pressed;
            pressed.type = EventType::UIPressed;
            pressed.data = CustomEvent{0, captureTarget_};
            dispatchToNode(captureTarget_, pressed);
        }
    }

    if (input.isMouseReleased(MouseButton::Left)) {
        Node* target = captureTarget_ ? captureTarget_ : hoverTarget_;
        if (target) {
            Event evt = Event::createMouseButtonRelease(static_cast<int>(MouseButton::Left), 0, worldPos);
            dispatchToNode(target, evt);

            Event released;
            released.type = EventType::UIReleased;
            released.data = CustomEvent{0, target};
            dispatchToNode(target, released);
        }

        if (captureTarget_ && captureTarget_ == hoverTarget_) {
            Event clicked;
            clicked.type = EventType::UIClicked;
            clicked.data = CustomEvent{0, captureTarget_};
            dispatchToNode(captureTarget_, clicked);
        }

        captureTarget_ = nullptr;
    }

    lastPointerWorld_ = worldPos;
}

} // namespace easy2d
