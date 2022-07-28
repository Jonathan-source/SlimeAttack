#include "pch.h"
#include "Systems.h"
//
//void Systems::MovementSystem(float frameTime)
//{
//	entt::registry registry;
//	auto view = registry.view<comp::Transform, comp::Rigidbody>();
//
//	view.each([&](comp::Transform& transform, comp::Rigidbody& rigidbody) {
//		transform.translation.x += rigidbody.velocity.x * frameTime;
//		transform.translation.y += rigidbody.velocity.y * frameTime;
//	});
//}
//
//void Systems::DamageSystem(float frameTime)
//{
//	entt::registry registry;
//	auto view = registry.view<comp::Attack, comp::Health>();
//}
//
//void Systems::CollisionSystem(float frameTime)
//{
//	entt::registry registry;
//	auto view = registry.view<comp::Rigidbody>();
//	// Collision Raylib?
//}
//
//void Systems::ScriptSystem(float frameTime)
//{
//	entt::registry registry;
//	auto view = registry.view<comp::Script>();
//	
//	view.each([&](entt::entity entity, comp::Script& script) {
//		script.func(static_cast<uint32_t>(entity), frameTime);
//	});
//}
